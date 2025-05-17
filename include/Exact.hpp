#ifndef EXACT_HPP_
#define EXACT_HPP_

enum status {SOLUTIONFOUND, INFEASIBLE};

#include <ilcplex/ilocplex.h>

#include "Instance.hpp"
#include "Solution.hpp"

typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVar3Matrix;
typedef IloArray<IloNumVar3Matrix> IloNumVar4Matrix;

typedef IloArray<IloNumArray> IloNumMatrix;
typedef IloArray<IloNumMatrix> IloNum3Matrix;

namespace opthirrygated {
    class Exact
    {
    private:

        Instance& p;
        IloConstraintArray constraints;
        IloEnv env;
        IloModel modelo;
        IloCplex pirr;	
        bool LBdefined;
        IloNumVarMatrix x;
        IloNumVarArray adf;
        IloNumVarArray adi;
        double timelimit;

        void addConstraint_Percentimetro(IloEnv& env, IloModel& modelo);
        void addConstraint_AguaInicial(IloEnv& env, IloModel& modelo);
        void addConstraint_AguaInicialRestante(IloEnv& env, IloModel& modelo);
        void addConstraint_AguaFinal(IloEnv& env, IloModel& modelo);
        void addConstraint_LimiteCritico(IloEnv& env, IloModel& modelo);
        

    public:
        Exact(Instance& _p, double _timelimit);

        ~Exact();

        void solve();
        status getStatus();
        void showSolution();
        double getFO();
        Solution getSolution();

    };
}

#endif // EXACT_HPP_
