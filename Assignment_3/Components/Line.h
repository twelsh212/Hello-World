/*
 * Line.h
 *
 *  Created on: Aug 17, 2010
 *      Author: rgreen
 */

#ifndef LINE_H_
#define LINE_H_

class Line {
	public:
		Line(int id, int from, int to, double r, double x, double b, double rateA, double rateB, double rateC, double Ratio, double Angle, double Lambda, double Mu, double outageRate);
		virtual ~Line();

		int getID();

		int getFromBus();
		int getToBus();
		double getR();
		double getX();
		double getB();
		double getRateA();
		double getRateB();
		double getRateC();
		double getTapRatio();
		double getAngle();
		double getLambda();
		double getMu();
		double getOutageRate();

		void setFromBus(int i);
		void setToBus(int i);
		void setR(double i);
		void setX(double i);
		void setB(double i);
		void setRateA(double i);
		void setRateB(double i);
		void setRateC(double i);
		void setTapRatio(double i);
		void setAngle(double i);
		void setLambda(double i);
		void setMu(double i);
		void setOutageRate(double i);

	private:
		int _ID;
		int _from;
		int _to;
		double _r;
		double _x;
		double _b;
		double _rateA;
		double _rateB;
		double _rateC;
		double _tapRatio;
		double _angle;
		double _lambda;
		double _mu;
		double _outageRate;
};

#endif /* LINE_H_ */
