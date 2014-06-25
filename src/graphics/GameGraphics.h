// GameGraphics.h
// Crucible Island

#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <map>
#include <string>
#include <vector>

#include "graphics/text/FontManager.h"
#include "graphics/texture/Texture.h"
#include "logic/Camera.h"
#include "math/MatrixMath.h"
#include "platform/OpenGLHeaders.h"

class GameGraphics {
private:
	std::map<std::string, GLuint> vertexShaderIDs;
	std::map<std::string, GLuint> fragmentShaderIDs;

	std::map<std::string, Texture*> textures;
	std::map<std::string, GLuint> textureIDs;

public:
	unsigned short int resolutionX, resolutionY;
	float aspectRatio;	// X over Y
	bool fullScreen;

	Matrix4 idMatrix, opMatrix, ppMatrix, ppMatrixInverse;
	float idMatrixArray[16], opMatrixArray[16], ppMatrixArray[16], ppMatrixInverseArray[16];

	FontManager* fontManager;
	Texture* noiseTexture;
	Texture* fourDepthNoiseTexture;

	Camera* currentCamera;

	GameGraphics(bool fullScreen, bool testSystem = false);
	~GameGraphics();

	GLuint getShaderID(GLenum shaderType, std::string shaderName);
	GLuint makeProgram(std::vector<GLuint> shaders);

	Texture* getTexture(std::string fileName);
	GLuint getTextureID(std::string fileName);

	void startFrame();
	void finishFrame();
};

#endif // GAMEGRAPHICS_H
