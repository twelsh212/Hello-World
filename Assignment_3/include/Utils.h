/*
 * Utils.h
 *
 *  Created on: Jan 11, 2010
 *      Author: rgreen
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#ifndef _OPENMP
    #include "omp.h"
#endif

#include "anyoption.h"
#include "Bus.h"
#include "defs.h"
#include "Generator.h"
#include "Line.h"
#include "MTRand.h"
#include "Primes.h"
#include "RandomNumbers.h"

namespace Utils {

    extern void printVector(std::vector < std::vector < double > >& v, std::string title, int precision = 2);
    extern void printVector(std::vector < std::vector < std::vector < double > > >& v, std::string title, int precision = 2);

    extern std::vector<double> decToBin(double n, int numDigits);
    extern double unitStep(double X);
    extern std::vector< std::vector<int>> matMult(std::vector< std::vector<int> > A, std::vector< std::vector<int> > B);

    extern std::string toLower(std::string str);
    extern std::string toUpper(std::string str);
    extern std::vector<std::string> permuteCharacters(std::string topermute);
    extern std::string changeBase(std::string Base, int number);

    extern std::string vectorToString(std::vector<double> v);
    extern std::string vectorToString(std::vector<int> v);
    extern std::string arrayToString(int* v, int size);

    extern double sigMoid(double v);
    extern void tokenizeString(std::string str,std::vector<std::string>& tokens,const std::string& delimiter );
    extern int factorial(int n);
    extern int combination(int n, int r);
    extern void loadSystemData(double& pLoad, double& qLoad, int& nb, int& nt, std::string curSystem, std::vector<Generator>& gens, std::vector<Line>& lines, std::vector<Bus>& Buses);
           
    extern void getTimeStamp(char* aTime);

    extern std::string getClassificationMethodString(pruning::CLASSIFICATION_METHOD cm);
    extern std::string getSamplingMethodString		(pruning::SAMPLING_METHOD 		ps);
    
    extern pruning::CLASSIFICATION_METHOD getClassificationMethod (std::string s);
    extern pruning::SAMPLING_METHOD       getSamplingMethod		  (std::string s);

    extern void setUsage(AnyOption* opt);
    extern void setOptions(AnyOption* opt);
};

#endif /* UTILS_H_ */