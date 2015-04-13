/*
 * OPF.h
 *
 *  Created on: Feb 5, 2010
 *      Author: rgreen
 */

#ifndef OPF_H_
#define OPF_H_

#include <vector>
#include <string>

#include "Eigen/Dense"
#include "Classifier.h"
#include "lp_lib.h"
#include "Utils.h"
#include "Generator.h"

using namespace Eigen;

class OPFClassifier : public Classifier {
    public:
        OPFClassifier();
        OPFClassifier(std::string curSystem, int n, std::vector < Generator > g, std::vector < Bus > b);
        OPFClassifier(std::string curSystem, int n, std::vector < Generator > g, std::vector < Bus > b, std::vector < Line > t);
        virtual ~OPFClassifier();

        virtual OPFClassifier* clone() const; 
        OPFClassifier(const OPFClassifier &L);             // copy constructor

        void initialInit();
        void load();
        void reset();
        void init();

        /*void addLoad(int busNumber, double amount);
        void addLoad(std::vector<double>& amounts);*/

        double run(std::string curSolution, double& excess);
        double run(std::vector<int> curSolution, double& excess);
        double run(std::vector<double> curSolution, double& excess);

        void formulateOPF(std::string curSolution);

        lprec* getLP();

    private:

        void calculateMatrices(std::string curSolution);
        void buildOPF		  (std::string curSolution);
        void updateOPF        (std::string curSolution);

        lprec* lp;
        //gsl_matrix *Bbus, *Bf;

        Eigen::Matrix<double, Dynamic, Dynamic> Bbus, Bf;

        bool loaded, built;
        int numModels;
        std::vector<double> origCap, origFlow, origDemand, origCurtail;
        std::vector<int> generatorIndices, lineIndices, demandIndices, curtailIndices;
};

#endif /* OPF_H_ */
