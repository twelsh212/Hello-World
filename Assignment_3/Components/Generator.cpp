/*
 * Generator.cpp
 *
 *  Created on: Sep 25, 2009
 *      Author: Rob
 */

#include "Generator.h"

Generator::Generator() {
	// TODO Auto-generated constructor stub
}

Generator::Generator(double outageRate, double PG, double QG, double Qmax, double Qmin, int bus) {
	_outageRate = outageRate;
	_PG = PG;
	_QG = QG;
	_Qmax = Qmax;
	_Qmin = Qmin;
	_bus = bus;
}

Generator::~Generator() {
	_outageRate = 0.01;
	_PG = 50;
	_QG = -4.96;
	_Qmax = 16;
	_Qmin = -10;
}

double Generator::getOutageRate(){
	return _outageRate;
}

double Generator::getPG(){
	return _PG;
}

double Generator::getQG(){
	return _QG;
}

double Generator::getQmax(){
	return _Qmax;
}

double Generator::getQmin(){
	return _Qmin;
}

int Generator::getIndex(){
	return _index;
}

int Generator::getBus(){
	return _bus;
}

void Generator::setOutageRate(double d){
	_outageRate = d;
}

void Generator::setPG(double d){
	_PG = d;
}

void Generator::setQG(double d){
	_QG = d;
}

void Generator::setQmax(double d){
	_Qmax = d;
}

void Generator::setQmin(double d){
	_Qmin = d;
}

void Generator::setIndex(int i){
	_index = i;
}

bool Generator::operator==(Generator object2){

  return (_bus       == object2.getBus() &&
		 _outageRate == object2.getOutageRate() &&
		 _PG         == object2.getPG() &&
		 _QG         == object2.getQG() &&
		 _Qmax       == object2.getQmax() &&
		 _Qmin       == object2.getQmin());
}

bool Generator::operator!=(Generator object2){

  return (_bus       != object2.getBus()        ||
		 _outageRate != object2.getOutageRate() ||
		 _PG         != object2.getPG()         ||
		 _QG         != object2.getQG()         ||
		 _Qmax       != object2.getQmax()       ||
		 _Qmin       != object2.getQmin());
}

