/*
 * GameGraphics.cpp
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

#include "graphics/GameGraphics.h"

GameGraphics::GameGraphics(bool fullScreen, bool testSystem) : fullScreen(fullScreen) {
	// initialize an SDL window
	resolutionX = (fullScreen ? gameSystem->displayResolutionX :
			(int) gameSystem->getFloat("displayWindowedResolutionX"));
	resolutionY = (fullScreen ? gameSystem->displayResolutionY :
			(int) gameSystem->getFloat("displayWindowedResolutionY"));
	aspectRatio = (float) resolutionX / (float) resolutionY;

	Uint32 flags = SDL_OPENGL | (fullScreen ? SDL_FULLSCREEN : 0);

	if(! SDL_VideoModeOK(
			resolutionX,
			resolutionY,
			(int) gameSystem->getFloat("displayColorDepth"),
			flags
		))
		gameSystem->log(GameSystem::LOG_FATAL,
				"SDL cannot initialize a window with the specified settings.");

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, (int) gameSystem->getFloat("displayColorDepth"));
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Surface* surface = SDL_SetVideoMode(
			resolutionX,
			resolutionY,
			(int) gameSystem->getFloat("displayColorDepth"),
			flags
		);

	if(surface == NULL)
		gameSystem->log(GameSystem::LOG_FATAL,
				"SDL could not initialize an OpenGL-ready window.");

	if(! fullScreen)
		SDL_WM_SetCaption(gameSystem->identifier.c_str(), NULL);

	// if specified, do system test
	if(testSystem) {
		// test and log the OpenGL version for compatibility
		std::string openGLVersionStr = (char*) glGetString(GL_VERSION);
		float openGLVersion = (float) atof(openGLVersionStr.substr(0,
				openGLVersionStr.find(' ')).c_str());

		if(openGLVersion < 2.0f) {
			std::stringstream err;
			err << "OpenGL version 2.0 or greater required, and only version " <<
					openGLVersionStr.substr(0, openGLVersionStr.find(' ')) <<
					" detected.";
			gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
		} else {
			std::stringstream err;
			err << "OpenGL Version: " <<
					openGLVersion;
			gameSystem->log(
					GameSystem::LOG_VERBOSE,
					err.str().c_str()
				);
		}

		// test and log the GLSL version for compatibility
		float GLSLVersion = (float) atof((char*) glGetString(GL_SHADING_LANGUAGE_VERSION));

		if(GLSLVersion < 1.1f) {
			std::stringstream err;
			err << "GLSL version 1.10 or greater required, and only version " <<
					(char*) glGetString(GL_SHADING_LANGUAGE_VERSION) <<
					" detected.";
			gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
		} else {
			gameSystem->log(
					GameSystem::LOG_VERBOSE,
					std::string("GLSL Version: " +
							std::string((char*) glGetString(GL_SHADING_LANGUAGE_VERSION))).c_str()
				);
		}

		// log the OpenGL Renderer
		gameSystem->log(
				GameSystem::LOG_VERBOSE,
				std::string("OpenGL Renderer: " + std::string((char*) glGetString(GL_RENDERER))).c_str()
			);

		// test and log the number of texture image units
		int maxTexUnits = 0;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnits);

		if(maxTexUnits < 8) {
			std::stringstream err;
			err << "OpenGL implementation supports only " <<
					maxTexUnits <<
					" texture units, whereas 8 are required.";
			gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
		} else {
			std::stringstream err;
			err << "OpenGL Texture Units Supported: " <<
					maxTexUnits;
			gameSystem->log(
					GameSystem::LOG_VERBOSE,
					err.str().c_str()
				);
		}

		// test and log the presence of the anisotropic filtering extension
		if(strstr((const char*) glGetString(GL_EXTENSIONS),
				"GL_EXT_texture_filter_anisotropic") == NULL)
			gameSystem->log(GameSystem::LOG_FATAL,
					"OpenGL extension not supported: GL_EXT_texture_filter_anisotropic");
		else
			gameSystem->log(GameSystem::LOG_VERBOSE, "OpenGL Extension Found: GL_EXT_texture_filter_anisotropic");

		// test and log the presence of the framebuffer object extension (needed for glBuildMipmaps)
		if(strstr((const char*) glGetString(GL_EXTENSIONS),
				"GL_EXT_framebuffer_object") == NULL)
			gameSystem->log(GameSystem::LOG_FATAL,
					"OpenGL extension not supported: GL_EXT_framebuffer_object");
		else
			gameSystem->log(GameSystem::LOG_VERBOSE, "OpenGL Extension Found: GL_EXT_framebuffer_object");
	}

	// set up matrices
	idMatrix = Matrix4(
			1.0, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	float idMatrixArrayVals[] = {
			idMatrix.m11, idMatrix.m12, idMatrix.m13, idMatrix.m14,
			idMatrix.m21, idMatrix.m22, idMatrix.m23, idMatrix.m24,
			idMatrix.m31, idMatrix.m32, idMatrix.m33, idMatrix.m34,
			idMatrix.m41, idMatrix.m42, idMatrix.m43, idMatrix.m44
		};
	memcpy((void*) idMatrixArray, (void*) idMatrixArrayVals, 16 * sizeof(float));

	opMatrix = Matrix4(
			(float) resolutionY / (float) resolutionX, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	float opMatrixArrayVals[] = {
			opMatrix.m11, opMatrix.m12, opMatrix.m13, opMatrix.m14,
			opMatrix.m21, opMatrix.m22, opMatrix.m23, opMatrix.m24,
			opMatrix.m31, opMatrix.m32, opMatrix.m33, opMatrix.m34,
			opMatrix.m41, opMatrix.m42, opMatrix.m43, opMatrix.m44
		};
	memcpy((void*) opMatrixArray, (void*) opMatrixArrayVals, 16 * sizeof(float));

	const float fov = gameSystem->getFloat("renderingPerspectiveFOV");
	const float nClip = gameSystem->getFloat("renderingPerspectiveNearClip");
	const float fClip = gameSystem->getFloat("renderingPerspectiveFarClip");

	ppMatrix = Matrix4(
			1.0f / tan(radians(fov)), 0.0f, 0.0f, 0.0f,
			0.0f, aspectRatio / tan(radians(fov)), 0.0f, 0.0f,
			0.0f, 0.0f, (fClip + nClip) / (fClip - nClip), 1.0f,
			0.0f, 0.0f, -2.0f * fClip * nClip / (fClip - nClip), 0.0f
		);

	float ppMatrixArrayVals[] = {
			ppMatrix.m11, ppMatrix.m12, ppMatrix.m13, ppMatrix.m14,
			ppMatrix.m21, ppMatrix.m22, ppMatrix.m23, ppMatrix.m24,
			ppMatrix.m31, ppMatrix.m32, ppMatrix.m33, ppMatrix.m34,
			ppMatrix.m41, ppMatrix.m42, ppMatrix.m43, ppMatrix.m44
		};
	memcpy((void*) ppMatrixArray, (void*) ppMatrixArrayVals, 16 * sizeof(float));

	ppMatrixInverse = Matrix4(
			tan(radians(fov)) / 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, tan(radians(fov)) / aspectRatio, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, -(fClip - nClip) / (2.0f * fClip * nClip),
			0.0f, 0.0f, 1.0f, (fClip + nClip) / (2.0f * fClip * nClip)
		);

	float ppMatrixInverseArrayVals[] = {
			ppMatrixInverse.m11, ppMatrixInverse.m12, ppMatrixInverse.m13, ppMatrixInverse.m14,
			ppMatrixInverse.m21, ppMatrixInverse.m22, ppMatrixInverse.m23, ppMatrixInverse.m24,
			ppMatrixInverse.m31, ppMatrixInverse.m32, ppMatrixInverse.m33, ppMatrixInverse.m34,
			ppMatrixInverse.m41, ppMatrixInverse.m42, ppMatrixInverse.m43, ppMatrixInverse.m44
		};
	memcpy((void*) ppMatrixInverseArray, (void*) ppMatrixInverseArrayVals, 16 * sizeof(float));

/*
Vector4 rbl = Vector4(-1.0f, -1.0f, -1.0f, 1.0f) * ppMatrixInverse;
printf("RBL %.2f %.2f %.2f\n", rbl.x / rbl.w, rbl.y / rbl.w, rbl.z / rbl.w);

Vector4 rtl = Vector4(-1.0f, 1.0f, -1.0f, 1.0f) * ppMatrixInverse;
printf("RTL %.2f %.2f %.2f\n", rtl.x / rtl.w, rtl.y / rtl.w, rtl.z / rtl.w);

Vector4 rtr = Vector4(1.0f, 1.0f, -1.0f, 1.0f) * ppMatrixInverse;
printf("RTR %.2f %.2f %.2f\n", rtr.x / rtr.w, rtr.y / rtr.w, rtr.z / rtr.w);

Vector4 rbr = Vector4(1.0f, -1.0f, -1.0f, 1.0f) * ppMatrixInverse;
printf("RBR %.2f %.2f %.2f\n", rbr.x / rbr.w, rbr.y / rbr.w, rbr.z / rbr.w);

Vector4 fbl = Vector4(-1.0f, -1.0f, 1.0f, 1.0f) * ppMatrixInverse;
printf("FBL %.2f %.2f %.2f\n", fbl.x / fbl.w, fbl.y / fbl.w, fbl.z / fbl.w);

Vector4 ftl = Vector4(-1.0f, 1.0f, 1.0f, 1.0f) * ppMatrixInverse;
printf("FTL %.2f %.2f %.2f\n", ftl.x / ftl.w, ftl.y / ftl.w, ftl.z / ftl.w);

Vector4 ftr = Vector4(1.0f, 1.0f, 1.0f, 1.0f) * ppMatrixInverse;
printf("FTR %.2f %.2f %.2f\n", ftr.x / ftr.w, ftr.y / ftr.w, ftr.z / ftr.w);

Vector4 fbr = Vector4(1.0f, -1.0f, 1.0f, 1.0f) * ppMatrixInverse;
printf("FBR %.2f %.2f %.2f\n", fbr.x / fbr.w, fbr.y / fbr.w, fbr.z / fbr.w);
*/
	// set up fonts
	fontManager = new FontManager();
	fontManager->populateCommonChars((unsigned int) gameSystem->getFloat("fontSizeSmall"));
	fontManager->populateCommonChars((unsigned int) gameSystem->getFloat("fontSizeLarge"));

	// initialize FPS tracking
	lastFPSTest = 0;
	fpsCount = 0;
	currentFPS = 0;
	frameTime = 0;

	// timers;
	waveTimer = 0.0f;
	hardTimer = 0.0f;

	// bad programming practice, but set the global variable to us since
	// we should only exist once, and our drawers need to find us
	gameGraphics = this;

	// instantiate drawers
	drawers["splash"] = new DrawSplash();
	drawers["controlBox"] = new DrawControlBox();
	drawers["cursor"] = new DrawCursor();
	drawers["container"] = new DrawContainer();
	drawers["label"] = new DrawLabel();
	drawers["field"] = new DrawField();
	drawers["button"] = new DrawButton();
	drawers["texture"] = new DrawTexture();

	drawers["radar"] = new DrawRadar();
	drawers["grayOut"] = new DrawGrayOut();

	drawers["terrainRenderer"] = new TerrainRenderer();
	drawers["waterRenderer"] = new WaterRenderer();
	drawers["shipRenderer"] = new ShipRenderer();
}

GameGraphics::~GameGraphics() {
	// destroy fonts
	delete(fontManager);

	// delete drawers
	delete((DrawSplash*) drawers["splash"]);
	delete((DrawControlBox*) drawers["controlBox"]);
	delete((DrawCursor*) drawers["cursor"]);
	delete((DrawContainer*) drawers["container"]);
	delete((DrawLabel*) drawers["label"]);
	delete((DrawField*) drawers["field"]);
	delete((DrawButton*) drawers["button"]);
	delete((DrawTexture*) drawers["texture"]);

	delete ((DrawRadar*) drawers["radar"]);

	delete((TerrainRenderer*) drawers["terrainRenderer"]);
	delete((WaterRenderer*) drawers["waterRenderer"]);
	delete((ShipRenderer*) drawers["shipRenderer"]);

	// delete shaders
	std::map<std::string, GLuint>::iterator shaderItr;

	for(shaderItr = vertexShaderIDs.begin(); shaderItr != vertexShaderIDs.end(); ++shaderItr)
		if(glIsShader(shaderItr->second))
			glDeleteShader(shaderItr->second);

	for(shaderItr = fragmentShaderIDs.begin(); shaderItr != fragmentShaderIDs.end(); ++shaderItr)
		if(glIsShader(shaderItr->second))
			glDeleteShader(shaderItr->second);

	// delete textures
	std::map<std::string, Texture*>::iterator textureItr;

	for(textureItr = textures.begin(); textureItr != textures.end(); ++textureItr)
		delete(textureItr->second);

	std::map<std::string, GLuint>::iterator textureIDItr;

	for(textureIDItr = textureIDs.begin(); textureIDItr != textureIDs.end(); ++textureIDItr)
		if(glIsTexture(textureIDItr->second))
			glDeleteTextures(1, &(textureIDItr->second));
}

GLuint GameGraphics::getShaderID(GLenum shaderType, std::string shaderName) {
	// return the compiled shader ID
	std::map<std::string, GLuint>::iterator itr;
	std::string extension;
	if(shaderType == GL_VERTEX_SHADER) {
		itr = vertexShaderIDs.find(shaderName);

		if(itr != vertexShaderIDs.end())
			return itr->second;

		extension = ".vertex.glsl";
	} else if(shaderType == GL_FRAGMENT_SHADER) {
		itr = fragmentShaderIDs.find(shaderName);

		if(itr != fragmentShaderIDs.end())
			return itr->second;

		extension = ".fragment.glsl";
	}

	std::stringstream filename;
	filename <<
			platform->dataPath <<
			"/shaders/" <<
			shaderName <<
			extension;

	std::ifstream fileStream;
	std::vector<std::string> fileLines;

	fileStream.open(filename.str().c_str());
	while(fileStream.good()) {
		std::string thisChar = "";
		thisChar += fileStream.get();

		if(fileLines.size() == 0)
			fileLines.push_back("");

		fileLines.back() += thisChar;

		if(thisChar == "\n") {
			fileLines.push_back("");
		}
	}
	fileStream.close();

	if(fileLines.size() > 0)
		fileLines.pop_back();
	else
		gameSystem->log(GameSystem::LOG_FATAL,
				std::string("The GLSL shader " +
						filename.str() +
						" could not be opened for reading.").c_str()
			);

	GLsizei count = fileLines.size();
	uint8_t bytes = 0;
	GLchar** shaderSource;

	for(size_t i = 0; i < fileLines.size(); ++i)
		bytes += fileLines[i].size() + 1;

	shaderSource = (GLchar**) malloc(fileLines.size() * sizeof(size_t));

	for(size_t i = 0; i < fileLines.size(); ++i) {
		shaderSource[i] = (GLchar*) malloc(fileLines[i].size() + 1);
		memcpy(
				shaderSource[i],
				fileLines[i].c_str(),
				fileLines[i].size() + 1
			);
		shaderSource[i][fileLines[i].size()] = '\0';
	}

	GLuint shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, count, (const GLchar**) shaderSource, NULL);

	for(int i = 0; i < count; ++i)
		free(shaderSource[i]);
	free(shaderSource);

	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if(result == GL_FALSE) {
		std::stringstream err;
		err << "The GLSL shader "
				<< filename
				<< " did not compile successfully."
				<< std::endl << std::endl;

		GLint sourceLength;
		glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &sourceLength);
		GLchar* sourceLines = (GLchar*) malloc(sourceLength);
		glGetShaderSource(shader, sourceLength, NULL, sourceLines);

		err << "SHADER SOURCE ON GPU" << std::endl
				<< "--------------------" << std::endl
				<< sourceLines
				<< "--------------------" << std::endl << std::endl;
		free(sourceLines);

		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* logLines = (GLchar*) malloc(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, logLines);

		err << "ERROR LOG" << std::endl
				<< "---------" << std::endl
				<< logLines
				<< "---------";

		free(logLines);

		gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
	}

	return shader;
}

GLuint GameGraphics::makeProgram(std::vector<GLuint> shaders) {
	GLuint program;

	program = glCreateProgram();

	for(size_t i = 0; i < shaders.size(); ++i)
		glAttachShader(program, shaders[i]);

	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if(result == GL_FALSE) {
		std::stringstream err;
		err << "The GLSL shader program did not link successfully." << std::endl << std::endl;

		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* logLines = (GLchar*) malloc(logLength);
		glGetProgramInfoLog(program, logLength, NULL, logLines);

		err << "ERROR LOG" << std::endl
				<< "---------" << std::endl
				<< logLines
				<< "---------";

		free(logLines);

		gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
	}

	return program;
}

Texture* GameGraphics::getTexture(std::string filename) {
	if(textures.find(filename) != textures.end())
		return textures.find(filename)->second;

	std::stringstream filenameStream;
	filenameStream <<
			platform->dataPath <<
			"/data/textures/" <<
			filename <<
			".bmp";
	textures[filename] = new Texture(filenameStream.str().c_str());

	return textures[filename];
}

GLuint GameGraphics::getTextureID(std::string filename) {
	// return the stored texture ID if it exists
	if(textureIDs.find(filename) != textureIDs.end())
		return(textureIDs.find(filename)->second);

	std::stringstream filenameStream;
	filenameStream <<
			platform->dataPath <<
			"/data/textures/" <<
			filename <<
			".bmp";
	Texture texture(filenameStream.str().c_str());

	// load the texture into OpenGL
	glEnable(GL_TEXTURE_2D);

	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(texture.format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			texture.width,
			texture.height,
			0,
			(texture.format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			texture.getDataPointer()
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_2D);

	textureIDs[filename] = textureID;

	return textureID;
}

unsigned int GameGraphics::execute() {
	// track the frame time
	unsigned int frameBeginTime = platform->getExecMills();

	// set timers
	waveTimer = sin(radians((float) (platform->getExecMills() % 2000) / 2000.0f * 360.0f - 90.0f));
	hardTimer = ((float) (platform->getExecMills() % 2000) < 1000.0f ? 1.0f : 0.0f);

	// prepare OpenGL for rendering
	glViewport(0, 0, resolutionX, resolutionY);

	// clear the screen
	Vector4 clearColor = gameSystem->getColor("colorClear");
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw everything
	for(
			GameLogic::DrawStack::iterator itr = gameLogic->drawStack.begin();
			itr != gameLogic->drawStack.end();
			++itr
		)
		drawers[itr->first]->execute(itr->second);

	// swap buffers
	SDL_GL_SwapBuffers();

	// check for OpenGL errors
	GLenum renderingError = glGetError();
	while(renderingError != GL_NO_ERROR) {
		// log the error and check for another one
		std::stringstream err;
		err << "OpenGL returned an error: "
				<< renderingError;
		gameSystem->log(GameSystem::LOG_VERBOSE, err.str().c_str());

		renderingError = glGetError();
	}

	// track FPS
	unsigned int now = platform->getExecMills();
	if(now > (lastFPSTest + 1000 / (unsigned int) gameSystem->getFloat("hudFPSTestFrequency"))) {
		lastFPSTest = now;
		currentFPS = fpsCount * (unsigned int) gameSystem->getFloat("hudFPSTestFrequency");
		fpsCount = 0;
//printf("FPS: %u\n", currentFPS);
	}

	++fpsCount;

	// track the frame time
	frameTime = platform->getExecMills() - frameBeginTime;

	// calculate and return sleep time from superclass
	unsigned int frequency = (unsigned int) gameSystem->getFloat("displayFPS");
	unsigned int idealSleepTime = (
			frequency != 0 ?
			1000 / frequency : 0
		);

	return getSleepTime(idealSleepTime);
}
