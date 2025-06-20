#include "../include/ConstructiveHeuristic.hpp"
#include "../include/Solution.hpp"
#include "../include/Measurer.hpp"
#include <float.h>
#include <unordered_map>

using namespace std;
using namespace opthirrygated;

ConstructiveHeuristic::ConstructiveHeuristic(opthirrygated::Instance &instance) : inst(instance) {}

Solution ConstructiveHeuristic::executeA() {
  Solution objSolution;
  vector<int> solution;
  vector<float> adfSol;

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
    adfSol.push_back(adf);
  }

  objSolution.setSolution(std::move(solution));
  objSolution.setAdfSolution(std::move(adfSol));
  return objSolution;
}

Solution ConstructiveHeuristic::executeB() {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSolution;

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
    adfSolution.push_back(adf);
  }

  objSolution.setSolution(std::move(solution));
  objSolution.setAdfSolution(std::move(adfSolution));
  return objSolution;
}



Solution ConstructiveHeuristic::executeC() {
 	Solution objSolution;
	vector<int> solution;
    vector<float> adfSol;

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
    adfSol.push_back(adf);
  }

  objSolution.setSolution(std::move(solution));
  objSolution.setAdfSolution(std::move(adfSol));
  return objSolution;
}
Solution ConstructiveHeuristic::executeLookAhead(int lookaheadDepth) {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSol;
    Measurer measurer(inst);

    float adi = inst.getCad()[0], adf = 0;
    size_t numDays = inst.getCicle().size();

    for (size_t day = 0; day < numDays; day++) {
        float bestScore = FLT_MAX;
        int bestPerc = -1;
        float bestAdf = adi;

        for (int perc : inst.getPerc()) {
            float currAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[perc];

            if (currAdf >= inst.getLc()[day]) {
                float currCost = inst.getCost()[perc];
                float futureCost = measurer.evaluateLookahead(day + 1, currAdf, lookaheadDepth - 1);
                float totalScore = currCost + futureCost;

                if (totalScore < bestScore) {
                    bestScore = totalScore;
                    bestPerc = perc;
                    bestAdf = currAdf;
                }
            }
        }

        if (bestPerc != -1) {
            solution.push_back(bestPerc);
            adf = bestAdf;
        } else {
            adf = adi; // fallback
        }

        adi = adf;
        adfSol.push_back(adf);
    }

    objSolution.setSolution(std::move(solution));
    objSolution.setAdfSolution(std::move(adfSol));

    return objSolution;
}

