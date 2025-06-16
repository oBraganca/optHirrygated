#ifndef REFINEMENT_HEURISTIC_HPP
#define REFINEMENT_HEURISTIC_HPP

#include "Instance.hpp"
#include "Solution.hpp"

namespace opthirrygated {

    class RefinementHeuristic {
    private:
        Instance& inst;
    public:
        RefinementHeuristic(Instance &instance);
        Solution executeA(Solution &solution);
        Solution executeB(Solution &solution);
        Solution findBestNeighbor(Solution &solution, int start, int end);
        Solution executeSA(Solution &solution, double T=1000.0, double T_min=1e-3, double alpha=0.97, int iterPerT=1000);
        bool propagate(Solution &solution, int d, float val);

        bool isFeasible(const Solution &solution, int d) const;
    };

} // namespace opthirrygated

#endif // REFINEMENT_HEURISTIC_HPP
