/*
 * Sampler.h
 *
 *  Created on: Feb 25, 2011
 *      Author: rgreen
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <map>
#include <string>
#include <vector>

#include "defs.h"
#include "Classifier.h"
#include "CStopWatch.h"
#include "Generator.h"
#include "Line.h"
#include "MTRand.h"

class Sampler {
    public:
        Sampler();
        virtual ~Sampler();

        virtual void Init(std::map < std::string, double > fs, std::map < std::string, double > ss, std::vector < Generator > g, double load, Classifier* l,
                            int nb, int tv = 1539000, double p = 0.05, double r = 0.8);
        virtual void Init(std::map < std::string, double > fs, std::map < std::string, double > ss, std::vector < Generator > g, std::vector < Line > t, double load, Classifier* l,
                    int nb, int tv = 1539000, double p = 0.05, double r = 0.8);

        virtual void run(MTRand& mt) = 0;

        double getLOLP();
        double getSimTime();
        double getGenTime();
        double getSearchTime();
        double getClassificationTime();
        double getNLOLP();
        double getSigma();
        double getNSigma();

        double getAvgLinesOut();
        double getAvgGensOut();
        double getTolerance();
        double getLineAdjustment();

        int getGenCount();
        int getLineCount();
        int getTotalCollisions();
        int getTotalIterations();
        int getNumStatesSampled();

        std::vector<int> getTLineOutageCounts();
        std::vector<int> getGenOutageCounts();
        std::map<std::string, int> getSampledStateOccurences();

        void setLineAdjustment(double la);
        void setNumBuses(int nb);
        void setTolerance(double t);
        void setUseLogging(bool ul);
        void setUseLines(bool ul);
        void setCurrentSystem(std::string cs);

        void writeLog(std::string fileName);

    protected:

        bool 		usePHEVs, useLogging, useLines;
        CStopWatch 	timer, timer1;
        int 		iterations, Collisions;
        double 		curProb;
        double      PLC;
        double 		vLOLP, pLOLP, LOLP;
        double 		NLOLP, vNLOLP, pNLOLP;
        double      EENS, pEENS, vEENS;

        double 		numSamples;
        double 		simulationTime, stateGenerationTime, searchTime, classificationTime;
        double 		lineAdjustment;
        double 		pLoad;
        double 		tolerance;

        std::vector<int>					fStatesCount, sStatesCount;
        std::map<std::string, double> 		failedStates, successStates;
        std::map<std::string, int> 			sampledStates;
        std::map<std::string, int> 			sampledStateOccurrences;
        std::map<std::string, long double> 	sampledStateProbs;
        std::map<std::string, std::vector < double > > uniqueStates;

        std::string				curSystem;
        std::vector<Generator> 	gens;
        std::vector<Line> 		lines;

        int 			 lineCount, numBuses;
        double      	 avgLineCount;
        std::vector<int> tLineOutageCounts;

        std::vector<int> uniqueSuccessStatesCount;
        std::vector<int> uniqueFailedStatesCount;

        std::map<std::string, double> localFailedStates, localSuccessStates;
        std::map<std::string, double> stateCurtailment;
        std::vector<double>			  lolps, sigmas, vLolps;
        std::vector<double>			  nLolps, nSigmas;

        int 			 genCount;
        double      	 avgGenCount;
        std::vector<int> genOutageCounts;

        Classifier* classifier;

        int 						totalVehicles, numClasses;
        double 						penetrationLevel, CDD, rho;
};

#endif /* SAMPLER_H_ */
