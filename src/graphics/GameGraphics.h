// GameGraphics.h
// Dominicus

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

	std::map<std::string, GLuint> programIDs;

	std::map<std::string, Texture*> textures;
	std::map<std::string, GLuint> textureIDs;

	GLuint getShaderID(GLenum shaderType, std::string shaderName);
	GLuint makeProgram(std::vector<GLuint> shaders);

public:
	unsigned short int resolutionX, resolutionY;
	float aspectRatio;	// X over Y
	bool fullScreen;
	bool supportsMultisampling;

	Matrix4 idMatrix, opMatrix, ppMatrix, ppBinoMatrix/*, ppMatrixInverse*/;
	float idMatrixArray[16], opMatrixArray[16], ppMatrixArray[16], ppBinoMatrixArray[16]/*, ppMatrixInverseArray[16]*/;

	FontManager* fontManager;
	Texture* noiseTexture;
	Texture* fourDepthNoiseTexture;

	Camera* currentCamera;

	GameGraphics(bool fullScreen, bool testSystem = false);
	~GameGraphics();

	GLuint getProgramID(std::string name);

	Texture* getTexture(std::string fileName);
	GLuint getTextureID(std::string fileName);

	void startFrame();
	void finishFrame();
};

#endif // GAMEGRAPHICS_H
