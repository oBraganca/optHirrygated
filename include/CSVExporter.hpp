#ifndef CSVEXPORTER_HPP
#define CSVEXPORTER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct DayResult {
    std::string data;
    float cad;
    float lc;
    float aguaInicial;
    float etc;
    float precipitacao;
    float irrigacao;
    float aguaFinal;
    float excesso;
};

class CSVExporter {
public:
    CSVExporter() = default;

    bool exportToCSV(const std::vector<DayResult>& results, const std::string& filename);
};

#endif
