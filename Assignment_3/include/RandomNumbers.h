/*
 * RandomNumbers.h
 *
 *  Created on: Aug 2, 2011
 *      Author: Rob
 */

#ifndef RANDOMNUMBERS_H_
#define RANDOMNUMBERS_H_

#include "MTRand.h"

namespace RandomNumbers {

	extern void bivariateGaussian(MTRand mt, double sigma_x, double sigma_y, double rho, double& x, double& y);
	extern double logNormal(MTRand mt, const double zeta, const double sigma);

};

#endif /* RANDOMNUMBERS_H_ */
