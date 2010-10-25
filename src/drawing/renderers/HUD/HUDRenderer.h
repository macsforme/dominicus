/*
 *  HUDRenderer.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 8/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef HUDRENDERER_H
#define HUDRENDERER_H

#define CONSOLE_BORDER 20.0f

// program headers
#include "FontManager.h"
#include "Platform.h"
#include "Preferences.h"
#include "ShaderTools.h"
#include "SubjectRenderer.h"
#include "VectorMath.h"

// library headers
#include <OpenGL/OpenGL.h>
#include <string>
#include <vector>

class HUDRenderer {
private:
	FontManager* fontManager;

	SubjectRenderer* subjectRenderer;

	unsigned int width, height;

	// text shader
	GLuint textProgram, textVertexShader, textFragmentShader;
	GLuint textPositionAttrib, textTexCoordAttrib;
	GLuint textTextureUniform;

	// hud shader
	GLuint hudProgram, hudVertexShader, hudFragmentShader;
	GLuint hudPositionAttrib, hudColorAttrib;

	// fps tracking
	unsigned long int lastCheck;
	unsigned int frameCount;
	unsigned int currentFPS;

	float normalize(unsigned int coordinate, unsigned int scale) { return (float) coordinate / ((float) scale / 2) - 1.0f; }
	Vector2 stringMaxAdvances(std::string str);
	Vector2 stringAdvances(std::string str);

public:
	HUDRenderer(unsigned int newWidth, unsigned int newHeight, SubjectRenderer* newSubjectRenderer);
	~HUDRenderer();

	void render();
};

#endif
