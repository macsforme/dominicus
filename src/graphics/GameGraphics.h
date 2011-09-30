// GameGraphics.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

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
#include "graphics/3dgraphics/renderers/SkyRenderer.h"
#include "graphics/3dgraphics/renderers/WaterRenderer.h"
#include "graphics/3dgraphics/renderers/TerrainRenderer.h"
#include "input/InputHandler.h"
#include "logic/GameLogic.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
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

	Matrix4 idMatrix, opMatrix, ppMatrix, ppMatrixInverse;
	float idMatrixArray[16], opMatrixArray[16], ppMatrixArray[16], ppMatrixInverseArray[16];

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
