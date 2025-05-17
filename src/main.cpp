#include <iostream>
#include "../include/Instance.hpp"
#include "../include/Measurer.hpp"
#include "../include/Solution.hpp"
#include "../include/ConstructiveHeuristic.hpp"
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

    Solution solution;


    cout << endl;
    Measurer measurer(instance);
    cout << "---------------------------------------------------------------"<<endl;
    cout << "Solution Cost (Exato): R$" << measurer.evaluate(exactSol)<<endl << endl;
    cout << "---------------------------------------------------------------"<<endl;
    cout << "Heuristics Evaluation:"<<endl;

    ConstructiveHeuristic constructiveHeuristic(instance);
    solution = constructiveHeuristic.executeA();

    cout << "\tTotal day evaluated: " << instance.getCicle().size() <<endl;
    cout << "\tSolution Cost: R$" << measurer.evaluate(solution)<<endl;
    cout << "\tSolution Output: [ ";
    for (float val : solution.getSolution()) {
        cout << val << " ";
    }
    cout<<"]"<<endl;


	CSVExporter exporter;
	SolutionProcessor sp;
	exporter.exportToCSV(sp.processSolution(instance, solution), "result.csv");

    cout << "---------------------------------------------------------------"<<endl;
    return 0;
}
