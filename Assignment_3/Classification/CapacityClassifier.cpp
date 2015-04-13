/*
 * CapacityClassifier.cpp
 *
 *  Created on: Feb 11, 2011
 *      Author: rgreen
 */

#include "CapacityClassifier.h"
#include <iostream>

CapacityClassifier::CapacityClassifier() : Classifier() {
    originalLoad = totalLoad = 0;
}
CapacityClassifier::CapacityClassifier(std::string c, int n, std::vector < Generator > g, std::vector < Bus > b, double load) : Classifier(c, n, g, b) {
    originalLoad = totalLoad = load;
}

CapacityClassifier::CapacityClassifier(std::string c, int n, std::vector < Generator > g, std::vector < Bus > b, std::vector < Line > l, double load) : Classifier(c, n, g, b, l) {
    originalLoad = totalLoad = load;
}

CapacityClassifier::CapacityClassifier(std::string c, int n, std::vector < Generator > g, std::vector < Bus > b, std::vector < Line > l, Classifier* cl, double load) : Classifier (c, n, g, b, l){
    originalLoad = totalLoad = load;
}

CapacityClassifier::~CapacityClassifier() {
    // TODO Auto-generated destructor stub
}
void CapacityClassifier::reset(){
    totalLoad = originalLoad;
}

CapacityClassifier* CapacityClassifier::clone() const { return new CapacityClassifier(*this);}

double CapacityClassifier::run(std::vector<double> curSolution, double& excess){
    double retValue, totalCap;

    retValue = 0; totalCap = 0;
    timer.startTimer();

    for(unsigned int i=0; i<addedLoad.size(); i++){
        totalLoad += addedLoad[i];
    }

    for(unsigned int i=0; i<gens.size(); i++){
        if(curSolution[i] == 1){
            totalCap += gens[i].getPG()/100;
        }
    }

    if(totalCap < totalLoad) { retValue =  totalLoad - totalCap;}
    else                     { retValue = 0;}
    timer.stopTimer();
    classificationTime += timer.getElapsedTime();

    return retValue;
}
double CapacityClassifier::run(std::vector<int> curSolution, double& excess){
    std::vector<double> newSolution(curSolution.size(),0);

    for(unsigned int i=0; i<curSolution.size(); i++){
        newSolution[i] = curSolution[i];
    }
    return run(newSolution, excess);
}
double CapacityClassifier::run(std::string curSolution, double& excess){
    std::vector<double> newSolution(curSolution.size());

    for(unsigned int i=0; i<curSolution.size(); i++){
        newSolution[i] = atof(&curSolution[i]);
    }
    return run(newSolution, excess);
}
