// GameGraphics.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

// initial declarations (needs to be prior to most program headers)
#include "platform/OpenGLHeaders.h"

class BaseDrawNode {
protected:
	GLuint shaderProgram;
	std::map<std::string, GLint> uniforms;
	std::map<std::string, GLint> attributes;
	std::map<std::string, GLuint> vertexBuffers;

public:
	virtual void execute(std::map<std::string, void*> arguments) = 0;
};

class BaseUIElement : public BaseDrawNode {
public:
	virtual Vector2 getSize(std::map<std::string, void*> arguments) = 0;
};

// forward declarations
class GameGraphics;
class GameLogic;
class InputHandler;
class Camera;

// program headers
#include "core/GameSystem.h"
#include "core/MainLoopMember.h"
#include "graphics/text/FontManager.h"
#include "graphics/texture/Texture.h"
#include "graphics/2dgraphics/DrawControlBox.h"
#include "graphics/2dgraphics/DrawCursor.h"
#include "graphics/2dgraphics/DrawGaugePanel.h"
#include "graphics/2dgraphics/DrawGrayOut.h"
#include "graphics/2dgraphics/DrawProgressBar.h"
#include "graphics/2dgraphics/DrawRadar.h"
#include "graphics/2dgraphics/DrawButton.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawLabel.h"
#include "graphics/2dgraphics/DrawField.h"
#include "graphics/2dgraphics/DrawTexture.h"
#include "graphics/2dgraphics/DrawSplash.h"
#include "graphics/3dgraphics/MissileRenderer.h"
#include "graphics/3dgraphics/ShellRenderer.h"
#include "graphics/3dgraphics/ShipRenderer.h"
#include "graphics/3dgraphics/SkyRenderer.h"
#include "graphics/3dgraphics/WaterRenderer.h"
#include "graphics/3dgraphics/TerrainRenderer.h"
#include "graphics/3dgraphics/TowerRenderer.h"
#include "input/InputHandler.h"
#include "logic/Camera.h"
#include "logic/GameLogic.h"
#include "math/MatrixMath.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"

// library headers
#include <fstream>
#include <map>
#include <SDL.h>
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
	Texture* noiseTexture;
	Texture* fourDepthNoiseTexture;

	// drawers
	std::map<std::string, BaseDrawNode*> drawers;

	// timers
	float waveTimer;
	float hardTimer;

	// camera tracking
	Camera* currentCamera;

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
	unsigned int execute(bool unScheduled = false);
};

#endif
