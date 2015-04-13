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
#include "CFO_Sampler.h"
#include "DS_Sampler.h"
#include "LDS_Sampler.h"
#include "LHS_Sampler.h"
#include "MCS_Sampler.h"
#include "MO_PSO_Sampler.h"
#include "PSO_Sampler.h"
#include "Sampler.h"

// State Classifiers
#include "OPFClassifier.h"
#include "SVMClassifier.h"
#include "CapacityClassifier.h"

// State Space Pruners
#include "ACOPruner.h"
#include "AISPruner.h"
#include "CFOPruner.h"
#include "GAPruner.h"
#include "ModifiedGAPruner.h"
#include "PSOPruner.h"
#include "RPSOPruner.h"
#include "MO_PSOPruner.h"
#include "MO_PSOPruner_Pareto.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv){
    Pruner 	 *p  = NULL;
    MOPruner *mp = NULL;
    vector < Generator > 	  gens;
    vector < Line >			  lines;
    vector < Bus  >			  buses;
    vector < vector < int > > lineOutages, genOutages;
    map    < string, double > failedStates,successStates;
    char* aTime;
    char* aTime2;
    AnyOption *opt = new AnyOption();
    CStopWatch timer;

    int pruningPopSize 	= 25, pruningPopMax   = 25,  pruningPopStep = 5,
        pruningGenSize 	= 25, pruningGenMax   = 25,  pruningGenStep = 5;
    int samplingPopSize = 25, samplingGenSize = 1000, trials = 1;
    int run 	      = 0,    numBuses        = 0,
        numLines      = 0,    totalVehicles   = 1539000,
        numSamples	  = 1000, numThreads	  = 1,
        batchSize     = 1000;

    double		eLOLP, pruningProb = 0, pruningTime = 0, pLoad	= 28.50, qLoad = 5.80, lineAdj = 1.0;
    double 		phevPenetration = 0.05, phevRho = 0.8;
    double		totalTime, sigma = 0.025;
    double 		stoppingValue = 0;
    bool		useLines 		= false, useMoPruner 		= false,
                usePHEVs 	    = false,
                saveGenOutages 	= false, saveLineOutages 	= false,
                logConvergence 	= false, negateFitness 		= false,
                useParetoMO 	= false, useWeightedMO		= false,
                useLocalSearch  = false, permuteSolutions   = false;
    MTRand 		mt;

    Sampler*	sampler = NULL;
    Classifier* classifier = NULL;

    pruning::STOPPING_METHOD 		stoppingMethod 		 = pruning::SM_GENERATION;
    pruning::PHEV_PLACEMENT  		phevPlacement   	 = pruning::PP_EVEN_ALL_BUSES;
    pruning::PRUNING_METHOD  		pruningMethod  		 = pruning::PM_NONE;
    pruning::SAMPLING_METHOD 		samplingMethod		 = pruning::SM_MCS;
    pruning::CLASSIFICATION_METHOD 	classificationMethod = pruning::CM_OPF;
    pruning::PRUNING_OBJECTIVE      pruningObj           = pruning::PO_PROBABILITY;

    string 	fName 	  	= "",
            logFileName = "",
            curSystem 	= "RTS79",
            tString;
    ofstream myFile;

    /************************************ Begin Command Line Options ************************************/
    setUsage(opt); setOptions(opt);
    opt->processCommandArgs(argc, argv);

    if(opt->getFlag( "help" ) || opt->getFlag( 'h' ) ){
        opt->printUsage();	exit(0);
    }
    if(opt->getValue("useLines") 		!= NULL){ useLines 			= true;}
    if(opt->getValue("useMOPruner")     != NULL){ useMoPruner       = true;}
    if(opt->getValue("useLocalSearch")	!= NULL){ useLocalSearch	= true;}
    if(opt->getValue("usePHEVs")		!= NULL){ usePHEVs 			= true;}
    if(opt->getValue("saveGenOutages")  != NULL){ saveGenOutages 	= true;}
    if(opt->getValue("saveLineOutages") != NULL){ saveLineOutages 	= true;}
    if(opt->getValue("logConvergence")  != NULL){ logConvergence	= true;}
    if(opt->getValue("negateFitness")   != NULL){ negateFitness	= true;}
    if(opt->getValue("useParetoMO")   	!= NULL){ useParetoMO		= true;}
    if(opt->getValue("useWeightedMO")   != NULL){ useWeightedMO		= true;}
    if(opt->getValue("permute")         != NULL){ permuteSolutions  = true;}
     
    // Parallel
    if(opt->getValue("numThreads")  != NULL){
        numThreads = atoi(opt->getValue("numThreads"));
        if(numThreads > omp_get_max_threads()-1){numThreads = omp_get_max_threads()-1; }
        omp_set_num_threads(numThreads);
    }

    if(opt->getValue("batchSize")  != NULL){
        batchSize = atoi(opt->getValue("batchSize"));
    }
    // End Parallel
    
    if(useParetoMO && useWeightedMO){
        cout << "Invalid Combination of MO-Pruners (Can't use pareto and weighted together)!\n";
        exit(1);
    }

    if(usePHEVs){
        if(opt->getValue("penetration")  != NULL){
            phevPenetration	= atof(opt->getValue("penetration"));
            if(phevPenetration < 0 || phevPenetration > 1){
                phevPenetration = 0.05;
            }
        }
        if(opt->getValue("rho")  != NULL){
            phevRho	= atof(opt->getValue("rho"));
            if(phevRho < 0 || phevRho > 1){
                phevRho = 0.8;
            }
        }
        if(opt->getValue("totalVehicles")  != NULL){
            totalVehicles	= atoi(opt->getValue("totalVehicles"));
            if(totalVehicles < 0){
                totalVehicles = 1539000;
            }
        }
        if(opt->getValue("placement")  != NULL){
            phevPlacement = Utils::getPHEVPlacement(opt->getValue("placement"));
        }else{
            phevPlacement = pruning::PP_EVEN_ALL_BUSES;
        }
    }else{
        phevPenetration = 0.00;
        phevRho 	  	= 0.00;
        totalVehicles 	= 0;
        phevPlacement 	= pruning::PP_NONE;
    }
    if(opt->getValue("system")  != NULL){
        curSystem	= Utils::toUpper(opt->getValue("system"));
    }
    if(opt->getValue("lineAdj") != NULL){
        lineAdj = atof(opt->getValue("lineAdj"));
    }
    if(opt->getValue("classifier") != NULL){
        classificationMethod = Utils::getClassificationMethod(opt->getValue("classifier"));
    }

    if(opt->getValue("sampler") 	   != NULL){samplingMethod  = Utils::getSamplingMethod(opt->getValue("sampler"));	}
    if(opt->getValue("samplingPop")    != NULL){samplingPopSize = atoi(opt->getValue("samplingPop"));}
    if(opt->getValue("samplingGen")    != NULL){samplingGenSize = atoi(opt->getValue("samplingGen"));}
    if(opt->getValue("sigma")	 	   != NULL){sigma           = atof(opt->getValue("sigma"));}
    if(opt->getValue("numSamples")	   != NULL){numSamples		= atoi(opt->getValue("numSamples"));}

    if(opt->getValue("pruner") != NULL){
        pruningMethod = Utils::getPruningMethod(opt->getValue("pruner"));
    }
    if(opt->getValue("pruningObj") != NULL){
        pruningObj = Utils::getPruningObj(opt->getValue("pruningObj"));
    }
    if(opt->getValue("stoppingMethod") != NULL){
        stoppingMethod = Utils::getStoppingMethod(opt->getValue("stoppingMethod"));
    }
    if(opt->getValue("pruningPopMin")  != NULL){pruningPopSize	= atoi(opt->getValue("pruningPopMin"));}
    if(opt->getValue("pruningPopMax")  != NULL){pruningPopMax	= atoi(opt->getValue("pruningPopMax"));}
    if(opt->getValue("pruningPopStep") != NULL){pruningPopStep 	= atoi(opt->getValue("pruningPopStep"));}
    if(opt->getValue("pruningGenMin")  != NULL){pruningGenSize 	= atoi(opt->getValue("pruningGenMin"));}
    if(opt->getValue("pruningGenMax")  != NULL){pruningGenMax	= atoi(opt->getValue("pruningGenMax"));}
    if(opt->getValue("pruningGenStep") != NULL){pruningGenStep 	= atoi(opt->getValue("pruningGenStep"));}

    if(opt->getValue("stoppingValue")  != NULL){stoppingValue  	= atof(opt->getValue("stoppingValue"));}
    if(opt->getValue("trials") != NULL)		   {trials 			= atoi(opt->getValue("trials"));}

    /************************************ End Command Line Options ************************************/

    Utils::loadSystemData(pLoad, qLoad, numBuses, numLines, curSystem, gens, lines, buses);

    if(classificationMethod == pruning::CM_OPF)     { classifier = new OPFClassifier(curSystem, numBuses, gens, buses, lines);}
    else if(classificationMethod == pruning::CM_SVM){ classifier = new SVMClassifier(curSystem, numBuses, gens, buses, lines);}
    else if(classificationMethod == pruning::CM_CAP){ classifier = new CapacityClassifier(curSystem, numBuses, gens, buses, lines, pLoad);}
    classifier->setUseLines(useLines);

    /************************************ Begin File Setup ************************************/
    //aTime = Utils::getTimeStamp();
    aTime = new char[20];
    Utils::getTimeStamp(aTime);
    fName = Utils::getFileName("../Results", curSystem, Utils::getSamplingMethodString(samplingMethod), Utils::getPruningMethodString(pruningMethod), 
                                Utils::getClassificationMethodString(classificationMethod), Utils::getPruningObjString(pruningObj),
                                useLines, useMoPruner, aTime, useLocalSearch, usePHEVs, negateFitness, phevPenetration, phevPlacement, numThreads);
                                
    myFile.open(fName.c_str(), ios::trunc);
    myFile << "\"Run #\" \"Population\" \"Generations\" \"Success States\" \"Failure States\" \"States Slope\" \"Prob Slope\" \"Sampler LOLP\" ";
    myFile << "\"Pruned LOLP\" \"Estimate LOLP\" \"Pruning Time\" \"State Generation Time\" \"Search Time\", \"Classification Time\" ";
    myFile << "\"Simulation Time\" \"Total Time\" \"Sampler Iterations\" \"Pruning Iterations\" ";
    myFile << "\"Collisions\" \"Avg # Lines Out\" Line Adj\" \"Avg # Gens Out\" \"PHEV Penetration\" \"PHEV Rho\" \"numThreads\"";
    myFile << endl;
    /************************************ End File Setup ************************************/

    cout	<< fixed
            << setw(5)  << left << "Run "
            << setw(10) << left << "Sampling "
            << setw(10) << left << "Pruning";
    if(pruningMethod != pruning::PM_NONE){
        cout	<< fixed
                << setw(5)  << left << "Pop "
                << setw(5)  << left << "Gen "
                << setw(10) << left << "SS"
                << setw(10) << left << "FS  ";
    }
    cout    << setw(10) << left << "System "
            << setw(10) << left << "LOLP"
            << setw(20) << left << "Sigma";
    
    cout	<< setw(10) << left << "Samples"
            << setw(15) << left << "Unique States"
            << setw(15) << left << "Collisions"
            << setw(15) << left << "Sampler Time"
            << setw(15) << left << "Total Time"
            << endl;

    for(; pruningPopSize <= pruningPopMax; pruningPopSize += pruningPopStep){
        for(int generations = pruningGenSize; generations <= pruningGenMax; generations += pruningGenStep){
            for(int t=0; t<trials; t++){
                run++;
                timer.startTimer();

                // Reset!
                failedStates.clear(); successStates.clear();
                classifier->init();
                classifier->resetTimes();
                pruningProb	= 0;
                if(stoppingMethod == pruning::SM_GENERATION){ stoppingValue = generations;} // Just in case

                if(useMoPruner){
                    if(pruningMethod == pruning::PM_GA){

                    }else if(pruningMethod == pruning::PM_MGA){

                    }else if(pruningMethod == pruning::PM_PSO){
                        if(mp == NULL){
                            if(!useWeightedMO && !useParetoMO){
                                p = new MO_PSOPruner(pruningPopSize, generations, classifier, gens, lines, pLoad, useLines);
                            }else if(useParetoMO){
                                p = new MO_PSOPruner_Pareto(pruningPopSize, generations, classifier, gens, lines, pLoad, useLines);
                            }else if (useWeightedMO){
                                cout << "Combination does not yet exist!\n";
                                exit(1);
                            }
                        }else{
                            p->Reset(pruningPopSize, generations);
                        }
                    }else if(pruningMethod == pruning::PM_RPSO){

                    }else if(pruningMethod == pruning::PM_ACO){

                    }
                }else{
                    if(pruningMethod == pruning::PM_GA){
                        if(p == NULL){
                            p = new GAPruner(pruningPopSize, generations, classifier, gens, lines, pLoad, useLines);
                        }else{
                            p->Reset(pruningPopSize, generations);
                        }
                    }else if(pruningMethod == pruning::PM_MGA){
                        if(p == NULL){
                            p = new ModifiedGAPruner(pruningPopSize, generations, classifier, gens, lines, pLoad, useLines);
                        }else{
                            p->Reset(pruningPopSize, generations);
                        }
                    }else if(pruningMethod == pruning::PM_PSO){
                        if(p == NULL){
                            p = new PSOPruner(pruningPopSize, generations, classifier, gens, lines, pLoad, useLines);
                            //((PSOPruner*)p)->C1 = 4;((PSOPruner*)p)->C2 = 10; ((PSOPruner*)p)->W = 3;
                        }else{
                            p->Reset(pruningPopSize, generations);
                        }
                    }else if(pruningMethod == pruning::PM_RPSO){
                        if(p == NULL){
                            p = new RPSOPruner(pruningPopSize, generations, classifier, gens, lines, pLoad, useLines);
                        }else{
                            p->Reset(pruningPopSize, generations);
                        }
                    }else if(pruningMethod == pruning::PM_ACO){
                        if(p == NULL){
                            p = new ACOPruner(pruningPopSize, generations, classifier, gens, lines, pLoad,  stoppingMethod, useLines);
                        }else{
                            p->Reset(pruningPopSize, generations);
                        }
                    }else if(pruningMethod == pruning::PM_AIS){
                        if(p == NULL){
                            p = new AISPruner(pruningPopSize, generations, classifier, gens, lines, pLoad, useLines, .35, .35);
                        }else{
                            p->Reset(pruningPopSize, generations);
                        }
                    }else if(pruningMethod == pruning::PM_CFO){
                        if(p == NULL){
                            p = new CFOPruner(pruningPopSize, generations, classifier, gens, lines, pLoad, useLines);
                        }else{
                            p->Reset(pruningPopSize, generations);
                        }
                    }
                }

                if(p != NULL){
                    p->setUseLogging(logConvergence);
                    p->setNegateFitness(negateFitness);
                    p->setLineAdjustment(lineAdj);
                    p->setuseLocalSearch(useLocalSearch);
                    p->setObjective(pruningObj);
                    p->setStoppingMethod(stoppingMethod);
                    p->setStoppingValue(stoppingValue);
                    p->setUsePHEVs(usePHEVs);
                    p->setPHEVPlacement(phevPlacement);
                    p->setNumBuses(numBuses);
                    p->setPermuteSolutions(permuteSolutions);
                    p->setNumThreads(numThreads);
                    p->Prune(mt);
                    pruningTime 	= p->getPruningTime();
                    successStates 	= p->getSuccessStates();
                    failedStates 	= p->getFailureStates();
                }

                if(sampler == NULL){
                    if(samplingMethod == pruning::SM_ACO)	   {}
                    else if(samplingMethod == pruning::SM_AIS) {}
                    else if(samplingMethod == pruning::SM_GA)  {}
                    else if(samplingMethod == pruning::SM_MGA) {}
                    else if(samplingMethod == pruning::SM_MCS) { sampler = new MCS_Sampler();}
                    else if(samplingMethod == pruning::SM_LHS) { 
                        sampler = new LHS_Sampler();
                        ((LHS_Sampler*)sampler)->setNumSamples(numSamples);    
                        ((LHS_Sampler*)sampler)->setBatchSize(batchSize);  
                    }else if(samplingMethod == pruning::SM_DS){
                        sampler = new DS_Sampler();
                        ((DS_Sampler*)sampler)->setNumSamples(numSamples);  
                    }else if(samplingMethod == pruning::SM_HAL || samplingMethod == pruning::SM_HAM || samplingMethod == pruning::SM_FAU){
                        sampler = new LDS_Sampler(samplingMethod, numSamples);
                    }else if(samplingMethod == pruning::SM_RPSO) {
                    }else if(samplingMethod == pruning::SM_MO_PSO){ 
                        if(useLines && lines.size() > 0){ sampler = new MO_PSO_Sampler(samplingPopSize, gens.size()+lines.size(), samplingGenSize); }
                        else                            { sampler = new MO_PSO_Sampler(samplingPopSize, gens.size(), samplingGenSize);}
                    }
                    else if(samplingMethod == pruning::SM_PSO){ 
                        if(useLines && lines.size() > 0){ 
                            sampler = new PSO_Sampler(samplingPopSize, gens.size()+lines.size(), samplingGenSize); }
                        else                            { sampler = new PSO_Sampler(samplingPopSize, gens.size(), samplingGenSize);}
                    }
                    else if(samplingMethod == pruning::SM_CFO){ 
                        if(useLines && lines.size() > 0){ sampler = new CFO_Sampler(samplingPopSize, gens.size()+lines.size(), samplingGenSize); }
                        else                            { sampler = new CFO_Sampler(samplingPopSize, gens.size(), samplingGenSize);}    
                    }
                }
                sampler->setCurrentSystem(curSystem);
                sampler->Init(failedStates, successStates, gens, lines, pLoad, classifier, numBuses, totalVehicles, phevPenetration, phevRho);
                sampler->setUseLines(useLines);
                sampler->setUsePHEVs(usePHEVs);
                sampler->setLineAdjustment(lineAdj);
                sampler->setPHEVPlacement(phevPlacement);
                sampler->setTolerance(sigma);
                sampler->setUseLogging(logConvergence);				
                sampler->run(mt);

                timer.stopTimer();
                totalTime = timer.getElapsedTime();

                if(useLines){lineOutages.push_back(sampler->getTLineOutageCounts());}
                genOutages.push_back(sampler->getGenOutageCounts());

                //Calculate LOLP
                if(p != NULL){pruningProb = p->getPrunedProb();}
                else		 {pruningProb = 0;}
                eLOLP = sampler->getLOLP() * (1 - pruningProb);

                /************************************ Begin File Output ************************************/
                myFile << run << " ";
                if(p != NULL){
                    myFile 	<< pruningPopSize << " "
                            << generations 			<< " "
                            << successStates.size()	<< " "
                            << failedStates.size()	<< " ";
                }else{
                    myFile 	<< 0 << " "
                            << 0 << " "
                            << 0 << " "
                            << 0 << " ";
                }

                if(p != NULL){
                    myFile  << setprecision(7)
                            << p->getStatesSlope()	<< " "
                            << p->getProbSlope()	<< " ";
                }else{
                    myFile  << setprecision(7)
                            << 0 << " "
                            << 0 << " ";
                }
                myFile << setprecision(7)<< sampler->getLOLP() << " ";
                if(p != NULL){
                    myFile  << setprecision(7)
                            << (1 - pruningProb) 	<< " "
                            << eLOLP 			 	<< " "
                            << p->getPruningTime() 	<< " ";
                }else{
                    myFile  << setprecision(7)
                            << 0 << " "
                            << 0 << " "
                            << 0 << " ";
                }

                myFile	<< setprecision(7)
                        << sampler->getGenTime()            << " "
                        << sampler->getSearchTime()         << " "
                        << sampler->getClassificationTime() << " " 
                        << sampler->getSimTime()	        << " "
                        << totalTime				        << " ";
                myFile  << setprecision(0)
                        << sampler->getTotalIterations() 	<< " ";

                if(p != NULL){
                    myFile 	<< p->getTotalIterations() 	    << " "
                            << sampler->getTotalCollisions() << " ";
                }else{
                    myFile 	<< 0 << " "
                            << 0 << " ";
                }
                
                if(useLines){
                    myFile	<< setprecision(3)
                            << sampler->getAvgLinesOut()  		<< " "
                            << sampler->getLineAdjustment()	<< " ";
                }else{
                    myFile	<< setprecision(3)
                            << 0 << " "
                            << 0 << " ";
                }
                myFile << sampler->getAvgGensOut()	<< " ";
                
                if(usePHEVs){
                    myFile << fixed << setprecision(3)
                           << phevPenetration		    << " "
                           << phevRho					<< " ";
                }else{
                    myFile << fixed << setprecision(2)
                           << 0	<< " "
                           << 0	<< " ";
                }
                myFile << numThreads;
                if(classificationMethod == pruning::CM_SVM){
                    myFile << fixed << setprecision (4)
                           << ((SVMClassifier*)classifier)->getSensitivity() 	<< " "
                           << ((SVMClassifier*)classifier)->getSpecificity() 	<< " "
                           << ((SVMClassifier*)classifier)->getAccuracy()    	<< " "
                           << ((SVMClassifier*)classifier)->getPrecision()	    << " ";
                }
                myFile << endl;

                if(logConvergence){
                    aTime2 = new char[20];
                    Utils::getTimeStamp(aTime2);
                    if(p != NULL){
                        logFileName = Utils::getFileName("../Results", curSystem, Utils::getSamplingMethodString(samplingMethod), Utils::getPruningMethodString(pruningMethod),
                                Utils::getClassificationMethodString(classificationMethod), Utils::getPruningObjString(pruningObj),
                                useLines, useMoPruner, aTime2, useLocalSearch, usePHEVs, negateFitness, phevPenetration, phevPlacement, numThreads);
                        logFileName = logFileName.replace(logFileName.end()- 4,logFileName.end(), "_Pruner_Convergence.csv");
                        p->writeLog(logFileName);
                    }
                    logFileName = Utils::getFileName("../Results", curSystem, Utils::getSamplingMethodString(samplingMethod), Utils::getPruningMethodString(pruningMethod),
                                                Utils::getClassificationMethodString(classificationMethod), Utils::getPruningObjString(pruningObj),
                                                useLines, useMoPruner, aTime2, useLocalSearch, usePHEVs, negateFitness, phevPenetration, phevPlacement, numThreads);
                    logFileName = logFileName.replace(logFileName.end()- 4,logFileName.end(), "_Sampler_Convergence.csv");
                    sampler->writeLog(logFileName);

                    delete aTime2;
                }
                /************************************ End File Output ************************************/

                cout	<< fixed
                        << setprecision(0) << setw(5)  << left << run
                        << setprecision(0) << setw(10) << left << Utils::getSamplingMethodString(samplingMethod)  
                        << setprecision(0) << setw(10) << left << Utils::getPruningMethodString(pruningMethod);
                        if(p != NULL){
                            cout	<< fixed
                                    << setw(5)  << left << pruningPopSize
                                    << setw(5)  << left << generations
                                    << setw(10) << successStates.size() 
                                    << setw(10) << failedStates.size();
                        }
                cout	<< setprecision(0) << setw(10) << left << curSystem
                        << setprecision(4) << setw(10) << left << eLOLP
                        << setprecision(4) << setw(20) << left << sampler->getSigma() 
                        //<< setprecision(4) << setw(20) << left << printf("%0.4f/%0.4f/%0.4f", sampler->getSigma(), sampler->getNSigma(),  (sampler->getSigma()+sampler->getNSigma())/2) 
                        << setprecision(0) << setw(10) << left << sampler->getTotalIterations()
                        << setprecision(0) << setw(15) << left << sampler->getNumStatesSampled()
                        << setprecision(0) << setw(15) << left << sampler->getTotalCollisions()
                        << setprecision(2) << setw(15) << left << sampler->getSimTime()
                        << setprecision(2) << setw(15) << left << totalTime << endl;

                if(pruningMethod == pruning::PM_PSO && useParetoMO){
                    ((MO_PSOPruner_Pareto*)p)->printArchive();
                }
            }
        }
    }
    myFile.close();
    if(saveLineOutages && useLines){
        aTime2 = new char[20];
        Utils::getTimeStamp(aTime2);
        Utils::writeLineOutages("../Results", curSystem, Utils::getSamplingMethodString(samplingMethod),
                Utils::getPruningMethodString(pruningMethod), Utils::getClassificationMethodString(classificationMethod),
                Utils::getPruningObjString(pruningObj),
                useLines, useMoPruner, aTime2, useLocalSearch, usePHEVs, negateFitness, phevPenetration, phevPlacement, lineOutages, lines, numThreads);
        delete aTime2;
    }
    if(saveGenOutages){
        aTime2 = new char[20];
        Utils::writeGeneratorOutages("../Results", curSystem, Utils::getSamplingMethodString(samplingMethod),
                Utils::getPruningMethodString(pruningMethod), Utils::getClassificationMethodString(classificationMethod),
                Utils::getPruningObjString(pruningObj),
                useLines, useMoPruner, aTime2, useLocalSearch, usePHEVs, negateFitness, phevPenetration, phevPlacement, genOutages, gens, numThreads);
        delete aTime2;
    }
    
    // Clean Up
    delete classifier;
    delete p;
    delete aTime;
    delete opt;

    return 0;
}

