#include "../include/ConstructiveHeuristic.hpp"
#include "../include/Solution.hpp"
#include <float.h>
#include <unordered_map>

using namespace std;
using namespace opthirrygated;

ConstructiveHeuristic::ConstructiveHeuristic(opthirrygated::Instance &instance) : inst(instance) {}

Solution ConstructiveHeuristic::executeA() {
  Solution objSolution;
  vector<int> solution;

  float adi = inst.getCad()[0], adf = 0;

  size_t numDays = inst.getCicle().size();
  for (size_t day = 0; day < numDays; day++) {
    float bestPrice = FLT_MAX;
    int bestPerc = -1;

    for (int perc : inst.getPerc()) {
      float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[perc];
      float percCost = inst.getCost()[perc];

      if (percCost < bestPrice && auxAdf >= inst.getLc()[day]) {
        bestPrice = percCost;
        bestPerc = perc;
        adf = auxAdf;
      }
    }

    if (bestPerc != -1) {
      solution.push_back(bestPerc);
    } else {
      adf = adi;
    }

    adi = adf;
  }

  objSolution.setSolution(std::move(solution));
  return objSolution;
}

Solution ConstructiveHeuristic::executeB() {
    Solution objSolution;
    vector<int> solution;

  float adi = inst.getCad()[0], adf = 0;

  size_t numDays = inst.getCicle().size();
  for (size_t day = 0; day < numDays; day++) {
    float bestPrice = FLT_MAX;
    int bestPerc = 10;

  	float preAdf = adi - inst.getEtc()[day] + inst.getPrec()[day];

  	if (preAdf >= inst.getCad()[day]) {
      solution.push_back(bestPerc);
      adi = preAdf;
      continue;
    }
	bestPerc=-1;
	float auxAdf =0;
    for (int perc : inst.getPerc()) {
      auxAdf = preAdf + inst.getLamp()[perc];
      float percCost = inst.getCost()[perc];

      if (percCost < bestPrice && (auxAdf >= inst.getCad()[day] || auxAdf >= inst.getLc()[day])) {
        bestPrice = percCost;
        bestPerc = perc;
        adf = auxAdf;
      }
    }

    if(adf<inst.getLc()[day]){
      cout<<auxAdf << "ERROR"<<endl;
    }

    if (bestPerc != -1) {
      solution.push_back(bestPerc);
    } else {
      adf = adi;
    }

    adi = adf;
  }

  objSolution.setSolution(std::move(solution));
  return objSolution;
}



Solution ConstructiveHeuristic::executeC() {
 	Solution objSolution;
	vector<int> solution;

  	float adi = inst.getCad()[0], adf = 0;

  size_t numDays = inst.getCicle().size();
  for (size_t day = 0; day < numDays; day++) {
    float bestPrice = FLT_MAX;
    int bestPerc = 10;

	float preAdf = adi - inst.getEtc()[day] + inst.getPrec()[day];

	if (preAdf == inst.getCad()[day] || preAdf >= inst.getLc()[day]) {
    	solution.push_back(bestPerc);
    	adi = preAdf;
    	continue;
  	}

	bestPerc=-1;
    for (int perc : inst.getPerc()) {
      float auxAdf = preAdf + inst.getLamp()[perc];
      float percCost = inst.getCost()[perc];

      if (percCost <= bestPrice && auxAdf <= inst.getCad()[day] && auxAdf >= inst.getLc()[day]) {
        bestPrice = percCost;
        bestPerc = perc;
        adf = auxAdf;
      }
    }

	if(adf<inst.getLc()[day]){
		cout<<"ERROR"<<endl;
	}

    if (bestPerc != -1) {
      solution.push_back(bestPerc);
    } else {
      adf = adi;
    }

    adi = adf;
  }

  objSolution.setSolution(std::move(solution));
  return objSolution;
}