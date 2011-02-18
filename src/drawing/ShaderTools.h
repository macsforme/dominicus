/*
 *  ShaderTools.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 6/2/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef SHADERTOOLS_H
#define SHADERTOOLS_H

// program headers
#include "core/ProgramLog.h"
#include "platform/OpenGLHeaders.h"

// library headers
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>

class ShaderTools {
public:
	static GLuint makeShader(const char* filename, GLenum shaderType);
	static void linkProgram(GLuint program);
	static GLuint makeProgram(std::vector<GLuint> shaders);
};

#endif
