// OpenGLHeaders.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

// this file includes the correct OpenGL headers for the build platform

#ifndef OPENGLHEADERS_H
#define OPENGLHEADERS_H

#include "platform/Platform.h"

#if defined(PROGRAM_ARCH_MACOSX)
#include <OpenGL/OpenGL.h>
#elif defined(PROGRAM_ARCH_WINNT)
#include <Windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#else
#include <GL/gl.h>
#endif

#endif
