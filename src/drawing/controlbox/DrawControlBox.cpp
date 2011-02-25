/*
 * DrawControlBox.cpp
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "drawing/controlbox/DrawControlBox.h"

DrawControlBox::DrawControlBox() {
	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform->dataPath +  "/shaders/controlBox.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform->dataPath + "/shaders/controlBox.fragment.glsl").c_str(),
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
	spotSizeUniform = glGetUniformLocation(program, "spotSize");
}

void DrawControlBox::draw() {
	// calculate the geomtery values
	float totalControlBoxSize = gamePrefs->getFloat("controlBoxSize");
	float spotSize = gamePrefs->getFloat("controlSpotSize") / totalControlBoxSize;

	Vector2 dims(
			totalControlBoxSize * 2.0f / (float) gameWindow->aspectRatio,
			totalControlBoxSize * 2.0f
		);
	Vector2 middle(0.0f, 0.0f);

	Vector2 ll(middle.x - dims.x / 2.0f, middle.y - dims.y / 2.0f);
	Vector2 ur(middle.x + dims.x / 2.0f, middle.y + dims.y / 2.0f);

	glUseProgram(program);

	glUniform1f(spotSizeUniform, spotSize);

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
