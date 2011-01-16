/*
 *  DrawCursor.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DrawCursor.h"

DrawCursor::DrawCursor() {
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

	// set attribute locations
	positionAttrib = 0;
	boxCoordAttrib = 1;
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "boxCoord");

	ShaderTools::linkProgram(program);

	// get uniform locations
	thicknessUniform = glGetUniformLocation(program, "thickness");
}

void DrawCursor::draw() {
	// calculate the cursor values
	Vector2 dims(
			gamePrefs.getFloat("cursorSize") * 2.0f / (float) gameWindow->aspectRatio,
			gamePrefs.getFloat("cursorSize") * 2.0f
		);
	Vector2 middle = inputHandler->mouse.position;

	Vector2 ll(middle.x - dims.x / 2.0f, middle.y - dims.y / 2.0f);
	Vector2 ur(middle.x + dims.x / 2.0f, middle.y + dims.y / 2.0f);

	// cursor thickness is a minimum of 2 pixels
	float thickness = gamePrefs.getFloat("cursorThickness");
	if(thickness * dims.y / 2.0f * (float) gameWindow->height < 1.0f)
		thickness = 2.0f / (dims.y / 2.0f * (float) gameWindow->height);

	glUseProgram(program);

	glUniform1f(thicknessUniform, thickness);

	glBegin(GL_QUADS);

	glVertexAttrib2f(boxCoordAttrib, -1.0f, -1.0f);
	glVertexAttrib3f(positionAttrib, ll.x, ll.y, -0.99f);

	glVertexAttrib2f(boxCoordAttrib, -1.0f, 1.0f);
	glVertexAttrib3f(positionAttrib, ll.x, ur.y, -0.99f);

	glVertexAttrib2f(boxCoordAttrib, 1.0f, 1.0f);
	glVertexAttrib3f(positionAttrib, ur.x, ur.y, -0.99f);

	glVertexAttrib2f(boxCoordAttrib, 1.0f, -1.0f);
	glVertexAttrib3f(positionAttrib, ur.x, ll.y, -0.99f);

	glEnd();
}
