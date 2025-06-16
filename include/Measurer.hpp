#ifndef MEASURER_HPP
#define MEASURER_HPP

#include "Instance.hpp"
#include "Solution.hpp"

namespace opthirrygated {
    class Measurer {
    private:
        Instance& inst;

    public:
        Measurer(Instance &instance);

        float evaluate(Solution& solution);
        bool validation(Solution& solution);
        bool validationRange(Solution& solution, int startIdx, int endIdx);
        float evaluateRange(Solution& solution, int startIdx, int endIdx);
        float evaluateLookahead(size_t day, float adi, int depth);
    };
}

#endif