/*
 *  OpenGLHeaders.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/27/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

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
