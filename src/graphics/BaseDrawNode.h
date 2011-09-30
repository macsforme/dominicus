// BaseDrawNode.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef BASEDRAWNODE_H
#define BASEDRAWNODE_H

// program headers
#include "platform/OpenGLHeaders.h"

// library headers
#include <map>
#include <string>

class BaseDrawNode {
protected:
	GLuint shaderProgram;
	std::map<std::string, GLint> uniforms;
	std::map<std::string, GLint> attributes;
	std::map<std::string, GLuint> vertexBuffers;

public:
	virtual void execute(std::map<std::string, void*> arguments) = 0;
};

#endif
