// OpenGLHeaders.h
// Dominicus

#ifndef OPENGLHEADERS_H
#define OPENGLHEADERS_H

#include "platform/Platform.h"

#if defined(PROGRAM_ARCH_MACOSX)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#elif defined(PROGRAM_ARCH_WINNT)
#include <Windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#else
#include <GL/gl.h>
#endif

#endif // OPENGLHEADERS_H
