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
	texturesUniform = glGetUniformLocation(program, "textures");
	noiseTextureUniform = glGetUniformLocation(program, "noiseTexture");
	totalWidthUniform = glGetUniformLocation(program, "totalWidth");
	totalHeightUniform = glGetUniformLocation(program, "totalHeight");

	// load the textures into OpenGL
	for(unsigned int i = 0; i < 4; ++i) {
		std::stringstream filename;
		filename <<
				platform.dataPath <<
				"/data/textures/terrain/" <<
				i <<
				".bmp";
		BMPImage texture(filename.str().c_str());

		// load the texture into OpenGL
		GLuint textureID = 0;
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
				(texture.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
				texture.getWidth(),
				texture.getHeight(),
				0,
				(texture.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
				GL_UNSIGNED_BYTE,
				texture.getDataPointer()
		);

		glGenerateMipmap(GL_TEXTURE_2D);

		textureIDs[i] = textureID;
	}

	// load the noise texture into OpenGL
	const size_t dsDensity = (size_t) gamePrefs.getInt("renderingNoiseDensity");
	DiamondSquare diamondSquare(dsDensity, gamePrefs.getFloat("renderingNoiseRoughness"));
	Texture texture(dsDensity, dsDensity, Texture::FORMAT_RGBA);
	for(size_t i = 0; i < dsDensity; ++i)
		for(size_t p = 0; p < dsDensity; ++p)
			texture.setColorAt(
					(uint32_t) i,
					(uint32_t) p,
					(uint8_t) (diamondSquare.data[i][p] * 128.0f + 127.0f),
					(uint8_t) (diamondSquare.data[i][p] * 128.0f + 127.0f),
					(uint8_t) (diamondSquare.data[i][p] * 128.0f + 127.0f),
					0xFF
				);
	setTextureDepth(&texture, (unsigned int) gamePrefs.getInt("renderingNoiseDepth"));

	glGenTextures(1, &noiseTextureID);
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(texture.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			texture.getWidth(),
			texture.getHeight(),
			0,
			(texture.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			texture.getDataPointer()
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	// generate the vertex buffers
	reloadGeometry(true);
}

TerrainRenderer::~TerrainRenderer() {
	for(unsigned int i = 0; i < 4; ++i)
		glDeleteTextures(1, &(textureIDs[i]));
	glDeleteTextures(1, &noiseTextureID);

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

	// dimensions uniforms
	glUniform1f(totalWidthUniform, TERRAIN_MAXWIDTH);
	glUniform1f(totalHeightUniform, TERRAIN_MAXHEIGHT);

	// textures
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureIDs[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureIDs[2]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureIDs[3]);

	GLint samplerList[] = { 0, 1, 2, 3 };
	glUniform1iv(texturesUniform, 4, samplerList);

	// noise texture
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);

	glUniform1i(noiseTextureUniform, 4);

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
	const float texDivisor =
			(float) gamePrefs.getInt("renderingTerrainRepeat"); // texture coordinate divisor

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

	size_t vertDataBufferArraySize =
			(size_t) 15 * faces->size() * sizeof(GLfloat); // 3 vertices + 2 texcoords * 3 per face
	GLfloat* vertDataBufferArray = (GLfloat*) malloc(vertDataBufferArraySize);
	for(size_t i = 0; i < faces->size(); ++i) {
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

	size_t vertElementBufferArraySize = (size_t) 3 * faces->size() * sizeof(GLuint);
	GLuint* vertElementBufferArray = (GLuint*) malloc(vertElementBufferArraySize);
	for(size_t i = 0; i < faces->size(); ++i) {
		vertElementBufferArray[i * 3 + 0] = i * 3 + 0;
		vertElementBufferArray[i * 3 + 1] = i * 3 + 1;
		vertElementBufferArray[i * 3 + 2] = i * 3 + 2;
	}

	// send the buffer data
	glBufferData(GL_ARRAY_BUFFER, vertDataBufferArraySize, vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertElementBufferArraySize, vertElementBufferArray,
			GL_STATIC_DRAW);

	free(vertDataBufferArray);
	free(vertElementBufferArray);
}
