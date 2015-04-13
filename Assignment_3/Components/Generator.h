/*
 * Generator.h
 *
 *  Created on: Sep 25, 2009
 *      Author: Rob
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

class Generator {

    public:

        Generator();
        //Generator(const Generator& g);
        Generator(double outageRate, double PG, double QG, double Qmax, double Qmin, int bus);
        virtual ~Generator();

        double getOutageRate();
        double getPG();
        double getQG();
        double getQmax();
        double getQmin();
        int getIndex();
        int getBus();

        void setOutageRate(double);
        void setPG(double);
        void setQG(double);
        void setQmax(double);
        void setQmin(double);
        void setIndex(int);

        bool operator==(Generator object2);
        bool operator!=(Generator object2);

    private:
        int    _bus;
        double _outageRate;
        double _PG;
        double _QG;
        double _Qmax;
        double _Qmin;
        int _index;
};

#endif /* GENERATOR_H_ */

