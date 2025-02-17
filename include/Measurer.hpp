#ifndef MEASURER_HPP
#define MEASURER_HPP
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Instance.hpp"
#include "Solution.hpp"

using namespace std;
namespace opthirrygated{
    class Measurer{
    private:
        Instance& inst;
    public:
        Measurer(Instance &instance);
        virtual float evaluate(Solution& solution);
    };
}

#endif