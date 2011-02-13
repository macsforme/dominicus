/*
 *  SystemInfo.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 8/15/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "SystemInfo.h"

void SystemInfo::init() {
	// this functionality cannot be placed in a constructor because
	// this class is instantiated before SDL_Init() is called from the
	// main loop

	// report the build version
	std::stringstream buildInfo;
	buildInfo <<
			"Dominicus " <<
			buildVersion.version << " " <<
			buildVersion.classification << " " <<
			buildVersion.architecture << " " <<
			buildVersion.buildDate;
	programLog.report(LOG_INFO, buildInfo.str().c_str());

	// the first call to SDL_getVideoInfo gives us the width and height of the desktop
	SDL_VideoInfo* vidInfo = (SDL_VideoInfo*) SDL_GetVideoInfo();
	if(vidInfo == NULL)
		ProgramLog::report(LOG_FATAL, "Could not obtain screen resolution from SDL.");

	screenWidth = (uint16_t) vidInfo->current_w;
	screenHeight = (uint16_t) vidInfo->current_h;
}

void SystemInfo::check() {
	// This function checks the system for compatibility to ensure all
	// needed elements are present, and reports on general system features.

	// ===== OpenGL =====

	// OpenGL version
	std::string openGLVersionStr = (char*) glGetString(GL_VERSION);
	float openGLVersion = (float)atof(openGLVersionStr.substr(0, openGLVersionStr.find(' ')).c_str());

	if(openGLVersion < 2.0f) {
		std::stringstream err;
		err << "OpenGL version 2.0 or greater required, and only version " <<
				openGLVersionStr.substr(0, openGLVersionStr.find(' ')) <<
				" detected.";
		ProgramLog::report(LOG_FATAL, err.str().c_str());
	} else {
		std::stringstream err;
		err << "OpenGL Version: " <<
				openGLVersion;
		ProgramLog::report(
				LOG_INFO,
				err.str().c_str()
			);
	}

	// GLSL version
	float GLSLVersion = (float)atof((char*) glGetString(GL_SHADING_LANGUAGE_VERSION));

	if(GLSLVersion < 1.1f) {
		std::stringstream err;
		err << "GLSL version 1.10 or greater required, and only version " <<
				(char*) glGetString(GL_SHADING_LANGUAGE_VERSION) <<
				" detected.";
		ProgramLog::report(LOG_FATAL, err.str().c_str());
	} else {
		ProgramLog::report(
				LOG_INFO,
				std::string("GLSL Version: " + std::string((char*) glGetString(GL_SHADING_LANGUAGE_VERSION))).c_str()
			);
	}

	// OpenGL Renderer (no check, report only)
	ProgramLog::report(
			LOG_INFO,
			std::string("OpenGL Renderer: " + std::string((char*) glGetString(GL_RENDERER))).c_str()
		);

	// texture image units
	int maxTexUnits = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnits);

	if(maxTexUnits < 4) {
		std::stringstream err;
		err << "OpenGL implementation supports only " <<
				maxTexUnits <<
				" texture units, whereas 4 are required.";
		ProgramLog::report(LOG_FATAL, err.str().c_str());
	} else {
		std::stringstream err;
		err << "OpenGL Texture Units Supported: " <<
				maxTexUnits;
		ProgramLog::report(
				LOG_INFO,
				err.str().c_str()
			);
	}

	// anisotropic filtering extension
	if(strstr((const char*)glGetString(GL_EXTENSIONS),
			"GL_EXT_texture_filter_anisotropic") == NULL)
		ProgramLog::report(LOG_FATAL,
				"OpenGL extension not supported: GL_EXT_texture_filter_anisotropic");
	else
		ProgramLog::report(LOG_INFO, "OpenGL Extension Found: GL_EXT_texture_filter_anisotropic");
}
