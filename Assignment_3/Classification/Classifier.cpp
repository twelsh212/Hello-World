/*
 * Classifier.cpp
 *
 *  Created on: Jan 18, 2011
 *      Author: rgreen
 */

#include "Classifier.h"

Classifier::Classifier() {
    curSystem			= "RTS79";
    nb					= 24;
    loadTime 			= 0;
    classificationTime	= 0;
    resetTime			= 0;
    useLines			= false;
    busLoads.resize(nb, 0);
    addedLoad.resize(nb, 0);
}

Classifier::Classifier(std::string c, int n, std::vector < Generator > g, std::vector < Bus > b) {
    curSystem			= c;
    nb					= n;
    loadTime 			= 0;
    classificationTime	= 0;
    gens				= g;
    buses				= b;
    resetTime			= 0;
    useLines			= false;
    busLoads.resize(nb, 0);
    addedLoad.resize(nb, 0);
}

Classifier::Classifier(std::string c, int n, std::vector < Generator > g, std::vector < Bus > b, std::vector< Line > l) {
    curSystem			= c;
    nb					= n;
    loadTime 			= 0;
    classificationTime	= 0;
    gens				= g;
    lines				= l;
    buses				= b;
    resetTime			= 0;
    useLines			= false;
    busLoads.resize(nb, 0);
    addedLoad.resize(nb, 0);
}

Classifier::~Classifier() {
    gens.clear();       lines.clear();
    addedLoad.clear();  busLoads.clear();
}

Classifier& Classifier::operator=(const Classifier &L){
    return *this;
}

double Classifier::getLoadTime()           { return loadTime;}
double Classifier::getClassificationTime() { return classificationTime;}
double Classifier::getResetTime()          { return resetTime;}

void Classifier::resetTimes(){
    loadTime = 0;
    classificationTime = 0;
    resetTime = 0;
}

void Classifier::load(){}
double Classifier::run(std::string curSolution, double& excess)         {return 0;}
double Classifier::run(std::vector<int> curSolution, double& excess)    {return 0;}
double Classifier::run(std::vector<double> curSolution, double& excess) {return 0;}

void Classifier::setUseLines(bool b)                  { useLines  = b;};
void Classifier::addLoad(std::vector<double>& amounts) { 
    if((int)amounts.size() == nb){
       for(unsigned int i=0; i<amounts.size(); i++){
           addLoad(i+1, amounts[i]);
        }
    }
};
void Classifier::addLoad(int busNumber, double amount){
    if(busNumber < (int)addedLoad.size()){
        addedLoad[busNumber] = amount;
    }
}

