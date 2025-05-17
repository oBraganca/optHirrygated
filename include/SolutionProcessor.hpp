#ifndef SOLUTIONPROCESSOR_HPP
#define SOLUTIONPROCESSOR_HPP

#include <vector>
#include <string>
#include "Instance.hpp"
#include "CSVExporter.hpp"
#include "Solution.hpp"

namespace opthirrygated {

    class SolutionProcessor {
    public:
        static std::vector<DayResult> processSolution(const Instance &inst, const Solution &solution);
    };

}

#endif
