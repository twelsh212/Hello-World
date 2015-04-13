/*
 * Sampler.cpp
 *
 *  Created on: Feb 25, 2011
 *      Author: rgreen
 */

#include "Sampler.h"

Sampler::Sampler() {
    pLoad 			    = 0.0;
    LOLP  			    = 0.0;
    iterations 	    = 0;
    simulationTime 	    = 0.0;
    classificationTime  = 0.0;
    Collisions	    = 0.0;
    lineAdjustment 	    = 1;
    tolerance		    = 0.025;
    usePHEVs		    = false;

    totalVehicles 	    = 1539000;
    penetrationLevel    = 0.05;
    rho 			    = 0.8;
    useLogging		    = false;
    useLines		    = false;
}

Sampler::~Sampler() {

}
void Sampler::Init(std::map < std::string, double > fs, std::map < std::string, double > ss, std::vector < Generator > g, double load, Classifier* l,
        int nb, int tv, double p, double r){
    failedStates 	    = fs;
    successStates 	    = ss;
    gens			    = g;
    pLoad			    = load;
    classifier		    = l;

    LOLP  			    = 0.0;
    iterations 	    = 0;
    simulationTime 	    = 0.0;
    classificationTime  = 0.0;
    Collisions	        = 0.0;
    lineAdjustment      = 1;
    numBuses		    = nb;
    tolerance 		    = 0.025;
    usePHEVs		    = false;

    numBuses		    = nb;
    totalVehicles 	    = tv;
    penetrationLevel    = p;
    rho 			    = r;
    useLogging		    = false;
    useLines		    = false;
}
void Sampler::Init(std::map < std::string, double > fs, std::map < std::string, double > ss, std::vector < Generator > g, std::vector < Line > t,
                    double load, Classifier* l, int nb, int tv, double p, double r){

    failedStates 	    = fs;
    successStates 	    = ss;
    gens			    = g;
    lines			    = t;
    pLoad			    = load;
    classifier		    = l;

    LOLP  			    = 0.0;
    iterations 		= 0;
    simulationTime 		= 0.0; 
    classificationTime  = 0.0;
    Collisions		    = 0.0;
    lineAdjustment 	    = 1;
    tolerance 		    = 0.025;
    numBuses		    = nb;
    usePHEVs		    = false;

    totalVehicles 	    = tv;
    penetrationLevel    = p;
    rho 			    = r;
    useLogging		    = false;
    useLines		    = false;
}
void Sampler::writeLog(std::string fileName){
    if(!useLogging) return;

    ofstream logFile;
    double ss = 0, fs = 0;
    logFile.open(fileName.c_str(), ios::trunc);
    if(logFile.is_open()){
        logFile << "\"Iteration\" \"Success\" \"Failure\" \"Success States\" \"Failure States\" "
                << "\"LOLP\" \"Variance\" \"nLOLP\" \"nVariance\"";

        for(int i=0; i<iterations; i++){

            if(sStatesCount[i] == 0){ fs++;}
            else					{ ss++;}
            logFile << i << " " << sStatesCount[i] 	<< " "
                                << fStatesCount[i] 	<< " "
                                << ss 				<< " "
                                << fs 				<< " "
                                << lolps[i] 		<< " "
                                << sigmas[i] 		<< " "
                                << nLolps[i] 		<< " "
                                << nSigmas[i] 		<< "\n";
        }
        logFile.close();
    }
}

void Sampler::setLineAdjustment(double la)				  { lineAdjustment = la;}
void Sampler::setTolerance(double t)					  { tolerance = t;}
void Sampler::setNumBuses(int nb)						  { numBuses = nb;}
void Sampler::setUseLogging(bool ul) 					  { useLogging = ul;}
void Sampler::setUseLines(bool ul)						  { useLines = ul;}
void Sampler::setCurrentSystem(std::string cs)			  { curSystem = cs;}

double Sampler::getAvgGensOut()		    { return avgGenCount;}
double Sampler::getAvgLinesOut()	    { return avgLineCount;}
double Sampler::getTolerance()		    { return tolerance;}
double Sampler::getNLOLP()			    { return NLOLP;}
double Sampler::getLineAdjustment()     { return lineAdjustment;}
double Sampler::getLOLP()			    { return LOLP;}
double Sampler::getSimTime()		    { return simulationTime;}
double Sampler::getGenTime()		    { return stateGenerationTime;}
double Sampler::getSearchTime()		    { return searchTime;}
double Sampler::getClassificationTime() { return classificationTime;};
double Sampler::getSigma()			    { return sigmas.size() > 0 ? sigmas[sigmas.size()-1] : 0;}
double Sampler::getNSigma()			    { return nSigmas.size() > 0 ? nSigmas[nSigmas.size()-1] : 0;}

int Sampler::getTotalIterations()	    { return iterations;}
int Sampler::getTotalCollisions()	    { return Collisions;}
int Sampler::getLineCount()			    { return lineCount;}
int Sampler::getGenCount()			    { return genCount;}
int Sampler::getNumStatesSampled()	    { return uniqueStates.size();}

std::vector<int> Sampler::getGenOutageCounts()				{ return genOutageCounts;}
std::vector<int> Sampler::getTLineOutageCounts()			{ return tLineOutageCounts;}
std::map<string, int> Sampler::getSampledStateOccurences()	{ return sampledStateOccurrences;}

