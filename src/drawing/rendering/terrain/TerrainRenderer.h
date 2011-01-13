/*
 *  TerrainRenderer.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

// definitions
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

// program headers
#include "BMPImage.h"
#include "DiamondSquare.h"
#include "MatrixMath.h"
#include "Mesh.h"
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "ShaderTools.h"
#include "Terrain.h"
#include "VectorMath.h"

// global variables
extern Platform platform;
extern Terrain terrain;

class TerrainRenderer {
private:
	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, texCoordAttrib;

	// uniforms
	GLint mvpMatrixUniform, textureUniform;

	// texture IDs
	GLuint textureID;

	// buffers
	GLuint vertDataBuffer, vertElementBuffer;

public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Matrix4 mvpMatrix);

	void reloadGeometry(bool firstLoad = false);
};

#endif
