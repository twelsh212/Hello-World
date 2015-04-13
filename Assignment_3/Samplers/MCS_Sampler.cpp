/*
 * MCS.cpp
 *
 *  Created on: Feb 5, 2010
 *      Author: rgreen
 */

#include "MCS_Sampler.h"
#include <iostream>
#include <iomanip>

using namespace std;

MCS_Sampler::MCS_Sampler() : Sampler() {
}

MCS_Sampler::~MCS_Sampler() {

}

void MCS_Sampler::run(MTRand& mt){
    double 	sumX,  sumXSquared, nSumX, nSumXSquared,
            curtailment, excess, totalCap, sigma, nSigma, sigmaEENS, stateProb;
    bool 	failed;
    std::string curSolution;
    std::vector<double> vCurSolution;
    std::vector<double> phevLoad, phevGen;

    curSolution = "";
    failed = false;
    vCurSolution.resize(gens.size() + lines.size() + 2, 0);
    sumX=0; sumXSquared=0; nSumX=0; nSumXSquared=0;
    curProb 	 = 0.0; 
    numSamples 	 = 1; 	iterations 	 = 0;	sigma = 1.0;
    nSigma       = 1.0;	
    Collisions	 = 0;	avgLineCount = 0;	avgGenCount  = 0;
    stateGenerationTime = searchTime = 0;

    vLOLP = 0; pLOLP  = 0; LOLP   = 0;
    NLOLP = 0; vNLOLP = 0; pNLOLP = 0;
    EENS  = 0; pEENS  = 0; vEENS  = 0;
    
    lolps.clear(); sigmas.clear(); vLolps.clear();
    nLolps.clear(); nSigmas.clear();

    srand((unsigned)time(NULL));

    timer.startTimer();
    while(true){

        classifier->reset();
        curSolution = "";
        totalCap = 0.00;

        /******************************************** Generate States********************************************/
        timer1.startTimer();
        stateProb = 1; genCount = 0;
        for(unsigned int x=0; x< gens.size(); x++){           
            curProb = mt.rand();
            if(curProb <=  gens[x].getOutageRate()){
                vCurSolution[x] = 0;
                stateProb 	   *= gens[x].getOutageRate();
            }else{
                vCurSolution[x]  = 1;
                stateProb 		*= (1-gens[x].getOutageRate());
                totalCap 		+= gens[x].getPG()/100;
            }
        }

        if(useLines && lines.size() > 0){
            for(unsigned int x=0; x<lines.size(); x++){
                curProb = mt.rand();
                if(curProb <= (lines[x].getOutageRate()*lineAdjustment)){
                    vCurSolution[x+gens.size()] = 0;
                    stateProb *= (lines[x].getOutageRate()*lineAdjustment);
                }else{
                    vCurSolution[x+gens.size()] = 1;
                    stateProb *= (1-lines[x].getOutageRate()*lineAdjustment);
                }
            }
        }
        sampledStateProbs[curSolution] = stateProb;

        vCurSolution[gens.size()+lines.size()] 	 = stateProb;
        vCurSolution[gens.size()+lines.size()+1] = totalCap;

        timer1.stopTimer();
        stateGenerationTime += timer1.getElapsedTime();

        /******************************************** Classify States********************************************/
       
        timer1.startTimer();
        curtailment = classifier->run(vCurSolution, excess);
        timer1.stopTimer();
        classificationTime += timer1.getElapsedTime();

        if(curtailment > 0){ 
            sumX++;	sumXSquared++;
            sampledStates.insert(std::pair<std::string, int>(curSolution, 1));   
        }else{ 
            nSumX++; nSumXSquared++;
            sampledStates.insert(std::pair<std::string, int>(curSolution, 0));
        }

        numSamples++; iterations++;

        pLOLP = LOLP;
        LOLP  = sumX/numSamples;
        vLOLP = (1/numSamples) * (sumXSquared/numSamples - pow(LOLP,2));
        sigma = sqrt(vLOLP)/LOLP;

        pNLOLP = NLOLP;
        NLOLP  = nSumX/numSamples;
        vNLOLP = (1/numSamples) * (nSumXSquared/numSamples - pow(NLOLP,2));
        nSigma = sqrt(vNLOLP)/NLOLP;
        
        vLolps.push_back(vLOLP);
        lolps .push_back(LOLP);
        sigmas.push_back(sigma);

        nLolps .push_back(NLOLP);
        nSigmas.push_back(nSigma);

        if(sigma < tolerance) { break;}
    }  
 
    avgLineCount /= numSamples;
    avgGenCount  /= numSamples;

    timer.stopTimer();
    simulationTime = timer.getElapsedTime();

    vCurSolution.clear();
    phevLoad.clear();
    phevGen.clear();

    /*for(p = localSuccessStates.begin(); p != localSuccessStates.end(); p++){
        cout << p->second << endl;
    }
    */
}
