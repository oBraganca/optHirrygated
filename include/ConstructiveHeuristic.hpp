#ifndef CONSTRUCTIVE_HEURISTIC_HPP
#define CONSTRUCTIVE_HEURISTIC_HPP

#include "Instance.hpp"
#include "Solution.hpp"

using namespace std;

namespace opthirrygated{
    class ConstructiveHeuristic{
    private:
        Instance& inst;
    public:
        ConstructiveHeuristic(Instance &instance);
        virtual Solution execute();
    };
}

#endif