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

// program headers
#include "BMPImage.h"
#include "DiamondSquare.h"
#include "MatrixMath.h"
#include "Mesh.h"
#include "Platform.h"
#include "ShaderTools.h"
#include "Terrain.h"
#include "VectorMath.h"

// library headers
#include <OpenGL/OpenGL.h>

// global variables
extern Platform platform;

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

	// geometry data
	Terrain terrain;

public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Matrix4 mvpMatrix);
};

#endif
