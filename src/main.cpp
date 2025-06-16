#include <iostream>
#include "../include/Instance.hpp"
#include "../include/Measurer.hpp"
#include "../include/Solution.hpp"
#include "../include/ConstructiveHeuristic.hpp"
#include "../include/RefinementHeuristic.hpp"
#include "../include/Exact.hpp"
#include "../include/SolutionProcessor.hpp"
#include "../include/CSVExporter.hpp"

using namespace std;
using namespace opthirrygated;
int main() {
    // Create an Instance object
    Instance instance("../datasource/planilha.xlsx");


    //Running exact method

    Solution exactSol;
    Exact exato(instance, 3600);
    exato.solve();
    if(exato.getStatus() == status::SOLUTIONFOUND)
    {
        cout << "Solucao encontrada!"<<endl;
        cout << "FO: " << exato.getFO() << endl;
        exato.showSolution();
        exactSol = exato.getSolution();
    }
    else
    {
        cout << "Verificar!!!" <<endl;
    }


    //CSVExporter exporter;
    //SolutionProcessor sp;
    //exporter.exportToCSV(sp.processSolution(instance, exato.getSolution()), "result.csv");


    Solution solution;
    cout << endl;
    Measurer measurer(instance);
    cout << "---------------------------------------------------------------"<<endl;
    cout << "Solution Cost (Exato): R$" << measurer.evaluate(exactSol)<<endl << endl;
    cout << "---------------------------------------------------------------"<<endl;
    cout << "Heuristics Evaluation:"<<endl;

    cout << "Caixa Preta Solution Output: [ ";
    for (float val : exactSol.getSolution()) {
        cout << val << " ";
    }
    cout<<"]"<<endl;


    cout << "---------------------------------------------------------------"<<endl;

    ConstructiveHeuristic constructiveHeuristic(instance);
    solution = constructiveHeuristic.executeLookAhead(4);


    RefinementHeuristic refinementHeuristic(instance);
    //solution = refinementHeuristic.executeB(solution);
    solution = refinementHeuristic.executeSA(solution);
    cout << measurer.validation(solution);

    cout << "Total day evaluated: " << instance.getCicle().size() <<endl;
    cout << "Solution Cost: R$" << measurer.evaluate(solution)<<endl;
    cout << "Solution Output: [ ";
    for (float val : solution.getSolution()) {
        cout << val << " ";
    }
    cout<<"]"<<endl;

    cout << "---------------------------------------------------------------"<<endl;


    /*
    CSVExporter exporter;
    SolutionProcessor sp;
    exporter.exportToCSV(sp.processSolution(instance, exactSol), "result.csv");

    Measurer measurer(instance);
    std::vector<int> validationSolution = {
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

    solution.setSolution(validationSolution);

    cout << "Validating the solution exact "<< endl;
    if (measurer.validation(exactSol)) {
        cout << "Valid solution" <<endl;
    }else {
        cout << "Solution not valid" <<endl;
    }
    */

    return 0;
}
