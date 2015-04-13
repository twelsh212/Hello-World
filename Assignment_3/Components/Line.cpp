/*
 * Line.cpp
 *
 *  Created on: Aug 17, 2010
 *      Author: rgreen
 */

#include "Line.h"

Line::Line(int id, int from, int to, double r, double x, double b, double rateA, double rateB, double rateC, double Ratio, double Angle, double Lambda, double Mu, double outageRate){

	_ID 		= id;
	_from 		= from;
	_to 		= to;
	_r 			= r;
	_x 			= x;
	_b 			= b;
	_rateA 		= rateA;
	_rateB 		= rateB;
	_rateC 		= rateC;
	_angle 		= Angle;
	_lambda 	= Lambda;
	_mu 		= Mu;
	_outageRate = outageRate;
	_tapRatio	= Ratio;
}

Line::~Line() {}


int    Line::getID()				{ return _ID;}
int    Line::getFromBus()			{ return _from;}
int    Line::getToBus()				{ return _to;}
double Line::getR()					{ return _r;}
double Line::getX()					{ return _x;}
double Line::getB()					{ return _b;}
double Line::getRateA()				{ return _rateA;}
double Line::getRateB()				{ return _rateB;}
double Line::getRateC()				{ return _rateC;}
double Line::getTapRatio()			{ return _tapRatio;}
double Line::getAngle()				{ return _angle;}
double Line::getLambda()			{ return _lambda;}
double Line::getMu()				{ return _mu;}
double Line::getOutageRate()		{ return _outageRate;}

void Line::setFromBus(int i)		{ _from = i;}
void Line::setToBus(int i)			{ _to = i;}
void Line::setR(double i)			{ _r = i;}
void Line::setX(double i)			{ _x = i;}
void Line::setB(double i)			{ _b = i;}
void Line::setRateA(double i)		{ _rateA = i;}
void Line::setRateB(double i)		{ _rateB = i;}
void Line::setRateC(double i)		{ _rateC = i;}
void Line::setTapRatio(double i)		{ _tapRatio = i;}
void Line::setAngle(double i)		{ _angle = i;}
void Line::setLambda(double i)		{ _lambda = i;}
void Line::setMu(double i)			{ _mu = i;}
void Line::setOutageRate(double i)	{ _outageRate = i;}

