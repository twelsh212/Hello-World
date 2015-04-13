/*
 * Classifier.h
 *
 *  Created on: Jan 18, 2011
 *      Author: rgreen
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <vector>
#include <string>

#include "Utils.h"
#include "CStopWatch.h"
#include "Generator.h"
#include "Line.h"


class Classifier {
    public:
        Classifier();
        Classifier(std::string curSystem, int n, std::vector < Generator > g, std::vector < Bus > b);
        Classifier(std::string curSystem, int n, std::vector < Generator > g, std::vector < Bus > b, std::vector < Line > t);
        virtual ~Classifier();

        virtual Classifier* clone() const = 0; 
        virtual Classifier & operator=(const Classifier &L); // assignment

        virtual void load();
        virtual void resetTimes();
        virtual void reset(){};
        virtual void init(){};

        virtual double run(std::string curSolution, double& excess);
        virtual double run(std::vector<int> curSolution, double& excess);
        virtual double run(std::vector<double> curSolution, double& excess);
        
        double getClassificationTime();
        double getLoadTime();
        double getResetTime();

        void setUseLines(bool b);

        void addLoad(std::vector<double>& amounts);
        void addLoad(int busNumber, double amount);

    protected:
        bool 		            useLines;
        CStopWatch              timer;
        double 	                loadTime, classificationTime, resetTime;

        int 					nb;
        std::string 			curSystem;
        std::vector<Generator> 	gens;
        std::vector<Line> 		lines;
        std::vector<Bus>		buses;
        std::vector<double>		busLoads, addedLoad;
};

#endif /* CLASSIFIER_H_ */
