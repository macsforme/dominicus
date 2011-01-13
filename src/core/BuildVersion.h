/*
 *  BuildVersion.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 1/9/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#ifndef BUILDVERSION_H
#define BUILDVERSION_H

// Major version (positive integer)
#define BV_VERSION 0

// Minor revision (positive integer)
#define BV_REVISION 1

// Build classification (char array, DEVEL | ALPHA | BETA | RC# | STABLE)
#define BV_CLASSIFICATION "DEVEL"

// program headers
#include "Platform.h"
#include "ProgramLog.h"

// library headers
#include <sstream>
#include <stdio.h>
#include <string>
#include <string.h>

// global variables
extern Platform platform;
extern ProgramLog programLog;

class BuildVersion {
public:
	std::string version;
	std::string classification;
	std::string architecture;
	std::string buildDate;

	BuildVersion();
};

#endif
