#include "../include/RefinementHeuristic.hpp"
#include "../include/ConstructiveHeuristic.hpp"
#include "../include/Measurer.hpp"
#include <iostream>
#include <algorithm>    // for std::swap
#include <cmath>
#include <random>

#define LEN 7
namespace opthirrygated {

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    RefinementHeuristic::RefinementHeuristic(Instance &instance)
            : inst(instance)
    {}


    Solution RefinementHeuristic::executeA(Solution &solution) {
        int totalSize = solution.getSolution().size();
        int numBlocks = (totalSize + LEN - 1) / LEN; // Ceiling division
        Measurer measurer(inst);
        int k = 1;

        while (k <= numBlocks) {
            int startIdx = (k - 1) * LEN;
            int endIdx = std::min(k * LEN - 1, totalSize - 1);

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

    Solution RefinementHeuristic::executeB(Solution &solution) {
        int totalSize = solution.getSolution().size();
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

        // Boundary check
        if (startIdx < 0 || endIdx >= solutionSize || startIdx > endIdx) {
            return bestSolution;
        }

        std::vector<int> rangeValues;
        for (int i = startIdx; i <= endIdx; i++) {
            rangeValues.push_back(solution.getSolution()[i]);
        }

        std::sort(rangeValues.begin(), rangeValues.end());

        float bestCost = measurer.evaluate(solution);

        do {
            Solution candidate = solution;

            for (int i = 0; i < rangeValues.size(); i++) {
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

    bool RefinementHeuristic::isFeasible(const Solution &solution, int d) const{
        auto adfSolutions = solution.getAdfSolutions();
        const auto& solutionVec = solution.getSolution();

        float adi, adf;
        if (d == 0) {
            adi = inst.getCad()[0];
        }

        for (int day = d; day <= adfSolutions.size(); day++) {
            float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[solutionVec[day]];
            float e = 0.0001;

            if (auxAdf + e < inst.getLc()[day]) {
                return false;
            }

            adfSolutions[day] = auxAdf;

            adi = auxAdf;
        }
        return true;
    }

    bool RefinementHeuristic::propagate(Solution &solution, int d, float val) {
        for (int i = d; i < solution.getAdfSolutions().size(); ++i) {
            float auxVal = val + solution.getAdfSolutions()[i];
            solution.updateAdfSolution(i, auxVal);
        }
        return isFeasible(solution, d);
    }

    Solution RefinementHeuristic::executeSA(Solution &solution, double T, double T_min, double alpha, int iterPerT){
        Measurer measurer(inst);
        float C = measurer.evaluate(solution);
        double T_initial = T;

        std::uniform_int_distribution<int> dayDist(0, solution.getAdfSolutions().size()-1);

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
            std::cout << "T=" << T << ", best cost=" << C << std::endl;
        }

        return solution;
    }
}