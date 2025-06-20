#include "../include/RefinementHeuristic.hpp"
#include "../include/ConstructiveHeuristic.hpp"
#include "../include/Measurer.hpp"
#include <iostream>
#include <algorithm>    // for std::swap
#include <cmath>
#include <random>
#include <limits>
#include <iomanip>

#define LEN 7
namespace opthirrygated {

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> unif(0.0, 1.0);

    Solution RefinementHeuristic::executeA(Solution &solution) {
        int totalSize = solution.getSolution().size();
        if (totalSize == 0) return solution; // Safety check

        int numBlocks = (totalSize + LEN - 1) / LEN; // Ceiling division
        Measurer measurer(inst);
        int k = 1;

        while (k <= numBlocks) {
            int startIdx = (k - 1) * LEN;
            int endIdx = std::min(k * LEN - 1, totalSize - 1);

            //Solution aux = findBestNeighbor(solution, startIdx, endIdx);
            Solution aux = executeMCTS(solution);
            if (measurer.evaluate(aux) < measurer.evaluate(solution)) {
                solution = aux;
                k = 1; // Reset to first block when improvement found
            } else {
                ++k;
            }
        }
        return solution;
    }

    Solution RefinementHeuristic::executeB(Solution &solution) {
        int totalSize = solution.getSolution().size();
        if (totalSize < LEN) return solution; // Safety check

        int maxBlocks = totalSize - LEN + 1; // Maximum number of overlapping blocks
        Measurer measurer(inst);
        int k = 1;

        while (k <= maxBlocks) {
            int startIdx = k - 1; // Starts at 0, then 1, then 2, etc.
            int endIdx = startIdx + LEN - 1;

            Solution aux = findBestNeighbor(solution, startIdx, endIdx);

            if (measurer.evaluate(aux) < measurer.evaluate(solution)) {
                solution = aux;
                k = 1; // Reset to first block when improvement found
            } else {
                ++k;
            }
        }
        return solution;
    }

    Solution RefinementHeuristic::findBestNeighbor(Solution &solution, int startIdx, int endIdx) {
        Solution bestSolution = solution;
        Measurer measurer(inst);

        int solutionSize = solution.getSolution().size();

        // Enhanced boundary check
        if (startIdx < 0 || endIdx >= solutionSize || startIdx > endIdx || solutionSize == 0) {
            return bestSolution;
        }

        std::vector<int> rangeValues;
        for (int i = startIdx; i <= endIdx; i++) {
            rangeValues.push_back(solution.getSolution()[i]);
        }

        if (rangeValues.empty()) return bestSolution; // Safety check

        std::sort(rangeValues.begin(), rangeValues.end());

        float bestCost = measurer.evaluate(solution);

        do {
            Solution candidate = solution;

            // Ensure we don't go out of bounds
            for (int i = 0; i < static_cast<int>(rangeValues.size()) && (startIdx + i) < solutionSize; i++) {
                candidate.getSolution()[startIdx + i] = rangeValues[i];
            }

            if (measurer.validationRange(candidate, startIdx, endIdx)) {
                float candidateCost = measurer.evaluate(candidate);

                if (candidateCost < bestCost) {
                    bestSolution = candidate;
                    bestCost = candidateCost;
                }
            }

        } while (std::next_permutation(rangeValues.begin(), rangeValues.end()));

        return bestSolution;
    }

    bool RefinementHeuristic::isFeasible(const Solution &solution, int d) const {
        auto adfSolutions = solution.getAdfSolutions();
        const auto& solutionVec = solution.getSolution();

        // Safety checks
        if (d < 0 || adfSolutions.empty() || solutionVec.empty()) {
            return false;
        }

        float adi, adf;
        if (d == 0) {
            if (inst.getCad().empty()) return false; // Safety check
            adi = inst.getCad()[0];
        }

        // Fixed loop condition - should be < not <=
        for (int day = d; day < static_cast<int>(adfSolutions.size()); day++) {
            // Safety checks for array access
            if (day >= static_cast<int>(inst.getEtc().size()) ||
                day >= static_cast<int>(inst.getPrec().size()) ||
                day >= static_cast<int>(inst.getLc().size()) ||
                day >= static_cast<int>(solutionVec.size())) {
                return false;
            }

            if (solutionVec[day] >= static_cast<int>(inst.getLamp().size())) {
                return false;
            }

            float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[solutionVec[day]];
            float e = 0.0001;

            if (auxAdf + e < inst.getLc()[day]) {
                return false;
            }

            // Note: This modifies a copy, not the original
            adfSolutions[day] = auxAdf;
            adi = auxAdf;
        }
        return true;
    }

    bool RefinementHeuristic::propagate(Solution &solution, int d, float val) {
        std::vector<float> adfSolutions = solution.getAdfSolutions(); // Get copy, not reference

        if (d < 0 || d >= static_cast<int>(adfSolutions.size())) {
            return false; // Safety check
        }

        for (int i = d; i < static_cast<int>(adfSolutions.size()); ++i) {
            float auxVal = val + adfSolutions[i];
            solution.updateAdfSolution(i, auxVal);
        }
        return isFeasible(solution, d);
    }

    Solution RefinementHeuristic::executeSA(Solution &solution, double T, double T_min, double alpha, int iterPerT){
        Measurer measurer(inst);
        float C = measurer.evaluate(solution);
        double T_initial = T;

        int solutionSize = solution.getAdfSolutions().size();
        if (solutionSize == 0) return solution; // Safety check

        std::uniform_int_distribution<int> dayDist(0, solutionSize - 1);

        while (T > T_min) {
            for (int iter = 0; iter < iterPerT; ++iter) {
                int d = dayDist(rng);

                Solution candidate = solution;

                double tempRatio = T / T_initial;
                int maxPerturbation = std::max(1, static_cast<int>(inst.getPerc().size() * 0.1));
                int perturbationLevel = std::max(1, static_cast<int>(maxPerturbation * tempRatio));

                std::uniform_int_distribution<int> stepDist(-perturbationLevel, perturbationLevel);
                int step = stepDist(rng);

                int currentIdx = candidate.getSolution()[d];
                int newIdx = std::max(0, std::min(currentIdx + step, static_cast<int>(inst.getPerc().size()) - 1));

                if (newIdx == currentIdx) continue;

                float oldAdf = candidate.getAdfSolutions()[d];
                candidate.updateSolution(d, newIdx);
                float newAdf = candidate.getAdfSolutions()[d];
                float deltaAdf = newAdf - oldAdf;

                if (!propagate(candidate, d, deltaAdf)) continue;

                float C2 = measurer.evaluate(candidate);
                double delta = C2 - C;

                if (delta < 0 || unif(rng) < std::exp(-delta / T)) {
                    solution = std::move(candidate);
                    C = C2;
                }
            }
            T *= alpha;
            ////std::cout << "T=" << T << ", best cost=" << C << std::endl;
        }

        return solution;
    }

    Solution RefinementHeuristic::executeMCTS(const Solution &rootSolution) {
        ////std::cout << "\n=== IMPROVED MCTS EXECUTION START ===" << std::endl;

        Measurer measurer(inst);
        float initialCost = measurer.evaluate(const_cast<Solution &>(rootSolution));
        ////std::cout << "Initial solution cost: " << initialCost << std::endl;

        MCTSNode root(rootSolution, nullptr);
        Solution globalBest = rootSolution;
        float globalBestCost = initialCost;

        // Track irrigation pattern statistics
        std::unordered_map<int, int> irrigationFrequency;
        std::unordered_map<std::string, double> patternRewards;

        for (int it = 0; it < mctsIters; ++it) {
            if ((it + 1) % 500 == 0) {
                ////std::cout << "Iteration " << (it + 1) << "/" << mctsIters
                          //<< " - Best cost: " << globalBestCost << std::endl;
            }

            MCTSNode* node = improvedTreePolicy(&root);
            if (!node) break;

            double reward = improvedDefaultPolicy(node->sol, irrigationFrequency, patternRewards);
            backup(node, reward);

            // Update global best
            float currentCost = measurer.evaluate(node->sol);
            if (currentCost < globalBestCost) {
                globalBestCost = currentCost;
                globalBest = node->sol;
                //std::cout << "*** NEW GLOBAL BEST: " << globalBestCost << " at iteration " << (it + 1) << std::endl;
            }

            // Adaptive exploration: increase exploration if no improvement for a while
            if (it > 0 && it % 1000 == 0) {
                mctsC = std::min(mctsC * 1.1, 3.0); // Increase exploration
            }
        }

        //std::cout << "\n=== MCTS EXECUTION COMPLETE ===" << std::endl;
        //std::cout << "Final best cost: " << globalBestCost << std::endl;
        //std::cout << "Improvement: " << (initialCost - globalBestCost) << std::endl;

        // Print irrigation pattern analysis
        //std::cout << "\n--- Irrigation Pattern Analysis ---" << std::endl;
        for (const auto& [level, freq] : irrigationFrequency) {
            //std::cout << "Level " << level << ": " << freq << " times" << std::endl;
        }

        return globalBest;
    }

    MCTSNode* RefinementHeuristic::improvedTreePolicy(MCTSNode* node) {
        int maxDepth = 50;
        int depth = 0;

        while (node && depth < maxDepth) {
            if (!node->fullyExpanded) {
                return improvedExpand(node);
            } else {
                node = improvedBestChild(node, mctsC);
                if (!node) break;
            }
            depth++;
        }
        return node;
    }

    MCTSNode* RefinementHeuristic::improvedExpand(MCTSNode* node) {
        if (!node) return nullptr;

        Solution s = node->sol;
        int D = s.getAdfSolutions().size();
        if (D == 0) return node;

        // Strategy 1: Focus on days with high irrigation (>= 10)
        std::vector<int> highIrrigationDays;
        for (int day = 0; day < D; ++day) {
            if (s.getSolution()[day] < 10 && s.getSolution()[day] > 2) {
                highIrrigationDays.push_back(day);
            }
        }

        // Strategy 2: Try different expansion approaches
        std::vector<std::function<Solution()>> strategies = {
                // Reduce high irrigation to minimal levels
                [&]() {
                    Solution cand = s;
                    if (!highIrrigationDays.empty()) {
                        std::uniform_int_distribution<int> dayDist(0, highIrrigationDays.size() - 1);
                        int day = highIrrigationDays[dayDist(rng)];
                        std::uniform_int_distribution<int> levelDist(0, 2); // Try 0, 1, 2
                        int newLevel = levelDist(rng);
                        cand.updateSolution(day, newLevel);
                    }
                    return cand;
                },

                // Pattern-based: create sequences of low irrigation
                [&]() {
                    Solution cand = s;
                    std::uniform_int_distribution<int> dayDist(0, D - 5);
                    int startDay = dayDist(rng);
                    std::uniform_int_distribution<int> levelDist(1, 2); // Focus on 1 and 2
                    int level = levelDist(rng);

                    // Apply to a sequence of 3-5 days
                    int seqLength = std::uniform_int_distribution<int>(3, 5)(rng);
                    for (int i = 0; i < seqLength && (startDay + i) < D; ++i) {
                        cand.updateSolution(startDay + i, level);
                    }
                    return cand;
                },

                // Gradual reduction: step down irrigation levels
                [&]() {
                    Solution cand = s;
                    std::uniform_int_distribution<int> dayDist(0, D - 1);
                    int day = dayDist(rng);
                    int current = cand.getSolution()[day];

                    if (current > 2) {
                        // Step down towards minimal irrigation
                        int newLevel = std::max(0, current - std::uniform_int_distribution<int>(1, 5)(rng));
                        cand.updateSolution(day, newLevel);
                    }
                    return cand;
                },

                // Smart replacement: replace 10s with optimal low levels
                [&]() {
                    Solution cand = s;
                    for (int day = 0; day < D; ++day) {
                        if (cand.getSolution()[day] == 10) {
                            // Try to find the best low-level replacement
                            float bestCost = std::numeric_limits<float>::max();
                            int bestLevel = 10;

                            for (int level : { 1, 2}) {
                                Solution temp = cand;
                                temp.updateSolution(day, level);

                                // Quick feasibility check
                                float oldAdf = cand.getAdfSolutions()[day];
                                float newAdf = temp.getAdfSolutions()[day];
                                float delta = newAdf - oldAdf;

                                if (propagate(temp, day, delta)) {
                                    Measurer measurer(inst);
                                    float cost = measurer.evaluate(temp);
                                    if (cost < bestCost) {
                                        bestCost = cost;
                                        bestLevel = level;
                                    }
                                }
                            }

                            if (bestLevel != 10) {
                                cand.updateSolution(day, bestLevel);
                                break; // Only change one day per expansion
                            }
                        }
                    }
                    return cand;
                }
        };

        // Try each strategy
        for (int attempt = 0; attempt < 10 && node->children.size() < 8; ++attempt) {
            std::uniform_int_distribution<int> strategyDist(0, strategies.size() - 1);
            int strategyIdx = strategyDist(rng);

            Solution candidate = strategies[strategyIdx]();

            // Validate the candidate
            if (isValidCandidate(candidate, s)) {
                auto child = std::make_unique<MCTSNode>(candidate, node);
                node->children.push_back(std::move(child));

                if (node->children.size() >= 8) {
                    node->fullyExpanded = true;
                }
                return node->children.back().get();
            }
        }

        node->fullyExpanded = true;
        return node;
    }

    bool RefinementHeuristic::isValidCandidate(const Solution& candidate, const Solution& original) {
        // Check if candidate is different and feasible
        const auto& candSol = candidate.getSolution();
        const auto& origSol = original.getSolution();

        bool isDifferent = false;
        for (size_t i = 0; i < std::min(candSol.size(), origSol.size()); ++i) {
            if (candSol[i] != origSol[i]) {
                isDifferent = true;
                break;
            }
        }

        if (!isDifferent) return false;

        // Quick feasibility check
        return isFeasible(candidate, 0);
    }

    MCTSNode* RefinementHeuristic::improvedBestChild(MCTSNode* node, double c) {
        if (!node || node->children.empty()) return nullptr;

        MCTSNode* best = nullptr;
        double bestScore = -std::numeric_limits<double>::infinity();

        for (auto& child : node->children) {
            if (child->N > 0) {
                double exploit = child->W / child->N;
                double explore = c * std::sqrt(std::log(node->N) / child->N);
                double score = exploit + explore;

                if (score > bestScore) {
                    bestScore = score;
                    best = child.get();
                }
            }
        }

        return best;
    }

    double RefinementHeuristic::improvedDefaultPolicy(const Solution& sol,
                                                      std::unordered_map<int, int>& irrigationFreq,
                                                      std::unordered_map<std::string, double>& patternRewards) {
        Solution sim = sol;
        Measurer measurer(inst);
        float initialCost = measurer.evaluate(sim);

        // Focused rollout strategies
        int D = sim.getAdfSolutions().size();
        if (D == 0) return -initialCost;

        // Strategy: Aggressive reduction of high irrigation
        std::vector<int> highIrrigationDays;
        for (int day = 0; day < D; ++day) {
            if (sol.getSolution()[day] < 10 && sol.getSolution()[day] > 2 ) {
                highIrrigationDays.push_back(day);
            }
        }

        int improvements = 0;
        int maxMoves = std::min(mctsRolloutDepth, static_cast<int>(highIrrigationDays.size()));

        for (int move = 0; move < maxMoves; ++move) {
            if (highIrrigationDays.empty()) break;

            // Select a day with high irrigation
            std::uniform_int_distribution<int> dayDist(0, highIrrigationDays.size() - 1);
            int dayIdx = dayDist(rng);
            int day = highIrrigationDays[dayIdx];

            // Try reducing to minimal irrigation levels
            std::vector<int> reductionLevels = {0, 1, 2};
            std::shuffle(reductionLevels.begin(), reductionLevels.end(), rng);

            bool foundValidMove = false;
            for (int newLevel : reductionLevels) {
                Solution candidate = sim;
                int oldLevel = candidate.getSolution()[day];

                if (oldLevel == newLevel) continue;

                candidate.updateSolution(day, newLevel);
                float oldAdf = sim.getAdfSolutions()[day];
                float newAdf = candidate.getAdfSolutions()[day];
                float delta = newAdf - oldAdf;

                if (propagate(candidate, day, delta)) {
                    float newCost = measurer.evaluate(candidate);
                    if (newCost <= initialCost * 1.05) { // Allow slight cost increase for exploration
                        sim = candidate;
                        improvements++;
                        irrigationFreq[newLevel]++;

                        // Remove this day from high irrigation list
                        highIrrigationDays.erase(highIrrigationDays.begin() + dayIdx);
                        foundValidMove = true;
                        break;
                    }
                }
            }

            if (!foundValidMove) {
                // Remove the day from consideration if no valid move found
                highIrrigationDays.erase(highIrrigationDays.begin() + dayIdx);
            }
        }

        float finalCost = measurer.evaluate(sim);

        // Enhanced reward function
        double costImprovement = (initialCost - finalCost) / initialCost;
        double improvementBonus = improvements * 0.1;
        double patternBonus = calculatePatternBonus(sim);

        double reward = costImprovement + improvementBonus + patternBonus;

        return reward;
    }

    double RefinementHeuristic::calculatePatternBonus(const Solution& sol) {
        const auto& solution = sol.getSolution();
        double bonus = 0.0;

        // Bonus for consecutive low irrigation levels
        int consecutiveLow = 0;
        int maxConsecutiveLow = 0;

        // Bonus for reducing high irrigation (10s)
        int reducedHighCount = 0;

        for (int day = 0; day < static_cast<int>(solution.size()); ++day) {
            int level = solution[day];

            // Count consecutive low irrigation (0, 1, 2)
            if (level <= 2) {
                consecutiveLow++;
                maxConsecutiveLow = std::max(maxConsecutiveLow, consecutiveLow);
            } else {
                consecutiveLow = 0;
            }

            // Count reduced high irrigation
            if (level < 10) {
                reducedHighCount++;
            }
        }

        // Bonuses
        bonus += maxConsecutiveLow * 0.02; // Reward long sequences of low irrigation
        bonus += (static_cast<double>(reducedHighCount) / solution.size()) * 0.5; // Reward overall reduction

        return bonus;
    }

    MCTSNode* RefinementHeuristic::expand(MCTSNode* node) {
        if (!node) return nullptr;

        //std::cout << "    Expand: trying to create new child..." << std::endl;

        Solution s = node->sol;
        int D = s.getAdfSolutions().size();
        if (D == 0) {
            //std::cout << "    Expand: empty solution!" << std::endl;
            return node;
        }

        std::uniform_int_distribution<int> dayDist(0, D-1);
        int d = dayDist(rng);
        int cur = s.getSolution()[d];
        int maxIdx = static_cast<int>(inst.getPerc().size()) - 1;

        if (maxIdx < 0) {
            //std::cout << "    Expand: invalid maxIdx!" << std::endl;
            return node;
        }

        std::uniform_int_distribution<int> stepDist(-1, 1);

        for (int attempt = 0; attempt < 5; ++attempt) {
            int ns = std::clamp(cur + stepDist(rng), 0, maxIdx);
            //std::cout << "    Expand attempt " << attempt << ": day=" << d
                      //<< ", current=" << cur << ", new=" << ns << std::endl;

            if (ns != cur) {
                Solution cand = s;
                cand.updateSolution(d, ns);
                float delta = cand.getAdfSolutions()[d] - node->sol.getAdfSolutions()[d];

                //std::cout << "    Expand: testing feasibility with delta=" << delta << std::endl;

                if (propagate(cand, d, delta)) {
                    //std::cout << "    Expand: SUCCESS! Creating new child" << std::endl;
                    auto child = std::make_unique<MCTSNode>(cand, node);
                    node->children.push_back(std::move(child));
                    if (node->children.size() >= 5) {
                        node->fullyExpanded = true;
                        //std::cout << "    Expand: Node now fully expanded (5 children)" << std::endl;
                    }
                    return node->children.back().get();
                } else {
                    //std::cout << "    Expand: move not feasible" << std::endl;
                }
            } else {
                //std::cout << "    Expand: no change in solution" << std::endl;
            }
        }

        //std::cout << "    Expand: No valid children found, marking as fully expanded" << std::endl;
        node->fullyExpanded = true;
        return node;
    }

    void RefinementHeuristic::backup(MCTSNode* node, double reward) {
        //std::cout << "    Backup: propagating reward=" << reward << std::endl;

        int level = 0;
        while (node) {
            node->N += 1;
            node->W += reward;
            //std::cout << "    Backup level " << level << ": N=" << node->N
                      //<< ", W=" << node->W
                      //<< ", avg=" << (node->W / node->N) << std::endl;
            node = node->parent;
            level++;
        }
    }

}