#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>

using namespace std;
namespace opthirrygated {
    class Solution {
    private:
        vector<int> solution;

    public:
        void setSolution(const vector<int>& newSolution) { solution = newSolution; }
        vector<int> getSolution() const { return solution; }

    };
}


#endif