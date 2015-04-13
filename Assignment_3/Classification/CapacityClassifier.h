/*
 * CapacityClassifier.h
 *
 *  Created on: Feb 11, 2011
 *      Author: rgreen
 */

#ifndef CAPACITYCLASSIFIER_H_
#define CAPACITYCLASSIFIER_H_

#include "Classifier.h"
#include <string>
#include <vector>

#include "Line.h"
#include "Generator.h"

class CapacityClassifier: public Classifier {
    public:
        CapacityClassifier();
        CapacityClassifier(std::string curSystem, int n, std::vector < Generator > g, std::vector < Bus > b, double load);
        CapacityClassifier(std::string curSystem, int n, std::vector < Generator > g, std::vector < Bus > b, std::vector < Line > t, double load);
        CapacityClassifier(std::string curSystem, int n, std::vector < Generator > g, std::vector < Bus > b, std::vector < Line > t, Classifier* cl, double load);
        virtual ~CapacityClassifier();
        virtual CapacityClassifier* clone() const; 

        void load(){};
        void init(){};

        double run(std::string curSolution, double& excess);
        double run(std::vector<int> curSolution, double& excess);
        double run(std::vector<double> curSolution, double& excess);
        double getClassificationTime();
        double getLoadTime();
        double getResetTime();
        double getLoad();

        void setVerbose(bool v);

        void reset();
    private:
        double totalLoad;
        double originalLoad;
};

#endif /* CAPACITYCLASSIFIER_H_ */
