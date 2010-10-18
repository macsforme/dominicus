/*
 *  RenderingMaster.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 7/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef RENDERINGSETUP_H
#define RENDERINGSETUP_H

// game headers
#include "GamePrefs.h"
#include "Platform.h"
#include "ProgramLog.h"
#include "SystemInfo.h"

// library headers
#include <sstream>
#include <SDL/SDL.h>
#include <OpenGL/OpenGL.h>

// global variables
extern SystemInfo systemInfo;
extern GamePrefs gamePrefs;
extern Platform platform;

class RenderingMaster {
public:
	RenderingMaster();
	~RenderingMaster();

	void setViewPortSize(unsigned short int width, unsigned short int height);

	void preFrame();
	void postFrame();

	unsigned long int loop();
};

#endif


