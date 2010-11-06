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
#include "ProgramLog.h"

// library headers
#include <OpenGL/OpenGL.h>
#include <SDL/SDL.h>
#include <sstream>

class SystemInfo {
public:
	unsigned short int screenWidth, screenHeight;

	void init();

	static void check();
};

#endif
