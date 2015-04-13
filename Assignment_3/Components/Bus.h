/*
 * Bus.h
 *
 *  Created on: Jun 27, 2011
 *      Author: Rob
 */

#ifndef BUS_H_
#define BUS_H_

class Bus {
	public:
		Bus();
		Bus(int busID, int type, int area, int zone, double Pd, double Qd, double Gs, double Bs, double vM, double vA, double vMin, double vMax, double baseKV);
		virtual ~Bus();

		int getBusID();	  int getType();
		int getArea();    int getZone();
		double getPd();   double getQd();
		double getGs();   double getBs();
		double getVm();   double getVa();
		double getVmin(); double getVmax();
		double getBaseKV();

		void setBusID(int busID);	void setType(int type);
		void setArea(int area);    	void setZone(int zone);
		void setPD(double Pd);   	void setQD(double Qd);
		void setGs(double Gs);   	void setBs(double Bs);
		void setVm(double Vm);   	void setVa(double Va);
		void setVmin(double vMin); 	void setVmax(double vMax);
		void setBaseKV(double baseKV);


	private:
		int _busID,   _type;
		int _area,    _zone;
		double _Pd,   _Qd;
		double _Gs,   _Bs;
		double _Vm,   _Va;
		double _vMax, _vMin;
		double _baseKV;

};

#endif /* BUS_H_ */
