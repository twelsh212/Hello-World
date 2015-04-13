/*
 * MCS_Sampler.h
 *
 *  Created on: Feb 5, 2010
 *      Author: rgreen
 */

#ifndef OMP_SAMPLER_H_
#define OMP_SAMPLER_H_

#include <math.h>
#include <map>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "Classifier.h"
#include "defs.h"
#include "Generator.h"
#include "CStopWatch.h"
#include "Line.h"
#include "MTRand.h"
#include "OPFClassifier.h"
#include "Sampler.h"

class OMP_Sampler : public Sampler{
	public:
		OMP_Sampler();
		virtual ~OMP_Sampler();
		virtual void run(MTRand& mt);

};

#endif /* OMP_SAMPLER_H_ */
