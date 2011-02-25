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
#include "core/GamePrefs.h"
#include "drawing/texture/BMPImage.h"
#include "drawing/ShaderTools.h"
#include "drawing/texture/TextureUtils.h"
#include "geometry/DiamondSquare.h"
#include "geometry/Mesh.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "state/Terrain.h"

// global variables
extern GamePrefs* gamePrefs;
extern Platform* platform;
extern Terrain* terrain;

class TerrainRenderer {
private:
	// program/shader names
	GLuint program, vertexShader, fragmentShader;

	// attributes
	GLint positionAttrib, texCoordAttrib;

	// uniforms
	GLint mvpMatrixUniform, texturesUniform, noiseTextureUniform, totalWidthUniform, totalHeightUniform;

	// texture IDs
	GLuint textureIDs[4], noiseTextureID;

	// buffers
	GLuint vertDataBuffer, vertElementBuffer;

public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Matrix4 mvpMatrix);

	void reloadGeometry(bool firstLoad = false);
};

#endif
