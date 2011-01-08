/*
 *  SystemInfo.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 8/15/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

// program headers
#include "OpenGLHeaders.h"
#include "ProgramLog.h"

// library headers
#include <SDL/SDL.h>
#include <sstream>

class SystemInfo {
public:
	unsigned short int screenWidth, screenHeight;

	void init();
	void check();
};

#endif
