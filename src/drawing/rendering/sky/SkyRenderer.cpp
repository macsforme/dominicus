/*
 *  SkyRenderer.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "drawing/rendering/sky/SkyRenderer.h"

SkyRenderer::SkyRenderer() {
	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/sky.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/sky.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	program = ShaderTools::makeProgram(shaders);

	// set attribute locations
	positionAttrib = 0;
	degreesAttrib = 1;
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "degrees");

	ShaderTools::linkProgram(program);

	// get uniform locations
	mvpMatrixUniform = glGetUniformLocation(program, "mvpMatrix");
	horizonColorUniform = glGetUniformLocation(program, "horizonColor");
	skyColorUniform = glGetUniformLocation(program, "skyColor");

	// set up the buffers
	glGenBuffers(1, &vertDataBuffer);
	glGenBuffers(1, &vertElementBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertDataBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertElementBuffer);

	// prepare arrays of vertex data and element data

	GLfloat vertDataBufferArray[] = {
			-1.0f, -1.0f, 0.999999f,
			-1.0f, 1.0f, 0.999999f,
			1.0f, 1.0f, 0.999999f,
			1.0f, -1.0f, 0.999999f
		};
	GLubyte vertElementBufferArray[] = { 0, 1, 2, 3 };

	// send the buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertDataBufferArray), vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertElementBufferArray), vertElementBufferArray,
			GL_STATIC_DRAW);
}

SkyRenderer::~SkyRenderer() {
	glDeleteBuffers(1, &vertDataBuffer);
	glDeleteBuffers(1, &vertElementBuffer);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(program);
}

void SkyRenderer::render(Matrix4 vpMatrix) {
	glUseProgram(program);

	// model view projection
	float vpMatrixArray[] = {
			vpMatrix.m11, vpMatrix.m12, vpMatrix.m13, vpMatrix.m14,
			vpMatrix.m21, vpMatrix.m22, vpMatrix.m23, vpMatrix.m24,
			vpMatrix.m31, vpMatrix.m32, vpMatrix.m33, vpMatrix.m34,
			vpMatrix.m41, vpMatrix.m42, vpMatrix.m43, vpMatrix.m44
		};

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, vpMatrixArray);

	glUniform3f(horizonColorUniform, 1.0f, 0.69f, 0.93f);
	glUniform3f(skyColorUniform, 0.36f, 0.09f, 0.73f);


//	glUniform3f(horizonColorUniform, 1.0f, 1.0f, 1.0f);
//	glUniform3f(skyColorUniform, 0.0f, 0.0f, 0.0f);

	// draw the data stored in GPU memory
//	glBindBuffer(GL_ARRAY_BUFFER, vertDataBuffer);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertElementBuffer);

//	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), NULL);
//	glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
//			(void*) (3 * sizeof(GLfloat)));

//	glEnableVertexAttribArray(positionAttrib);
//	glEnableVertexAttribArray(texCoordAttrib);

//	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_BYTE, NULL);

//	glDisableVertexAttribArray(vertDataBuffer);
printf("rotation %+.2f\n", degrees(asin(ship.orientation.m32)));
	glBegin(GL_QUADS);

	glVertexAttrib1f(degreesAttrib, (degrees(asin(ship.orientation.m32)) - 60.0f) / 90.0f);
	glVertexAttrib3f(positionAttrib, -1.0f, -1.0f, 0.999999f);

	glVertexAttrib1f(degreesAttrib, (degrees(asin(ship.orientation.m32)) + 60.0f) / 90.0f);
	glVertexAttrib3f(positionAttrib, -1.0f, 1.0f, 0.999999f);

	glVertexAttrib1f(degreesAttrib, (degrees(asin(ship.orientation.m32)) + 60.0f) / 90.0f);
	glVertexAttrib3f(positionAttrib, 1.0f, 1.0f, 0.999999f);

	glVertexAttrib1f(degreesAttrib, (degrees(asin(ship.orientation.m32)) - 60.0f) / 90.0f);
	glVertexAttrib3f(positionAttrib, 1.0f, -1.0f, 0.999999f);

	glEnd();
}
