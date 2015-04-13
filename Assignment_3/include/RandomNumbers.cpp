/*
 * RandomNumbers.cpp
 *
 *  Created on: Aug 2, 2011
 *      Author: Rob
 */

#include "RandomNumbers.h"

namespace RandomNumbers{
	void bivariateGaussian(MTRand mt, double sigma_x, double sigma_y, double rho, double& x, double& y){
		double u, v, r2, scale;

		do{
			/* choose x,y in uniform square (-1,-1) to (+1,+1) */
			u = -1 + 2 * mt.randDblExc();
			v = -1 + 2 * mt.randDblExc();

			/* see if it is in the unit circle */
			r2 = u * u + v * v;
		}while (r2 > 1.0 || r2 == 0);

		scale = sqrt(-2.0 * log(r2)/r2);

		x = sigma_x * u * scale;
		y = sigma_y * (rho * u + sqrt(1 - rho*rho) * v) * scale;
	}

	double logNormal(MTRand mt, const double zeta, const double sigma){
		double u, v, r2, normal, z;

		do{
			/* choose x,y in uniform square (-1,-1) to (+1,+1) */
			u = -1 + 2 * mt.randDblExc();
			v = -1 + 2 * mt.randDblExc();

			/* see if it is in the unit circle */
			r2 = u * u + v * v;
	    } while (r2 > 1.0 || r2 == 0);

		normal = u * sqrt (-2.0 * log (r2) / r2);
		z =  exp (sigma * normal + zeta);
		return z;
	}
}
