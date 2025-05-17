#include "../include/SolutionProcessor.hpp"
#include "../include/CSVExporter.hpp"
#include "../include/Solution.hpp"
#include <string>

namespace opthirrygated {

std::vector<DayResult> SolutionProcessor::processSolution(const Instance &inst, const Solution &solution) {
    std::vector<DayResult> results;

    // Assume that the number of days is the size of the cicle vector.
    size_t numDays = inst.getCicle().size();

    // Retrieve the necessary vectors from the instance.
    const std::vector<float>& cad  = inst.getCad();  // Plant absorption capacity per day.
    const std::vector<float>& lc   = inst.getLc();   // Critical limit per day.
    const std::vector<float>& etc  = inst.getEtc();  // Evapotranspiration consumption.
    const std::vector<float>& prec = inst.getPrec(); // Precipitation.
    const std::vector<float>& lamp = inst.getLamp(); // Irrigation amount per percentage.
    const std::vector<float>& cicle = inst.getCicle();   // Day identifier (can be converted to string).

    float aguaInicial = cad[0];
    float aguaFinal = aguaInicial;

    for (size_t day = 0; day < numDays; day++) {
        DayResult dr;

        dr.data = std::to_string(cicle[day]);

        dr.cad = cad[day];
        dr.lc = lc[day];
        dr.aguaInicial = aguaInicial;
        dr.etc = etc[day];
        dr.precipitacao = prec[day];

        int perc = (day < solution.getSolution().size()) ? solution.getSolution()[day] : 8;

        float irrigation = (perc == 8) ? 0.0f : lamp[perc];
        dr.irrigacao = irrigation;

        aguaFinal = aguaInicial - etc[day] + prec[day] + irrigation;
        dr.aguaFinal = aguaFinal;

        dr.excesso = aguaFinal - lc[day];

        results.push_back(dr);

        aguaInicial = aguaFinal;
    }

    return results;
}

} // namespace opthirrygated
