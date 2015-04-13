#include "Utils.h"
#include <iostream>
#include "Eigen\Dense"

namespace Utils{

    std::vector<double> decToBin(double n, int numDigits) {
        std::vector<double> retVal;


        if(n <= 1) {
            retVal.push_back(n);
        }else{
            while(n > 0){
                retVal.push_back(floor(fmod(n, 2)));
                //retVal.push_back((int)n % 2);
                n = floor(n/2);  
            }
        }

        while(retVal.size() < numDigits){
            retVal.push_back(0);
        }
        reverse(retVal.begin(), retVal.end());
        return retVal;
    }
    double unitStep(double X){
        double retValue;
    
        if (X < 0.0){ retValue = 0.0;}
        else		{ retValue = 1.0;}
    
        return retValue;
    }
  
    std::vector< std::vector<int>> matMult(std::vector< std::vector<int> > A, std::vector< std::vector<int> > B){
        std::vector< std::vector<int> > C(A.size(), std::vector<int>(B.size(), 0));

        int sum = 0;

        for(int i=0; i<A.size(); i++){
            for(int j=0; j<B[0].size(); j++){
                sum = 0;       
                for(int k=0; k<A[0].size(); k++){
                    sum = sum + A[i][k]*B[k][j];
                }
                C[i][j] = sum;
            }
        }
        return C;
    }

    
    std::string toLower(std::string str) {
        for (unsigned int i=0;i<str.length();i++){
            str[i] = tolower(str[i]);
        }
        return str;
    }
    std::string toUpper(std::string str) {
        for (unsigned int i=0;i<str.length();i++){
            str[i] = toupper(str[i]);
        }
        return str;
    }
    std::vector<std::string> permuteCharacters(std::string topermute){

        std::vector<char> iV;
        std::vector<std::string> results;
        std::string s;

        for(unsigned int x=0; x< topermute.length(); x++){
            iV.push_back(topermute[x]);
        }

        std::sort(iV.begin(), iV.end());
        s.assign(iV.begin(), iV.end());
        results.push_back(s);

        while (std::next_permutation(iV.begin(), iV.end())){
            s.assign(iV.begin(), iV.end());
            results.push_back(s);
        }
        return results;
    }

    double sigMoid(double v){
        return 1/(1+exp(-v));
    }

    void tokenizeString(std::string str,std::vector<std::string>& tokens, const std::string& delimiter = " "){

        int pos = 0;
        std::string token;

        for(;;){

            pos = str.find(delimiter);

            if(pos == (int)std::string::npos){
                tokens.push_back(str);
                break;
            }

            token = str.substr(0, pos);
            tokens.push_back(token);
            str = str.substr(pos+1);
        }
    }

    std::string changeBase(std::string Base, int number){
        std::stringstream ss;

        ss << Base << number;
        return ss.str();
    }

    int factorial(int n){
        int result=1;

        for (int i=1; i<=n; ++i){
            result=result*i;
        }

        return result;
    }

    // n Choose r
    int combination(int n, int r){
        return Utils::factorial(n)/(Utils::factorial(r) * Utils::factorial(n-r));
    }

    void loadSystemData(double& pLoad, double& qLoad, int& nb, int& nt, std::string curSystem,
                            std::vector<Generator>& gens, std::vector<Line>& lines, std::vector<Bus>& buses){
        //Clear Vectors
        gens.clear(); lines.clear(); buses.clear();

        std::ifstream myFile;
        int  	count = 0,
                numGens = 0;
        //myFile.open(("../Data/" + curSystem).c_str());
        myFile.open(curSystem);

        if (myFile.is_open()) {
            myFile >> numGens;
            myFile >> pLoad;
            myFile >> qLoad;
            myFile >> nb;
            myFile >> nt;

            std::vector<std::string> tokens;
            std::string s;

            // Generators
            for(int i = 0; i<numGens; i++){
                myFile >> s;
                Utils::tokenizeString(s,tokens,",");

                gens.push_back(Generator(
                        atof(tokens[3].c_str()),
                        atof(tokens[4].c_str()),
                        atof(tokens[5].c_str()),
                        atof(tokens[6].c_str()),
                        atof(tokens[7].c_str()),
                        atoi(tokens[0].c_str()))
                    );

                gens[i].setIndex(count);
                count++;
                tokens.clear();
            }

            // Transmission Lines
            for(int i = 0; i<nt; i++){
                myFile >> s;
                Utils::tokenizeString(s,tokens,",");

                lines.push_back(Line(
                    atoi(tokens[0].c_str()),
                    atoi(tokens[1].c_str()),
                    atoi(tokens[2].c_str()),
                    atof(tokens[3].c_str()),
                    atof(tokens[4].c_str()),
                    atof(tokens[5].c_str()),
                    atof(tokens[6].c_str()),
                    atof(tokens[7].c_str()),
                    atof(tokens[8].c_str()),
                    atof(tokens[9].c_str()),
                    atof(tokens[10].c_str()),
                    atof(tokens[11].c_str()),
                    atof(tokens[12].c_str()),
                    atof(tokens[13].c_str()))
                );
                tokens.clear();
            }

            for(int i=0; i<nb; i++){
                myFile >> s;
                Utils::tokenizeString(s,tokens,",");
                //id, type, area, zone, Pd, Qd, Gs, Bs, Vm, Va, vMin, vMax, baseKva

                buses.push_back(Bus(
                    atoi(tokens[0].c_str()),
                    atoi(tokens[1].c_str()),
                    atoi(tokens[6].c_str()),
                    atoi(tokens[10].c_str()),
                    atof(tokens[2].c_str()),
                    atof(tokens[3].c_str()),
                    atof(tokens[4].c_str()),
                    atof(tokens[5].c_str()),
                    atof(tokens[6].c_str()),
                    atof(tokens[7].c_str()),
                    atof(tokens[12].c_str()),
                    atof(tokens[11].c_str()),
                    atof(tokens[9].c_str()))
                );
                tokens.clear();
            }

            myFile.close();
        }
    }
    
    void getTimeStamp(char* aTime){
        time_t now;
        //char* aTime = new char[20];

        time(&now);
        struct tm * timeinfo = localtime(&now);;
        strftime(aTime, 20, "%m_%d_%Y_%H_%M_%S", timeinfo);

        //return aTime;
    }

    void twoByTwoCholeskyDecomp(std::vector<std::vector<double> >& A){

        std::vector < std::vector < double > > L(A.size(),std::vector < double > (A.size(),0));
        L[0][0] = sqrt(A[0][0]);
        L[0][1] = 0;
        L[1][0] = A[0][1]/L[0][0];
        L[1][1] = sqrt(A[1][1]-(L[1][0]*L[1][0]));
        A = L;
    }
   
    std::string getClassificationMethodString(pruning::CLASSIFICATION_METHOD cm){
        std::string retValue;
        switch(cm){
            case pruning::CM_OPF: retValue = "OPF"; break;
            case pruning::CM_CAP: retValue = "CAP";	break;
            default: retValue = "OPF";
        }
        return retValue;
    }
    std::string getSamplingMethodString(pruning::SAMPLING_METHOD ps){
        std::string retValue;
        switch(ps){
            default: retValue = "MCS";
        }
        return retValue;
    }
  
    pruning::CLASSIFICATION_METHOD getClassificationMethod(std::string s){
        pruning::CLASSIFICATION_METHOD retValue;
        s = toUpper(s);
        if(s == "OPF") {retValue = pruning::CM_OPF;}
        else if(s == "CAP") {retValue = pruning::CM_CAP;}
        else 				{retValue = pruning::CM_OPF;}
        return retValue;
    }
    pruning::SAMPLING_METHOD getSamplingMethod(std::string s){
        pruning::SAMPLING_METHOD retValue;
        s = toUpper(s);
        retValue = pruning::SM_MCS;
        return retValue;
    }

    void printVector(std::vector < std::vector < double > >& v, std::string title, int precision){
        std::cout << title << std::endl;
        for(int i=0; i<v.size(); i++){
            for(int j=0; j<v[i].size(); j++){
                std::cout << setprecision(precision) << v[i][j]<< " ";
            }
            std::cout << std::endl;
        }
    }
    void printVector(std::vector < std::vector < std::vector < double > > >& v, std::string title, int precision){
        std::cout << title << std::endl;
        for(int i=0; i<v.size(); i++){
            for(int j=0; j<v[i].size(); j++){
                for(int k=0; k<v[i][j].size(); k++){
                    std::cout << setprecision(precision) << v[i][j][k] << " ";
                }
            }
            std::cout << std::endl;
        }
    }

    void setUsage(AnyOption* opt){
        // General
        opt->addUsage( "" );
        opt->addUsage( "Usage: " );
        opt->addUsage( "" );
        opt->addUsage( "--help              Prints this help");
        opt->addUsage( "" );
       

        // Power System Specific
        opt->addUsage("--system            RTS79 or RTS96. Defaults to RTS79");
        opt->addUsage("--useLines          Use Transmission Lines");
        opt->addUsage("" );

        // Classification
        opt->addUsage("--classifier        CAP, or OPF. Defaults to OPF");
        opt->addUsage("" );
        
        // Sampling
        opt->addUsage("--sampler           MCS. Defaults to MCS.");
        opt->addUsage("--sigma             Tolerance for convergance");
        opt->addUsage("" );

        // Pruning
        opt->addUsage("--pruner            MCS. Defaults to MCS.");

    }
    void setOptions(AnyOption* opt){
        // Flags
        opt->setFlag("help", 'h');

        // Power System
        opt->setOption("system");
        opt->setOption("lineAdj");
        opt->setFlag("useLines");

        // Classifying
        opt->setOption("classifier");

        // Sampling
        opt->setOption("sampler");      
        opt->setOption("sigma");
      
    }
}
