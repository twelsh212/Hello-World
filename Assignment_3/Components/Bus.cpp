/*
 * Bus.cpp
 *
 *  Created on: Jun 27, 2011
 *      Author: Rob
 */

#include "Bus.h"

Bus::Bus() {
	// TODO Auto-generated constructor stub

}

Bus::Bus(int busID, int type, int area, int zone, double Pd, double Qd, double Gs, double Bs, double vM, double vA, double vMin, double vMax, double baseKV){
	_busID 	= busID; _type	= type;
	_area	= area;  _zone 	= zone;
	_Pd 	= Pd; 	 _Qd 	= Qd;
	_Gs		= Gs; 	 _Bs	= Bs;
	_Vm 	= vM; 	 _Va 	= vA;
	_vMin 	= vMin;  _vMax 	= vMax;
	_baseKV = baseKV;
}
Bus::~Bus(){}

int    Bus::getBusID()	{ return _busID;}
int    Bus::getType()	{ return _type;}
int    Bus::getArea()	{ return _area;}
int    Bus::getZone()	{ return _zone;}
double Bus::getPd()   	{ return _Pd;}
double Bus::getQd()		{ return _Qd;}
double Bus::getGs()   	{ return _Gs;}
double Bus::getBs()		{ return _Bs;}
double Bus::getVm()  	{ return _Vm;}
double Bus::getVa()		{ return _Va;}
double Bus::getVmin() 	{ return _vMin;}
double Bus::getVmax()	{ return _vMax;}
double Bus::getBaseKV()	{ return _baseKV;}

void Bus::setBusID(int busID)		{_busID=busID;}
void Bus::setType(int type)			{_type=type;}
void Bus::setArea(int area)			{_area=area;}
void Bus::setZone(int zone)			{_zone=zone;}
void Bus::setPD(double Pd)			{_Pd=Pd;}
void Bus::setQD(double Qd)			{_Qd=Qd;}
void Bus::setGs(double Gs)			{_Gs=Gs;}
void Bus::setBs(double Bs)			{_Bs=Bs;}
void Bus::setVm(double Vm)			{_Vm=Vm;}
void Bus::setVa(double Va)			{_Va=Va;}
void Bus::setVmin(double vMin)		{_vMin=vMin;}
void Bus::setVmax(double vMax)		{_vMax=vMax;}
void Bus::setBaseKV(double baseKV)	{_baseKV=baseKV;}
