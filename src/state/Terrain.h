/*
 *  Terrain.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef TERRAIN_H
#define TERRAIN_H

// program headers
#include "DiamondSquare.h"
#include "MatrixMath.h"
#include "Mesh.h"
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "ScalarMath.h"
#include "ShaderTools.h"
#include "Texture.h"
#include "VectorMath.h"

// library headers
#include <vector>

// global variables
extern Platform platform;

/*
	Terrain Inputs

	Input		Minimum	Maximum
	---------------------------
	density		128		128
	rough		0.4		0.6
	gradDist	0.45	0.45
	alphaBumps	2		5
	flatness	0.33	0.33

*/

class Terrain {
private:
	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, texCoordAttrib;

	// uniforms
	GLint mvpMatrixUniform, textureUniform;

	// texture IDs
	std::vector<GLuint> alphaBumpTextureIDs;
	GLuint noiseTextureID;
	GLuint finalTextureID;
	GLuint finalCombinedTextureID;

	unsigned int alphaBumps;

public:
	unsigned short int** finalHeightMap;
	unsigned int density;

	Mesh mesh;

	Terrain();

	void render();
};

#endif
