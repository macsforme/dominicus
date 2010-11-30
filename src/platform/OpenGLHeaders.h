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

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#endif
