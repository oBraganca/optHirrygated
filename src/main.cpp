#include <iostream>
#include "../include/Instance.hpp"
#include "../include/Measurer.hpp"
#include "../include/Solution.hpp"
#include "../include/ConstructiveHeuristic.hpp"

using namespace std;
using namespace opthirrygated;
int main() {
    // Create an Instance object
    Instance instance("../datasource/planilha.xlsx");

    // Access and print member variables using getters
    cout << "Perc IDs: ";
    for (int val : instance.getPerc()) {
        cout << val << " ";
    }
    cout<<endl;
    // Access and print member variables using getters
    cout << "Lamp: ";
    for (float val : instance.getLamp()) {
        cout << val << " ";
    }
    cout << endl;
    // Access and print member variables using getters
    cout << "Perc Cost: ";
    for (float val : instance.getCost()) {
        cout << val << " ";
    }
    cout << endl;


    vector<int> v = {3,1,0,0,8,3,1,0};

    Solution solution;
    solution.setSolution(v);

    cout << "Solution Output: [ ";
    for (float val : solution.getSolution()) {
        cout << val << " ";
    }
    cout<<"]";

    cout << endl;
    Measurer measurer(instance);
    cout << "Solution Cost: R$" << measurer.evaluate(solution)<<endl << endl;
    cout << "---------------------------------------------------------------"<<endl;
    cout << "Heuristics Evaluation:"<<endl;

    ConstructiveHeuristic constructiveHeuristic(instance);
    solution = constructiveHeuristic.execute();

    cout << "\tTotal day evaluated: " << instance.getCicle().size() <<endl;
    cout << "\tSolution Cost: R$" << measurer.evaluate(solution)<<endl;
    cout << "\tSolution Output: [ ";
    for (float val : solution.getSolution()) {
        cout << val << " ";
    }
    cout<<"]"<<endl;

    cout << "---------------------------------------------------------------"<<endl;



    // You can add further tests here to see if the methods are working properly.
    return 0;
}
