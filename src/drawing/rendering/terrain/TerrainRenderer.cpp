/*
 *  TerrainRenderer.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer() {
	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/terrain.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/terrain.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	program = ShaderTools::makeProgram(shaders);

	// set attribute locations
	positionAttrib = 0;
	texCoordAttrib = 1;
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "texCoord");

	ShaderTools::linkProgram(program);

	// get uniform locations
	mvpMatrixUniform = glGetUniformLocation(program, "mvpMatrix");
	textureUniform = glGetUniformLocation(program, "texture");

	// load the texture into OpenGL
	std::string textureFile = platform.dataPath + std::string("/data/textures/terrain/dirt.bmp");
	BMPImage image(textureFile);

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	float maxAniso;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

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

	glGenerateMipmap(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_2D);

	// generate the vertex buffers
	reloadGeometry(true);
}

TerrainRenderer::~TerrainRenderer() {
	glDeleteBuffers(1, &vertDataBuffer);
	glDeleteBuffers(1, &vertElementBuffer);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(program);
}

void TerrainRenderer::render(Matrix4 vpMatrix) {
	glUseProgram(program);

	// model view projection
	float vpMatrixArray[] = {
			vpMatrix.m11, vpMatrix.m12, vpMatrix.m13, vpMatrix.m14,
			vpMatrix.m21, vpMatrix.m22, vpMatrix.m23, vpMatrix.m24,
			vpMatrix.m31, vpMatrix.m32, vpMatrix.m33, vpMatrix.m34,
			vpMatrix.m41, vpMatrix.m42, vpMatrix.m43, vpMatrix.m44
		};

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, vpMatrixArray);

	// textures
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureUniform, 0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertDataBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertElementBuffer);

	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), NULL);
	glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
			(void*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(positionAttrib);
	glEnableVertexAttribArray(texCoordAttrib);

	glDrawElements(GL_TRIANGLES, terrain.mesh.faceGroups.begin()->second.size() * 3, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(positionAttrib);
	glDisableVertexAttribArray(texCoordAttrib);

	glDisable(GL_TEXTURE_2D);
}

void TerrainRenderer::reloadGeometry(bool firstLoad) {
	const float texDivisor = 50.0f; // texture coordinate divisor

	// set up the buffers
	if(! firstLoad && glIsBuffer(vertDataBuffer))
		glDeleteBuffers(1, &vertDataBuffer);
	if(! firstLoad && glIsBuffer(vertElementBuffer))
		glDeleteBuffers(1, &vertElementBuffer);

	glGenBuffers(1, &vertDataBuffer);
	glGenBuffers(1, &vertElementBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertDataBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertElementBuffer);

	// prepare arrays of vertex data and element data
	std::vector<Mesh::Face>* faces = &(terrain.mesh.faceGroups.begin()->second);

	GLfloat* vertDataBufferArray = (GLfloat*)malloc(sizeof(GLfloat) * 15 * faces->size());	// 3 vertices + 2 texcoords * 3 per face
	for(int i = 0; i < faces->size(); ++i) {
		vertDataBufferArray[i * 15 + 0] = terrain.mesh.vertices[(*faces)[i].vertices[0]].x;
		vertDataBufferArray[i * 15 + 1] = terrain.mesh.vertices[(*faces)[i].vertices[0]].y;
		vertDataBufferArray[i * 15 + 2] = terrain.mesh.vertices[(*faces)[i].vertices[0]].z;

		vertDataBufferArray[i * 15 + 3] = terrain.mesh.vertices[(*faces)[i].vertices[0]].x / texDivisor;
		vertDataBufferArray[i * 15 + 4] = terrain.mesh.vertices[(*faces)[i].vertices[0]].z / texDivisor;

		vertDataBufferArray[i * 15 + 5] = terrain.mesh.vertices[(*faces)[i].vertices[1]].x;
		vertDataBufferArray[i * 15 + 6] = terrain.mesh.vertices[(*faces)[i].vertices[1]].y;
		vertDataBufferArray[i * 15 + 7] = terrain.mesh.vertices[(*faces)[i].vertices[1]].z;

		vertDataBufferArray[i * 15 + 8] = terrain.mesh.vertices[(*faces)[i].vertices[1]].x / texDivisor;
		vertDataBufferArray[i * 15 + 9] = terrain.mesh.vertices[(*faces)[i].vertices[1]].z / texDivisor;

		vertDataBufferArray[i * 15 + 10] = terrain.mesh.vertices[(*faces)[i].vertices[2]].x;
		vertDataBufferArray[i * 15 + 11] = terrain.mesh.vertices[(*faces)[i].vertices[2]].y;
		vertDataBufferArray[i * 15 + 12] = terrain.mesh.vertices[(*faces)[i].vertices[2]].z;

		vertDataBufferArray[i * 15 + 13] = terrain.mesh.vertices[(*faces)[i].vertices[2]].x / texDivisor;
		vertDataBufferArray[i * 15 + 14] = terrain.mesh.vertices[(*faces)[i].vertices[2]].z / texDivisor;
	}

	GLuint *vertElementBufferArray = (GLuint*)malloc(sizeof(GLuint) *3 * faces->size());
	for(int i = 0; i < faces->size(); ++i) {
		vertElementBufferArray[i * 3 + 0] = i * 3 + 0;
		vertElementBufferArray[i * 3 + 1] = i * 3 + 1;
		vertElementBufferArray[i * 3 + 2] = i * 3 + 2;
	}

	// send the buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertDataBufferArray), vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertElementBufferArray), vertElementBufferArray,
			GL_STATIC_DRAW);

	free(vertDataBufferArray);
	free(vertElementBufferArray);
}
