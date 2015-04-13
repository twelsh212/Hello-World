/*
 * MCS.cpp
 *
 *  Created on: Feb 5, 2010
 *      Author: rgreen
 */

#include "OMP_Sampler.h"
#include <iostream>
#include <iomanip>

using namespace std;

OMP_Sampler::OMP_Sampler() : Sampler() {
}

OMP_Sampler::~OMP_Sampler() {

}

void OMP_Sampler::run(MTRand& mt){
    double 	sumX,  sumXSquared, nSumX, nSumXSquared, localCurProb,
            curtailment, excess, totalCap, sigma, nSigma, sigmaEENS, stateProb;
    bool 	failed;
    std::string curSolution;
    std::vector<double> vCurSolution;
    std::vector<double> phevLoad, phevGen;
    Classifier* localClassifier;

     std::vector<Generator> 	localGens;
     std::vector<Line> 		    localLines;

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

    localGens  = gens;
    localLines = lines;

    double curProb, numSamples, stateGenerationTime=0;
    CStopWatch timer1;
    localClassifier = classifier;

    timer.startTimer();
    while(true){

        classifier->reset();
        curSolution = "";
        totalCap = 0.00;

        /******************************************** Generate States********************************************/
       
        #pragma omp parallel private(curProb, localClassifier, timer1, vCurSolution, curtailment, stateProb) shared(localLines, localGens) reduction(+:stateGenerationTime, numSamples, sumX, sumXSquared)
        {
            timer1.startTimer();
            stateProb = 1; genCount = 0;
            for(int x=0; x< localGens.size(); x++){           
                curProb = mt.rand();
                if(curProb <=  localGens[x].getOutageRate()){ vCurSolution[x] = 0;}
                else                                        { vCurSolution[x] = 1;}
            }

            if(useLines && localLines.size() > 0){
                for(int x=0; x<localLines.size(); x++){
                    curProb = mt.rand();
                    if(curProb <= (localLines[x].getOutageRate()*lineAdjustment)){
                        vCurSolution[x+gens.size()] = 0;
                    }else{
                        vCurSolution[x+gens.size()] = 1;
                    }
                }
            }

            vCurSolution[gens.size()+lines.size()] 	 = stateProb;
            vCurSolution[gens.size()+lines.size()+1] = totalCap;

            timer1.stopTimer();
            stateGenerationTime += timer1.getElapsedTime();

            /******************************************** Classify States********************************************/
       
            timer1.startTimer();
            curtailment = localClassifier->run(vCurSolution, excess);
            timer1.stopTimer();
            classificationTime += timer1.getElapsedTime();

            if(curtailment > 0){ 
                sumX++;	sumXSquared++;
                //sampledStates.insert(std::pair<std::string, int>(curSolution, 1));   
            }else{ 
                nSumX++; nSumXSquared++;
                //sampledStates.insert(std::pair<std::string, int>(curSolution, 0));
            }

            numSamples++; iterations++; 
        }
        pLOLP = LOLP;
        LOLP  = sumX/numSamples;
        vLOLP = (1/numSamples) * (sumXSquared/numSamples - pow(LOLP,2));
        sigma = sqrt(vLOLP)/LOLP;
        if(sigma < tolerance) { break;}
    }  
 
    avgLineCount /= numSamples;
    avgGenCount  /= numSamples;

    timer.stopTimer();
    simulationTime = timer.getElapsedTime();

    vCurSolution.clear();
    phevLoad.clear();
    phevGen.clear();
}
