/*
 *  Platform.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef PLATFORM_H
#define PLATFORM_H

// program headers
#include "ProgramLog.h"

// library headers
#include <map>
#include <stdint.h>
#include <string>

class Platform {
public:
	std::string dataPath;

	Platform();

	// drawing functions
	void hideCursor();

	// time functions
	unsigned int getExecutionTimeMicros();
	void sleepMicros(unsigned int micros);
};

#endif

