// TerrainRenderer.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/3dgraphics/renderers/TerrainRenderer.h"

TerrainRenderer::TerrainRenderer() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "terrain"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "terrain"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["textures"] = glGetUniformLocation(shaderProgram, "textures");
	uniforms["lightPosition"] = glGetUniformLocation(shaderProgram, "lightPosition");
	uniforms["lightColor"] = glGetUniformLocation(shaderProgram, "lightColor");
	uniforms["depth"] = glGetUniformLocation(shaderProgram, "depth");
	uniforms["height"] = glGetUniformLocation(shaderProgram, "height");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["normal"] = glGetAttribLocation(shaderProgram, "normal");
	attributes["texCoord"] = glGetAttribLocation(shaderProgram, "texCoord");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));
}

TerrainRenderer::~TerrainRenderer() {
	;
}

void TerrainRenderer::reloadGraphics() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	Mesh* terrainMesh = &(gameState->world->mesh);
	std::vector<Mesh::Face>* faceGroup = &(terrainMesh->faceGroups.begin()->second);
	size_t vertDataBufferArraySize =
			24 *
			faceGroup->size() *
			sizeof(GLfloat); // 3 vertices + 3 normals + 2 texcoords * 3 per face
float maxHeight = 0.0f;
	GLfloat* vertDataBufferArray = (GLfloat*) malloc(vertDataBufferArraySize);
	for(size_t i = 0; i < faceGroup->size(); ++i) {
		vertDataBufferArray[i * 24 + 0] = terrainMesh->vertices[(*faceGroup)[i].vertices[0]].x;
		vertDataBufferArray[i * 24 + 1] = terrainMesh->vertices[(*faceGroup)[i].vertices[0]].y;
		vertDataBufferArray[i * 24 + 2] = terrainMesh->vertices[(*faceGroup)[i].vertices[0]].z;

		vertDataBufferArray[i * 24 + 3] = terrainMesh->normals[(*faceGroup)[i].normals[0]].x;
		vertDataBufferArray[i * 24 + 4] = terrainMesh->normals[(*faceGroup)[i].normals[0]].y;
		vertDataBufferArray[i * 24 + 5] = terrainMesh->normals[(*faceGroup)[i].normals[0]].z;

		vertDataBufferArray[i * 24 + 6] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[0]].x;
		vertDataBufferArray[i * 24 + 7] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[0]].y;

		vertDataBufferArray[i * 24 + 8] = terrainMesh->vertices[(*faceGroup)[i].vertices[1]].x;
		vertDataBufferArray[i * 24 + 9] = terrainMesh->vertices[(*faceGroup)[i].vertices[1]].y;
		vertDataBufferArray[i * 24 + 10] = terrainMesh->vertices[(*faceGroup)[i].vertices[1]].z;

		vertDataBufferArray[i * 24 + 11] = terrainMesh->normals[(*faceGroup)[i].normals[1]].x;
		vertDataBufferArray[i * 24 + 12] = terrainMesh->normals[(*faceGroup)[i].normals[1]].y;
		vertDataBufferArray[i * 24 + 13] = terrainMesh->normals[(*faceGroup)[i].normals[1]].z;

		vertDataBufferArray[i * 24 + 14] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[1]].x;
		vertDataBufferArray[i * 24 + 15] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[1]].y;

		vertDataBufferArray[i * 24 + 16] = terrainMesh->vertices[(*faceGroup)[i].vertices[2]].x;
		vertDataBufferArray[i * 24 + 17] = terrainMesh->vertices[(*faceGroup)[i].vertices[2]].y;
		vertDataBufferArray[i * 24 + 18] = terrainMesh->vertices[(*faceGroup)[i].vertices[2]].z;

		vertDataBufferArray[i * 24 + 19] = terrainMesh->normals[(*faceGroup)[i].normals[2]].x;
		vertDataBufferArray[i * 24 + 20] = terrainMesh->normals[(*faceGroup)[i].normals[2]].y;
		vertDataBufferArray[i * 24 + 21] = terrainMesh->normals[(*faceGroup)[i].normals[2]].z;

		vertDataBufferArray[i * 24 + 22] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[2]].x;
		vertDataBufferArray[i * 24 + 23] = terrainMesh->texCoords[(*faceGroup)[i].texCoords[2]].y;

if(terrainMesh->vertices[(*faceGroup)[i].vertices[0]].y > maxHeight)
	maxHeight = terrainMesh->vertices[(*faceGroup)[i].vertices[0]].y;
if(terrainMesh->vertices[(*faceGroup)[i].vertices[0]].y > maxHeight)
	maxHeight = terrainMesh->vertices[(*faceGroup)[i].vertices[0]].y;
if(terrainMesh->vertices[(*faceGroup)[i].vertices[0]].y > maxHeight)
	maxHeight = terrainMesh->vertices[(*faceGroup)[i].vertices[0]].y;


	}
//printf("renderer max height %f\n", maxHeight);
//printf("given height %f\n", gameState->world->height);
	size_t vertElementBufferArraySize = 3 * faceGroup->size() * sizeof(GLuint);
	GLuint* vertElementBufferArray = (GLuint*) malloc(vertElementBufferArraySize);
	for(size_t i = 0; i < faceGroup->size() * 3; ++i) {
		vertElementBufferArray[i] = i;
	}

	// send the buffer data
	glBufferData(GL_ARRAY_BUFFER, vertDataBufferArraySize, vertDataBufferArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertElementBufferArraySize, vertElementBufferArray,
			GL_STATIC_DRAW);

	free(vertDataBufferArray);
	free(vertElementBufferArray);

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &noiseTextureID);
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(gameState->fourDepthNoiseTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			gameState->fourDepthNoiseTexture->width,
			gameState->fourDepthNoiseTexture->height,
			0,
			(gameState->fourDepthNoiseTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			gameState->fourDepthNoiseTexture->getDataPointer()
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_2D);
}

void TerrainRenderer::execute(std::map<std::string, void*> arguments) {
	// collect arguments
	Camera* camera = ((Camera*) arguments["camera"]);

	// call for an updated camera
	camera->execute();

	// prepare variables
	Matrix4* mvpMatrix = &(camera->terrainMatrix);
	float mvpMatrixArray[] = {
			mvpMatrix->m11, mvpMatrix->m12, mvpMatrix->m13, mvpMatrix->m14,
			mvpMatrix->m21, mvpMatrix->m22, mvpMatrix->m23, mvpMatrix->m24,
			mvpMatrix->m31, mvpMatrix->m32, mvpMatrix->m33, mvpMatrix->m34,
			mvpMatrix->m41, mvpMatrix->m42, mvpMatrix->m43, mvpMatrix->m44
		};

	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, mvpMatrixArray);

	Matrix3 orientation = completeMatrix(gameState->ships[0]->direction);
	orientation = Matrix3(
			-orientation.m31, orientation.m32, -orientation.m33,
			orientation.m21, orientation.m22, orientation.m23,
			orientation.m11, orientation.m12, orientation.m13
		);
	orientation.transpose();
	Vector3 lightPosition(1.0f, 1.0f, -1.0f);
	lightPosition.norm();
	lightPosition = lightPosition * orientation;

	glUniform3f(uniforms["lightPosition"], lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(uniforms["lightColor"], 0.8f, 0.8f, 1.0f);
	GLint textureUniforms[] = { 0, 1, 2, 3, 4 };
	glUniform1iv(uniforms["textures"], 5, textureUniforms);
	glUniform1f(uniforms["depth"], (GLfloat) gameSystem->getFloat("terrainDepth"));
	glUniform1f(uniforms["height"], (GLfloat) gameState->world->height);
//static bool printMe = true; if(printMe) { printf("sent depth %f, height %f\n", gameSystem->getFloat("terrainDepth"), gameState->world->height); printMe = false;}

	// activate the textures
	for(int i = 0; i < 3; ++i) {
		if(i == 0) glActiveTexture(GL_TEXTURE0);
		else if(i == 1) glActiveTexture(GL_TEXTURE1);
		else if(i == 2) glActiveTexture(GL_TEXTURE2);
//		else if(i == 3) glActiveTexture(GL_TEXTURE3);

		std::stringstream terrainFile;
		terrainFile << "terrain/" << i;
		glBindTexture(GL_TEXTURE_2D, gameGraphics->getTextureID(terrainFile.str().c_str()));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		float maxAniso = 4.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
//printf("maxIniso %f\n", maxAniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	}

	// activate the noise texture
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//	float maxAniso;
//	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["normal"], 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT),
			(GLvoid*) (6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["normal"]);
	glEnableVertexAttribArray(attributes["texCoord"]);

	glDrawElements(GL_TRIANGLES, gameState->world->mesh.faceGroups.begin()->second.size() * 3,
			GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["normal"]);
	glDisableVertexAttribArray(attributes["texCoord"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

/*
TerrainRenderer::TerrainRenderer() {
	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform->dataPath +  "/shaders/terrain.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform->dataPath + "/shaders/terrain.fragment.glsl").c_str(),
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
				platform->dataPath <<
				"/data/textures/terrain/" <<
				i <<
				".bmp";
		BMPImage texture(filename.str().c_str());

		// load the texture into OpenGL
		GLuint textureID = 0;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

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
	const size_t dsDensity = (size_t) gamePrefs->getFloat("renderingNoiseDensity");
	DiamondSquare diamondSquare(dsDensity, gamePrefs->getFloat("renderingNoiseRoughness"));
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
	setTextureDepth(&texture, (unsigned int) gamePrefs->getFloat("renderingNoiseDepth"));

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

	glDrawElements(GL_TRIANGLES, terrain->mesh.faceGroups.begin()->second.size() * 3, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(positionAttrib);
	glDisableVertexAttribArray(texCoordAttrib);

	glDisable(GL_TEXTURE_2D);
}

void TerrainRenderer::reloadGeometry(bool firstLoad) {
	const float texDivisor =
			gamePrefs->getFloat("renderingTerrainTextureRepeat"); // texture coordinate divisor

	glBindBuffer(GL_ARRAY_BUFFER, vertDataBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertElementBuffer);

}
*/
