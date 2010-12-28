/*
 *  DrawControlBox.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 12/26/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DrawControlBox.h"

DrawControlBox::DrawControlBox(GameWindow* gameWindow) : gameWindow(gameWindow) {
	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/controlBox.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/controlBox.fragment.glsl").c_str(),
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
	borderSizeUniform = glGetUniformLocation(program, "borderSize");
}

void DrawControlBox::draw() {
	// calculate the geomtery values
	float borderHeight = (float) gamePrefs.getInt("controlBoxBorderPixels") * 2.0f / gameWindow->height;
	float totalControlBoxSize =
			gamePrefs.getFloat("controlBoxSize") + borderHeight * 2.0f;

	Vector2 dims(
			totalControlBoxSize * 2.0f / (float) gameWindow->aspectRatio,
			totalControlBoxSize * 2.0f
		);
	Vector2 middle(0.0f, 0.0f);

	Vector2 ll(middle.x - dims.x / 2.0f, middle.y - dims.y / 2.0f);
	Vector2 ur(middle.x + dims.x / 2.0f, middle.y + dims.y / 2.0f);

	glUseProgram(program);

	glUniform1f(borderSizeUniform, borderHeight / totalControlBoxSize);

	glBegin(GL_QUADS);

	glVertexAttrib2f(boxCoordAttrib, -1.0f, -1.0f);
	glVertexAttrib3f(positionAttrib, ll.x, ll.y, -0.5f);

	glVertexAttrib2f(boxCoordAttrib, -1.0f, 1.0f);
	glVertexAttrib3f(positionAttrib, ll.x, ur.y, -0.5f);

	glVertexAttrib2f(boxCoordAttrib, 1.0f, 1.0f);
	glVertexAttrib3f(positionAttrib, ur.x, ur.y, -0.5f);

	glVertexAttrib2f(boxCoordAttrib, 1.0f, -1.0f);
	glVertexAttrib3f(positionAttrib, ur.x, ll.y, -0.5f);

	glEnd();
}
