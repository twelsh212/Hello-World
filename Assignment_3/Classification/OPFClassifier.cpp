/*
 * OPF.cpp
 *
 *  Created on: Feb 5, 2010f
 *      Author: rgreen
 */

#include "OPFClassifier.h"
#include <iostream>

OPFClassifier::OPFClassifier() : Classifier() {
    initialInit();
}
OPFClassifier::OPFClassifier(std::string c, int n, std::vector < Generator > g, std::vector < Bus > b) : Classifier(c, n, g, b) {
    initialInit();
}
OPFClassifier::OPFClassifier( std::string c, int n, std::vector < Generator > g, std::vector < Bus > b, std::vector< Line > l) : Classifier (c, n, g, b, l){
    initialInit();
}

void OPFClassifier::initialInit(){
    lp 		  = NULL;
    loaded 	  = false;
    built 	  = false;
    numModels = 0;

    formulateOPF("");
}

OPFClassifier* OPFClassifier::clone() const {     
    OPFClassifier* retValue = new OPFClassifier(curSystem, nb, gens, buses, lines);
    retValue->init();
    return retValue;
}

OPFClassifier::~OPFClassifier(){
    delete_lp(lp);
}

void OPFClassifier::load(){ }

void OPFClassifier::init(){ }

void OPFClassifier::reset(){
    timer.startTimer();

    timer.stopTimer();
    resetTime += timer.getElapsedTime();
}

void OPFClassifier::calculateMatrices(std::string curSolution){
    
    Eigen::Matrix<double, Dynamic, Dynamic> bPosDiag, //nl x nl
                                            bNegDiag, // nl x nl
                                            Cf, Ct,   //nb x nl
                                            r1, r2,   //nb x nl
                                            CfTranspose, CtTranspose; // nl x nb
    Eigen::VectorXd 						b; // nl
    int nl;
    std::vector<Line> localLines;
    
    if(useLines && lines.size() > 0){
        for(unsigned int i=gens.size(); i<curSolution.size(); i++){
            if(curSolution[i] == '1'){
                localLines.push_back(lines[i-gens.size()]);
            }
        }
    }else{
        localLines = lines;
    }
    nl = localLines.size();
                                
    bPosDiag.resize(nl, nl); bNegDiag.resize(nl, nl),
    Cf.resize(nb, nl);		 Ct.resize(nb, nl),
    CfTranspose.resize(nl, nb);	CtTranspose.resize(nl, nb);
    b.resize(nl);		

    Bbus.resize(nb, nb); Bbus.fill(0); 
    Bf.resize(nl, nb);	 Bf.fill(0); 
    r1.fill(0);			 r2.fill(0);
    Cf.fill(0);			 Ct.fill(0);
    bPosDiag.fill(0);	 bNegDiag.fill(0);

    for(int i=0; i<nl; i++){

        b(i) = 1.0/localLines[i].getX();

        if(lines[i].getTapRatio() == 0) { b(i) = b(i)/1.0;}
        else						    { b(i) = b(i)/lines[i].getTapRatio();}

        bPosDiag(i,i) =  b(i);	
        bNegDiag(i,i) = -b(i);

        Cf(localLines[i].getFromBus()-1, i) = 1;
        Ct(localLines[i].getToBus()-1, i)   = 1;
    }
    CfTranspose = Cf.transpose();
    CtTranspose = Ct.transpose();

    // Bbus
    Bbus = Bbus + (Cf*bPosDiag)*CfTranspose;
    Bbus = Bbus + (Cf*bNegDiag)*CtTranspose;
    Bbus = Bbus + (Ct*bNegDiag)*CfTranspose;
    Bbus = Bbus + (Ct*bPosDiag)*CtTranspose;

    // Bf
    for(int i=0; i<nl; i++){ Bf(i, lines[i].getFromBus()-1) =  b(i); }
    for(int i=0; i<nl; i++){ Bf(i, lines[i].getToBus()-1)	= -b(i); }

    bPosDiag.resize(0, 0);      bNegDiag.resize(0, 0),
    Cf.resize(0, 0);		    Ct.resize(0, 0),
    CfTranspose.resize(0, 0);	CtTranspose.resize(0, 0);
    b.resize(nl);		
}

void OPFClassifier::updateOPF(std::string curSolution){
    std::vector<double> totalGenCap;
    int nl, nr, nc, curRow;
    double newValue;
    unsigned char result = 0;
    
    nl = 0;
    
    if(useLines && lines.size() > 0){
        for(unsigned int i=gens.size(); i<curSolution.size(); i++){
            if(curSolution[i] == '1'){
                nl++;
            }
        }
    }else{
        nl     = lines.size();
    }
        
    nr     = 3*nb + 2*nl;
    nc     = 3*nb;    
    curRow = 1;
    
    for(int i=0; i<nb; i++){
        newValue = -buses[i].getPd()/100.0 - addedLoad[i];
        result  += set_rh (lp, curRow, newValue);
        if(useLines && lines.size() > 0){
            for(int j=0; j<nb; j++)  {    
                result += set_mat(lp, curRow, j+1, Bbus(i,j));
            }
        }
        curRow++;
    }
    
    // Generation Constraints
    totalGenCap.resize(nb, 0);
    for(int i=0; i<(int)gens.size(); i++){
        if(curSolution[i] == '1'){ totalGenCap[gens[i].getBus()-1] += gens[i].getPG();}
    }
 
    for(int i=0; i<nb; i++){
        result += set_rh(lp, curRow++, totalGenCap[i]/100.0);
    }

    totalGenCap.clear();
}

void OPFClassifier::buildOPF(std::string curSolution){
    int nl, nr, nc, curRow;
    std::vector<double> totalGenCap;
    std::string name;
    REAL *row;
    std::vector<Line> localLines;
    unsigned char result = 0;

    if(useLines && lines.size() > 0){
        for(unsigned int i=gens.size(); i<curSolution.size(); i++){
            if(curSolution[i] == '1'){
                localLines.push_back(lines[i-gens.size()]);
            }
        }
    }else{
        localLines = lines;
    }
    nl = localLines.size();
    nr = 3*nb + 2*nl;
    nc = 3*nb;
    row = new REAL[nc+1];

    if(lp != NULL){ delete_lp(lp); }
    
    lp = make_lp(0, nc);
    
    //Set Objective Function
    for(int j=0; j<nc+1; j++) 		{ row[j] = 0.0;}
    for(int i=nb*2+1; i<nc+1; i++)	{ row[i] = 1;}
    result += set_obj_fn(lp, row);

    curRow=1;
    set_add_rowmode(lp, TRUE);

    // Flow Constraints
    for(int i=0; i<nb; i++){
        for(int j=0; j<nc+1; j++){ row[j] = 0.0; }
        for(int j=0; j<nb; j++)  { row[j+1] = Bbus(i, j);}

        row[nb+i+1]   = -1; // G Variable
        row[nb*2+i+1] = -1; // C Variable

        //result += add_constraint(lp, row, EQ, -buses[i].getPd()/100.0); // Add Bus load
        result += add_constraint(lp, row, EQ, -buses[i].getPd()/100.0 - addedLoad[i]); // Add Bus load
        curRow++;
    }

    // Generation Constraints
    totalGenCap.resize(nb, 0);
    for(int i=0; i<(int)gens.size(); i++){
        if(curSolution[i] == '1'){ totalGenCap[gens[i].getBus()-1] += gens[i].getPG();}
    }
    for(int i=0; i<nb; i++){
        for(int j=0; j<nc+1; j++) { row[j] = 0.0;}
        row[nb+i+1] = 1.0;
        result += add_constraint(lp, row, LE, totalGenCap[i]/100.0); // Add Bus load
        curRow++;
    }
    // Curtailment Constraints
    for(int i=0; i<nb; i++){
        for(int j=0; j<nc+1; j++){ row[j] = 0.0;}
        row[nb*2+i+1] = 1; // C Variable
        result += add_constraint(lp, row, LE, buses[i].getPd()/100.0); // Add Bus load
        curRow++;
    }
    // Flow Constraints
    for(int i=0; i<nl; i++){
        for(int j=0; j<nc+1; j++){ row[j] = 0.0;}
        for(int j=0; j<nb; j++)	 { row[j+1] = Bf(i,j);}

        result += add_constraint(lp, row, LE, localLines[i].getRateA()/100.0);
        curRow++;
    }
    for(int i=0; i<nl; i++){
        for(int j=0; j<nc+1; j++){ row[j] = 0.0;}
        for(int j=0; j<nb; j++)	 { row[j+1] = -Bf(i,j); }

        result += add_constraint(lp, row, LE, localLines[i].getRateA()/100.0);
        curRow++;
    }
    // Bounds
    for(int i=0; i<nb; i++){
        for(int j=0; j<nc+1; j++){ row[j] = 0.0;}
        row[nb+i+1] = 1; // G Variable
        result += add_constraint(lp, row, GE, 0.0); // Add Bus load
        curRow++;
    }
    for(int i=0; i<nb; i++){
        for(int j=0; j<nc+1; j++){ row[j] = 0.0;}
        row[nb*2+i+1] = 1; // C Variable
        result += add_constraint(lp, row, GE, 0.0); // Add Bus load
        curRow++;
    }
    set_add_rowmode(lp, FALSE);

    /////////////////////////////////////////////////////////////////////////////////////////////
    // Name Rows/columns
    for(int i=0; i<nb; i++){
        name = Utils::changeBase("T", i+1);
        result += set_col_name(lp, i+1, (char*)(name.c_str()));
    }
    for(int i=nb; i<nb*2; i++){
        name = Utils::changeBase("G",i-nb+1);
        result += set_col_name(lp, i+1, (char*)(name.c_str()));
    }
    for(int i=nb*2; i<nc; i++){
        name = Utils::changeBase("C",i-nb*2+1);
        result += set_col_name(lp, i+1, (char*)(name.c_str()));
    }
    
    for(int i=0; i<nb; i++){
        name = Utils::changeBase("T", i+1);
        result += set_col_name(lp, i+1, (char*)(name.c_str()));
    }
    for(int i=nb; i<nb*2; i++){
        name = Utils::changeBase("G",i-nb+1);
        result += set_col_name(lp, i+1, (char*)(name.c_str()));
    }
    for(int i=nb*2; i<nc; i++){
        name = Utils::changeBase("C",i-nb*2+1);
        result += set_col_name(lp, i+1, (char*)(name.c_str()));
    }

    curRow=1;
    // Flow Constraints
    for(int i=0; i<nb; i++){
        name = Utils::changeBase("LOAD_",i+1);
        result += set_row_name(lp, curRow++, const_cast<char*>(name.c_str()));
    }

    // Generation Constraints
    for(int i=0; i<nb; i++){
        name = Utils::changeBase("G_",i+1);
        result += set_row_name(lp, curRow++, const_cast<char*>(name.c_str()));
    }

    // Curtailment Constraints
    for(int i=0; i<nb; i++){
        name = Utils::changeBase("CURTAIL_",i+1);
        result += set_row_name(lp, curRow++, const_cast<char*>(name.c_str()));
    }

    // Flow Constraints
    for(int i=0; i<nl; i++){
        name = Utils::changeBase("F1_",i+1);
        result += set_row_name(lp, curRow++, const_cast<char*>(name.c_str()));
    }
    for(int i=0; i<nl; i++){
        name = Utils::changeBase("F2_",i+1);
        result +=  set_row_name(lp, curRow++, const_cast<char*>(name.c_str()));
    }

    delete [] row;
}

void OPFClassifier::formulateOPF(std::string curSolution){

    if(curSolution == ""){
        for(unsigned int i=0; i<gens.size(); i++){
            curSolution += '1';
        }
        if(useLines && lines.size() > 0){
            for(unsigned int i=0; i<lines.size(); i++){
                curSolution += '1';
            }
        }
    }
    timer.startTimer();
    if(useLines || Bbus.size() == 0 || Bf.size() == 0){	
        calculateMatrices(curSolution); 
    }
    
    if(lp == NULL)	{ buildOPF(curSolution); 
    }else			{ updateOPF(curSolution); }

    timer.stopTimer();
    loadTime += timer.getElapsedTime();
    set_verbose(lp, NEUTRAL);
}

double OPFClassifier::run(std::vector<int> curSolution, double& excess){
    std::string newSol;
    for(unsigned int i=0; i<gens.size(); i++){
        if(curSolution[i] == 1){ newSol += "1";}
        else				   { newSol += "0";}
    }

    if(useLines && lines.size() > 0){
        for(unsigned int i=0; i<lines.size(); i++){
            if(curSolution[gens.size()+i] == 1){ newSol += "1";}
            else 							   { newSol += "0";}
        }
    }else{
        for(unsigned int i=0; i<lines.size(); i++){ newSol += "1";}
    }
    return run(newSol, excess);
}

double OPFClassifier::run(std::vector<double> curSolution, double& excess){
    std::string newSol;
    for(unsigned int i=0; i<gens.size(); i++){
        if(curSolution[i] == 1){ newSol += "1";}
        else				   { newSol += "0";}
    }
    if(useLines && lines.size() > 0){
        for(unsigned int i=0; i<lines.size(); i++){
            if(curSolution[gens.size()+i] == 1){ newSol += "1";}
            else							   { newSol += "0";}
        }
    }else{
        for(unsigned int i=0; i<lines.size(); i++){ newSol += "1"; }
    }
    return run(newSol, excess);
}

double OPFClassifier::run(std::string curSolution, double& excess){
    int result;
    double used, cap, ret;
    std::string name;

    excess = 0;	used = 0; cap = 0;

    timer.startTimer();
    formulateOPF(curSolution);
    result = solve(lp);
    ret    = get_objective(lp);

    // Catches a silly lp_solve problem that 
    // causes NUMFAILURES and INFEASIBILITY
    if(result != 0 && result != 1 && result != 9){
        result = solve(lp);
        ret    = get_objective(lp);
    }
    if(result == 0 || result == 1 || result == 9){ // OPTIMAL, SUBOPTIMAL, PRESOLVED
        int curIndex;
        double *var;

        var = new double[get_Ncolumns(lp)];
        get_variables(lp, var);

        // For Each Bus
        if(ret > 0){ excess = 0;}
        else{
            for(int x=0; x<nb; x++){

                //Get Name, Index, and Amount of capacity used
                name 	 = Utils::changeBase("G",x+1);
                curIndex = get_nameindex(lp, const_cast<char*>(name.c_str()), FALSE);
                used 	 = var[curIndex-1];

                //For each generator at Bus x+1, add up capacity
                cap = 0;
                for(unsigned i=0; i<gens.size(); i++){
                    if(gens[i].getBus() == x+1){ cap += gens[i].getPG()/100;}
                }
                excess += (cap - used);
            }
        }

        delete var;
        timer.stopTimer();
        classificationTime += timer.getElapsedTime();
    }else{
        switch(result){
            case -2: std::cout << "No Memory\n";         break;
            case 0:  std::cout << "Optimal\n";           break;
            case 1:	 std::cout << "Suboptimal\n";	     break;
            case 2:  std::cout << "Infeasible\n";	     break;
            case 3:  std::cout << "Unbounded\n";		 break;
            case 4:	 std::cout << "Degenerate\n";		 break;
            case 5:	 std::cout << "NumFailure\n";		 break;
            case 6:	 std::cout << "UserAbort\n";		 break;
            case 7:	 std::cout << "Timeout\n";		 break;
            case 9:  std::cout << "Presolved\n";		 break;
            case 10: std::cout << "ProcFail\n";			 break;
            case 11: std::cout << "ProcBreak\n";         break;
            case 12: std::cout << "Feasible Found\n";	 break;
            case 13: std::cout << "No Feasible Found\n"; break;
        }

        ret = 1000;
    }
    timer.stopTimer();
    classificationTime += timer.getElapsedTime();

    return ret;
}

lprec* OPFClassifier::getLP(){
    return lp;
}