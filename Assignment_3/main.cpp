/*
 * main.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: Rob
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <math.h>
#include <string>
#include <vector>

// Utils
#include "anyoption.h"
#include "Utils.h"

// State Space Samplers
#include "MCS_Sampler.h"
#include "Sampler.h"

// State Classifiers
#include "OPFClassifier.h"
#include "CapacityClassifier.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv){
    vector < Generator > 	  gens;
    vector < Line >			  lines;
    vector < Bus  >			  buses;
    vector < vector < int > > lineOutages, genOutages;
    map    < string, double > failedStates,successStates;
    char* aTime;
    char* aTime2;
    AnyOption *opt = new AnyOption();
    CStopWatch timer;

    int samplingPopSize = 25, samplingGenSize = 1000, trials = 1;
    int run 	      = 0,    
        numBuses      = 0,
        numLines      = 0,          
        numSamples	  = 1000, 
        numThreads	  = 1,
        batchSize     = 1000;
    bool        useLines = true;
    double		eLOLP, pruningProb = 0, pruningTime = 0, pLoad	= 28.50, qLoad = 5.80, lineAdj = 1.0;
    double		totalTime, sigma = 0.025;
    double 		stoppingValue = 0;
    MTRand 		mt;

    Sampler*	sampler = NULL;
    Classifier* classifier = NULL;
    pruning::SAMPLING_METHOD 		samplingMethod		 = pruning::SM_MCS;
    pruning::CLASSIFICATION_METHOD 	classificationMethod = pruning::CM_OPF;

    string 	curSystem 	= "RTS79",
            tString;

    /************************************ Begin Command Line Options ************************************/
    setUsage(opt); setOptions(opt);
    opt->processCommandArgs(argc, argv);

    if(opt->getValue("useLines") 	!= NULL) { useLines 			 = true;}    
    if(opt->getValue("system")      != NULL) { curSystem	         = Utils::toUpper(opt->getValue("system"));}
    if(opt->getValue("classifier")  != NULL) { classificationMethod  = Utils::getClassificationMethod(opt->getValue("classifier")); }
    if(opt->getValue("sampler") 	!= NULL) { samplingMethod        = Utils::getSamplingMethod(opt->getValue("sampler"));	}
    if(opt->getValue("sigma")	    != NULL) { sigma                 = atof(opt->getValue("sigma"));}
    if(opt->getValue("numSamples")	!= NULL) { numSamples		     = atoi(opt->getValue("numSamples"));}
    /************************************ End Command Line Options ************************************/

    Utils::loadSystemData(pLoad, qLoad, numBuses, numLines, curSystem, gens, lines, buses);
    if(classificationMethod == pruning::CM_OPF)     { classifier = new OPFClassifier(curSystem, numBuses, gens, buses, lines);}
    else if(classificationMethod == pruning::CM_CAP){ classifier = new CapacityClassifier(curSystem, numBuses, gens, buses, lines, pLoad);}
    classifier->setUseLines(useLines);

    /************************************ Begin File Setup ************************************/
    aTime = new char[20];
    Utils::getTimeStamp(aTime);
    
    /************************************ End File Setup ************************************/
    
    timer.startTimer();

    // Reset!
    failedStates.clear(); successStates.clear();
    classifier->init();
    classifier->resetTimes();
    pruningProb	= 0;

    omp_set_num_threads(2);

    if(sampler == NULL){ sampler = new MCS_Sampler(); }
    sampler->setCurrentSystem(curSystem);
    sampler->Init(failedStates, successStates, gens, lines, pLoad, classifier, numBuses, 0, 0, 0);
    sampler->setUseLines(useLines);
    sampler->setLineAdjustment(lineAdj);
    sampler->setTolerance(sigma);
    sampler->setUseLogging(false);				
    sampler->run(mt);

    timer.stopTimer();
    totalTime = timer.getElapsedTime();

    //Calculate LOLP
    eLOLP = sampler->getLOLP();
    /************************************ End File Output ************************************/

    cout << "Number of Threads: " << omp_get_num_threads() << endl;
    cout << fixed << setprecision(4) << left << eLOLP;
    return 0;
}

