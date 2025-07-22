#include "../include/RefinementHeuristic.hpp"
#include "../include/Measurer.hpp"
#include <float.h>
#include <iomanip>
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

bool Measurer::validation(Solution& solution) {
    float adi = inst.getCad()[0], adf = 0;

    size_t numDays = solution.getSolution().size();
    for (size_t day = 0; day < numDays; day++) {
        float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[solution.getSolution()[day]];
        float e = 0.0001;
        if (auxAdf + e < inst.getLc()[day]) {
            cout << endl << "A planta está com a hidratação inferior o limite critico no dia: " << day << endl;
            printf("%f", auxAdf+e);
            cout << "Limite critico: " << setprecision(4) <<(inst.getLc()[day]) << endl;
            cout << "Precipitação : " << inst.getPrec()[day] << endl;
            cout << "Agua disponivel no fim do dia: "<< auxAdf << endl;

            return 0;
        }

        adi = auxAdf;
    }
    return 1;
}

// Nova função de validação por range
bool Measurer::validationRange(Solution& solution, int startIdx, int endIdx) {
    const auto& solutionVec = solution.getSolution();
    auto adfSolutions = solution.getAdfSolutions();

    // Verificar se os índices são válidos
    if (startIdx < 0 || endIdx >= solutionVec.size() || startIdx > endIdx) {
        return false;
    }

    float adi, adf;

    if (startIdx == 0) {
        adi = inst.getCad()[0]; // Primeiro dia usa capacidade inicial
    } else {
        adi = adfSolutions[startIdx - 1];
    }

    for (int day = startIdx; day <= endIdx; day++) {
        float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[solutionVec[day]];
        float e = 0.0001;

        if (auxAdf + e < inst.getLc()[day]) {
            return false;
        }

        adfSolutions[day] = auxAdf;

        adi = auxAdf;
    }

    solution.setAdfSolution(adfSolutions);

    return true;
}
float Measurer::evaluateRange(Solution& solution, int startIdx, int endIdx) {
    float costSolution = 0.0;
    const auto& costVec = inst.getCost();
    const auto& solutionVec = solution.getSolution();

    // Verificar se os índices são válidos
    if (startIdx < 0 || endIdx >= solutionVec.size() || startIdx > endIdx) {
        std::cerr << "Warning: Invalid range [" << startIdx << ", " << endIdx << "] for solution of size " << solutionVec.size() << std::endl;
        return 0.0;
    }

    // Calcular o custo apenas para o range especificado
    for (int i = startIdx; i <= endIdx; i++) {
        int sol = solutionVec[i];
        if (sol >= 0 && sol < costVec.size()) {
            costSolution += costVec[sol];
        } else {
            std::cerr << "Warning: Index " << sol << " out of bounds in getCost() at position " << i << std::endl;
        }
    }

    return costSolution;
}

