#include "../include/Measurer.hpp"
using namespace opthirrygated;

Measurer::Measurer(Instance &instance) : inst(instance) {
}

float Measurer::evaluate(Solution& solution) {
    float costSolution = 0.0;
    const auto& costVec = inst.getCost();  // Get reference to cost vector

    for (int sol : solution.getSolution()) {
        if (sol >= 0 && sol < costVec.size()) {  // Bounds check
            costSolution += costVec[sol];
        } else {
            std::cerr << "Warning: Index " << sol << " out of bounds in getCost()" << std::endl;
        }
    }
    return costSolution;
}
