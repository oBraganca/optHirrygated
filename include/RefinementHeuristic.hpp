#ifndef REFINEMENT_HEURISTIC_HPP
#define REFINEMENT_HEURISTIC_HPP

#include "Instance.hpp"
#include "Solution.hpp"
#include "Measurer.hpp"
#include <memory>
#include <vector>
#include <random>
#include <unordered_map>
#include <functional>

namespace opthirrygated {

    static constexpr int LEN = 7;

    struct MCTSNode {
        Solution sol;
        MCTSNode* parent;
        std::vector<std::unique_ptr<MCTSNode>> children;
        double W = 0.0;    // total reward (negative cost)
        int N = 0;         // visit count
        bool fullyExpanded = false;
        MCTSNode(const Solution& s, MCTSNode* p = nullptr)
                : sol(s), parent(p) {}
    };

    class RefinementHeuristic {
    private:
        Instance& inst;
        std::mt19937_64 rng;
        std::uniform_real_distribution<double> unif;

        // MCTS parameters
        double mctsC;
        int mctsIters;
        int mctsRolloutDepth;

        MCTSNode* expand(MCTSNode* node);
        void backup(MCTSNode* node, double reward);

        // Improved MCTS methods
        MCTSNode* improvedTreePolicy(MCTSNode* node);
        MCTSNode* improvedExpand(MCTSNode* node);
        MCTSNode* improvedBestChild(MCTSNode* node, double c);
        double improvedDefaultPolicy(const Solution& sol,
                                     unordered_map<int, int>& irrigationFreq,
                                     unordered_map<std::string, double>& patternRewards);

        // Helper methods
        bool isValidCandidate(const Solution& candidate, const Solution& original);
        double calculatePatternBonus(const Solution& sol);
    public:
        RefinementHeuristic(Instance &instance,
                            double C = 1.4142,
                            int iter = 1000,
                            int rollout_d = 20)
                : inst(instance),
                  rng(std::random_device{}()),
                  unif(0.0,1.0),
                  mctsC(C),
                  mctsIters(iter),
                  mctsRolloutDepth(rollout_d) {}

        Solution executeA(Solution &solution);
        Solution executeB(Solution &solution);
        Solution findBestNeighbor(Solution &solution, int start, int end);
        Solution executeSA(Solution &solution,
                           double T = 1000.0,
                           double T_min = 1e-3,
                           double alpha = 0.97,
                           int iterPerT = 1000);
        Solution executeMCTS(const Solution &rootSolution);
        bool propagate(Solution &solution, int d, float val);
        bool isFeasible(const Solution &solution, int d) const;
    };

} // namespace opthirrygated
#endif // REFINEMENT_HEURISTIC_HPP
