#include "../include/Measurer.hpp"
using namespace opthirrygated;

Measurer::Measurer(Instance &instance) : inst(instance) {
}

float Measurer::evaluate(Solution& solution) {
    float costSolution = 0.0;
    const auto& costVec = inst.getCost();
std::vector<int> ids = {
    10, 10, 10,  1, 10, 10, 10,  0,  1,  1,
     0,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  0,  1,  1,  1,  0,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1, 10,  1,  0, 10,  1,
    10, 10, 10, 10,  1, 10, 10,  1, 10,  0,
     0,  0, 10, 10, 10, 10,  1, 10,  1,  1,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10
};

    for (int sol : solution.getSolution()) {
	    if (sol >= 0 && sol < costVec.size()) {
            costSolution += costVec[sol];
        } else {
            std::cerr << "Warning: Index " << sol << " out of bounds in getCost()" << std::endl;
        }
    }
    return costSolution;
}
