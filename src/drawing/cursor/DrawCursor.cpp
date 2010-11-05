/*
 *  DrawCursor.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DrawCursor.h"

DrawCursor::DrawCursor(Screen* screen) : screen(screen) {
	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/cursor.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/cursor.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	program = ShaderTools::makeProgram(shaders);

	// get attribute locations
	positionAttrib = glGetAttribLocation(program, "position");
	boxCoordAttrib = glGetAttribLocation(program, "boxCoord");

	// get uniform locations
	thicknessUniform = glGetUniformLocation(program, "thickness");
}

void DrawCursor::draw() {
	// calculate the cursor values
	Vector2 dims(
			gamePrefs.getFloat("cursorSize") * 2.0f / (float) screen->aspectRatio,
			gamePrefs.getFloat("cursorSize") * 2.0f
		);
	Vector2 middle(
			-1.0f + (float) mouse.positionX / (float) screen->width * 2.0f,
			1.0f - (float) mouse.positionY / (float) screen->height * 2.0f
		);
	Vector2 ll(middle.x - dims.x / 2.0f, middle.y - dims.y / 2.0f);
	Vector2 ur(middle.x + dims.x / 2.0f, middle.y + dims.y / 2.0f);
//printf("dims %+.2f, %+.2f middle %+.2f, %+.2f ll %+.2f, %+.2f ur %+.2f, %+.2f\n", dims.x, dims.y, middle.x, middle.y, ll.x, ll.y, ur.x, ur.y);

	glUseProgram(program);

	glUniform1f(thicknessUniform, gamePrefs.getFloat("cursorThickness"));

	glBegin(GL_QUADS);

	glVertexAttrib2f(boxCoordAttrib, -1.0f, -1.0f);
	glVertexAttrib3f(positionAttrib, ll.x, ll.y, -0.9f);

	glVertexAttrib2f(boxCoordAttrib, -1.0f, 1.0f);
	glVertexAttrib3f(positionAttrib, ll.x, ur.y, -0.9f);

	glVertexAttrib2f(boxCoordAttrib, 1.0f, 1.0f);
	glVertexAttrib3f(positionAttrib, ur.x, ur.y, -0.9f);

	glVertexAttrib2f(boxCoordAttrib, 1.0f, -1.0f);
	glVertexAttrib3f(positionAttrib, ur.x, ll.y, -0.9f);

	glEnd();
}
