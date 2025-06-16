#ifndef EXACT_CPP_
#define EXACT_CPP_

#include "Exact.hpp"

using namespace opthirrygated;

void Exact::addConstraint_Percentimetro(IloEnv& env, IloModel& modelo)
{

	for (unsigned int c = 0; c < p.getCicle().size(); c++) // para cada dia do Ciclo
	{

		//criando restrição
		IloExpr temp(env);

		for (unsigned int perc = 0; perc < p.getPerc().size(); perc++) // a soma das escolhas dos percentimetros usados para o dia c
		{
			temp += x[perc][c];

		}

		IloConstraint constr = (temp) == 1; // deve ser igual a 1
		stringstream name;
		name << "PERC dia(" << c << ") : ";

		constr.setName(name.str().c_str());
		modelo.add(constr);
		constraints.add(constr);

	}

}

void Exact::addConstraint_AguaInicial(IloEnv& env, IloModel& modelo)
{
	//a quantidade de água disponível no primeiro dia é igual a CAD do primeiro dia
	IloExpr temp(env);

	temp = adi[0] - p.getCad().at(0);

	IloConstraint constr = (temp) == 0;
	stringstream name;
	name << "Dia0: ";
	constr.setName(name.str().c_str());
	modelo.add(constr);
	constraints.add(constr);

}


void Exact::addConstraint_AguaInicialRestante(IloEnv& env, IloModel& modelo)
{
	for(unsigned int c = 1; c < p.getCicle().size(); c++) // para todos os dias do ciclo, exceto o primeiro (0)
	{
		IloExpr temp(env);

		temp = adi[c] - adf[c-1];

		IloConstraint constr = temp == 0;

		stringstream name;
		name << "Dia(c): ";
		constr.setName(name.str().c_str());
		modelo.add(constr);
		constraints.add(constr);

	}
}


void Exact::addConstraint_AguaFinal(IloEnv& env, IloModel& modelo)
{
	//para cada dia do ciclo
	for(unsigned int c = 0 ; c < p.getCicle().size(); c++)
	{
		IloExpr temp(env);

		//pegando o valor da lamina dágua utilizada no dia c para somar
		IloExpr lamina(env);
		for(unsigned int perc = 0; perc < p.getPerc().size(); perc++)
		{
			lamina += p.getLamp().at(perc) * x[perc][c];
		}

		temp = adf[c] - adi[c] + p.getEtc().at(c) - p.getPrec().at(c) - lamina;
	
		IloConstraint constr = (temp) == 0;
		stringstream name;
		name << "AguaF(" << c << ") : ";
		constr.setName(name.str().c_str());
		modelo.add(constr);
		constraints.add(constr);
		
	}

}


void Exact::addConstraint_LimiteCritico(IloEnv& env, IloModel& modelo)
{
	//para cada dia do ciclo
	for(unsigned int c = 0 ; c < p.getCicle().size(); c++)
	{
		IloExpr temp(env);		

		temp = adf[c] - p.getLc().at(c);
	
		IloConstraint constr = (temp) >= 0;
		stringstream name;
		name << "Lc(" << c << ") : ";
		constr.setName(name.str().c_str());
		modelo.add(constr);
		constraints.add(constr);
		
	}

}

Exact::Exact(Instance& _p, double _timelimit) :
		p(_p), timelimit(_timelimit)
{
	modelo = IloModel(env);
	constraints = IloConstraintArray(env);
	LBdefined = false;

	//definindo a variavel x_pc
	//  - binária, representa se o percentímetro p foi utilizado no dia c do Ciclo

	x = IloNumVarMatrix(env, p.getPerc().size());

	for (unsigned int perc = 0; perc < p.getPerc().size(); perc++)
	{
		x[perc] = IloNumVarArray(env, p.getCicle().size(), 0, 1, ILOINT);	
		for(unsigned int c = 0; c < p.getCicle().size(); c++)
		{
			stringstream var;
			var << "x[" << perc <<","<<c<<"]";

			x[perc][c].setName(var.str().c_str());

			modelo.add(x[perc][c]);

		}
	}

	//definindo variavel adf_c
	//  - quantidade de água disponível no solo no final do dia c do Ciclo
	adf = IloNumVarArray(env, p.getCicle().size(), 0, IloInfinity, ILOFLOAT);

	for(unsigned int c = 0; c < p.getCicle().size(); c++)
	{
		stringstream var;
		var << "adf["<<c<<"]";
		adf[c].setName(var.str().c_str());
		modelo.add(adf[c]);
	}

	//definindo variavel adi_c
	//  - quantidade de água diponível no solo no início do dia c do Ciclo
	adi = IloNumVarArray(env, p.getCicle().size(), 0, IloInfinity, ILOFLOAT);

	for(unsigned int c = 0; c < p.getCicle().size(); c++)
	{
		stringstream var;
		var << "adi["<<c<<"]";
		adi[c].setName(var.str().c_str());
		modelo.add(adi[c]);
	}


	// função objetivo
	IloExpr FO(env);

	for (int perc = 0; perc < p.getPerc().size(); perc++)
	{
		for (int c = 0; c < p.getCicle().size(); c++)
		{
			FO += p.getCost().at(perc)*x[perc][c];
		}
	}

	//adicionando a fo ao modelo
	modelo.add(IloMinimize(env, FO));

	addConstraint_Percentimetro(env, modelo);
	addConstraint_AguaInicial(env, modelo);
	addConstraint_AguaInicialRestante(env, modelo);
	addConstraint_AguaFinal(env, modelo);
	addConstraint_LimiteCritico(env, modelo);
	
}

Exact::~Exact()
{
	pirr.end();
}
/*
Solution Exact::getSolution()
{
	Solution s = vector<int>(p.N);
	for (unsigned int i = 0; i < p.N; i++)
	{
		for (unsigned int k = 0; k < p.N; k++)
		{
			if (mla.getValue(x[i][k]) > 0.9999)
			{
				s.at(i) = k+1;
			}
		}
	}

	return s;
}

*/
void Exact::solve()
{

	// Gera LP
	pirr = IloCplex(modelo);
	pirr.exportModel("lp.lp");

	pirr.setParam(IloCplex::Param::MIP::Display, 0);
	pirr.setParam(IloCplex::Param::Simplex::Display, 0);
	pirr.setParam(IloCplex::Param::Barrier::Display, 0);
	pirr.setParam(IloCplex::Param::Network::Display, 0);

	pirr.setParam(IloCplex::Param::TimeLimit, timelimit);
	//resolvendo o modelo
	pirr.solve();

}


void Exact::showSolution()
{
    cout << "Solution Output: [ ";
    for(unsigned int c = 0; c < p.getCicle().size(); c++)
    {
        for(unsigned int perc = 0; perc < p.getPerc().size(); perc++)
        {
            if( pirr.getValue(x[perc][c] == 1) )
            {
                cout << perc << " ";
            }
        }
    }
    cout << "]" << endl;

}

Solution Exact::getSolution()
{
    Solution sol;
    vector<int> v;
    for(unsigned int c = 0; c < p.getCicle().size(); c++)
    {
        for(unsigned int perc = 0; perc < p.getPerc().size(); perc++)
        {
            if( pirr.getValue(x[perc][c] >= 0.5) )
            {
                v.push_back(perc);
            }
        }
    }

    sol.setSolution(v);

    return sol;
}

double Exact::getFO()
{
    if(getStatus() == SOLUTIONFOUND)
    {
        return pirr.getObjValue();
    }
    else
    {
        return -1;
    }

}

status Exact::getStatus()
{
	if(pirr.getStatus() == IloAlgorithm::Infeasible)
	{
		return INFEASIBLE;
	}
	else
	{
		return SOLUTIONFOUND;
	}
}

void Exact::showVars()
{
	for(unsigned int c=0; c< p.getCicle().size(); c++)
	{
		printf("D[%d] - Ai: %.2f Af: %.2f Lc: %.2f\n",c,  pirr.getValue(adi[c]), pirr.getValue(adf[c]), p.getLc().at(c));
	}

}



#endif //EXACT_CPP_
