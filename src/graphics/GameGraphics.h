/*
 * GameGraphics.h
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

#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

// forward declarations
class DrawControlBox;
class DrawCursor;
class DrawSplash;
class GameGraphics;
class GameLogic;
class InputHandler;

// program headers
#include "core/GameSystem.h"
#include "core/MainLoopMember.h"
#include "graphics/BaseDrawNode.h"
#include "graphics/text/FontManager.h"
#include "graphics/texture/Texture.h"
#include "graphics/2dgraphics/controlbox/DrawControlBox.h"
#include "graphics/2dgraphics/cursor/DrawCursor.h"
#include "graphics/2dgraphics/ui/DrawGrayOut.h"
#include "graphics/2dgraphics/ui/DrawRadar.h"
#include "graphics/2dgraphics/ui/components/DrawButton.h"
#include "graphics/2dgraphics/ui/components/DrawContainer.h"
#include "graphics/2dgraphics/ui/components/DrawLabel.h"
#include "graphics/2dgraphics/ui/components/DrawField.h"
#include "graphics/2dgraphics/ui/components/DrawTexture.h"
#include "graphics/2dgraphics/splash/DrawSplash.h"
#include "graphics/3dgraphics/renderers/ShipRenderer.h"
#include "graphics/3dgraphics/renderers/TerrainRenderer.h"
#include "graphics/3dgraphics/renderers/WaterRenderer.h"
#include "input/InputHandler.h"
#include "logic/GameLogic.h"
#include "math/MatrixMath.h"
#include "VectorMath.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"

// library headers
#include <fstream>
#include <map>
#include <SDL/SDL.h>
#include <sstream>
#include <stdint.h>
#include <string>
#include <string.h>
#include <vector>

// global variables
extern GameGraphics* gameGraphics;
extern GameLogic* gameLogic;
extern GameSystem* gameSystem;
extern InputHandler* inputHandler;
extern Platform* platform;

class GameGraphics : public MainLoopMember {
public:
	// state info
	unsigned short int resolutionX, resolutionY;
	float aspectRatio;
	bool fullScreen;

	Matrix4 ppMatrix, opMatrix, idMatrix;
	float ppMatrixArray[16], opMatrixArray[16], idMatrixArray[16];

	// other info
	FontManager* fontManager;

	// FPS tracking
	unsigned int lastFPSTest;
	unsigned int fpsCount;
	unsigned int currentFPS;
	unsigned int frameTime;

	// drawers
	std::map<std::string, BaseDrawNode*> drawers;

	// timers
	float waveTimer;
	float hardTimer;

private:
	std::map<std::string, GLuint> vertexShaderIDs;
	std::map<std::string, GLuint> fragmentShaderIDs;

	std::map<std::string, Texture*> textures;
	std::map<std::string, GLuint> textureIDs;

public:
	// object management
	GameGraphics(bool fullScreen, bool testSystem = false);
	~GameGraphics();

	// shader and program management
	GLuint getShaderID(GLenum shaderType, std::string shaderName);
	GLuint makeProgram(std::vector<GLuint> shaders);

	// texture management
	Texture* getTexture(std::string fileName);
	GLuint getTextureID(std::string fileName);

	// frame execution
	unsigned int execute();
};

#endif
