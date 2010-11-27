/*
 *  SplashBackground.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/6/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "SplashBackground.h"

SplashBackground::SplashBackground() {
	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/default.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/default.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	program = ShaderTools::makeProgram(shaders);

	// get attribute locations
	positionAttrib = glGetAttribLocation(program, "position");
	texCoordAttrib = glGetAttribLocation(program, "texCoord");

	// get uniform locations
	textureUniform = glGetUniformLocation(program, "texture");
	mvpMatrixUniform = glGetUniformLocation(program, "mvpMatrix");

	// load the texture
	BMPImage image(platform.dataPath + "/data/textures/program/splash.bmp");

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(image.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			image.getWidth(),
			image.getHeight(),
			0,
			(image.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			image.getDataPointer()
	);

	glDisable(GL_TEXTURE_2D);
}

void SplashBackground::draw() {
	glUseProgram(program);

	Matrix4 mvpMatrix;
	mvpMatrix.identity();

	float mvpMatrixArray[] = {
			mvpMatrix.m11, mvpMatrix.m12, mvpMatrix.m13, mvpMatrix.m14,
			mvpMatrix.m21, mvpMatrix.m22, mvpMatrix.m23, mvpMatrix.m24,
			mvpMatrix.m31, mvpMatrix.m32, mvpMatrix.m33, mvpMatrix.m34,
			mvpMatrix.m41, mvpMatrix.m42, mvpMatrix.m43, mvpMatrix.m44
		};
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrixArray);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(textureUniform, 0);

	glBegin(GL_QUADS);

	glVertexAttrib2f(texCoordAttrib, 0.0f, 0.0f);
	glVertexAttrib3f(positionAttrib, -1.0f, -1.0f, 0.99f);

	glVertexAttrib2f(texCoordAttrib, 0.0f, 1.0f);
	glVertexAttrib3f(positionAttrib, -1.0f, 1.0f, 0.99f);

	glVertexAttrib2f(texCoordAttrib, 1.0f, 1.0f);
	glVertexAttrib3f(positionAttrib, 1.0f, 1.0f, 0.99f);

	glVertexAttrib2f(texCoordAttrib, 1.0f, 0.0f);
	glVertexAttrib3f(positionAttrib, 1.0f, -1.0f, 0.99f);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}
