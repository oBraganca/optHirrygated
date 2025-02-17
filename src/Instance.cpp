#include "../include/Instance.hpp"
#include <OpenXLSX.hpp>

using namespace OpenXLSX;
using namespace std;
using namespace opthirrygated;

Instance::Instance(string path) {
    setPath(path);
    Instance::exec();
}


template <typename T>
vector<T> loadColumn(XLWorksheet& wks, const string& col, int startCell, int endCell) {
    vector<T> values;
    for (int i = startCell; i <= endCell; i++) {
        string cellRef = col + to_string(i);
        values.push_back(wks.cell(XLCellReference(cellRef)).value().get<T>());
    }
    return values;
}

void Instance::exec() {

    try {
        XLDocument doc;
        doc.open(getPath());
        auto wks = doc.workbook().worksheet("ciclo");
        setCicle(loadColumn<int>(wks, "A", 2, 121));
        setPrec(loadColumn<float>(wks, "B", 2, 121));
        setEtc(loadColumn<float>(wks, "C", 2, 121));
        setCad(loadColumn<float>(wks, "D", 2, 121));
        setLc(loadColumn<float>(wks, "E", 2, 121));

        wks = doc.workbook().worksheet("perc");
        setPerc(loadColumn<int>(wks, "E", 3, 11));
        setCost(loadColumn<float>(wks, "D",3 , 11));
        setLamp(loadColumn<float>(wks, "C", 3, 11));



    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

}

