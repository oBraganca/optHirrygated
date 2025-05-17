#include "CSVExporter.hpp"

bool CSVExporter::exportToCSV(const std::vector<DayResult>& results, const std::string& filename) {
    std::ofstream file("../output/"+filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    file << "Data,CAD,LC,Agua Inicial,ETC,Precipitacao,Irrigacao,Agua Final,Excesso\n";

    for (const auto& res : results) {
        file << res.data << ","
             << res.cad << ","
             << res.lc << ","
             << res.aguaInicial << ","
             << res.etc << ","
             << res.precipitacao << ","
             << res.irrigacao << ","
             << res.aguaFinal << ","
             << res.excesso << "\n";
    }

    file.close();
    std::cout<< "Data exported successfull" <<std::endl;
    return true;
}
