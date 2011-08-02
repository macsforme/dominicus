/*
 * GameSystem.h
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

#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

// Game identifier string
#define PROGRAM_IDENTIFIER "Dominicus Conception"

// Major version (non-negative integer)
#define PROGRAM_VERSION 0

// Minor revision (non-negative integer)
#define PROGRAM_REVISION 1

// Build classification (char array, DEVEL | ALPHA# | BETA# | RC# | STABLE)
#define PROGRAM_CLASSIFICATION "DEVEL"

// forward declarations
class Platform;

// program headers
#include "OpenGLHeaders.h"
#include "Platform.h"
#include "VectorMath.h"

// library headers
#include <map>
#include <stdint.h>
#include <SDL/SDL.h>
#include <sstream>
#include <string>
#include <vector>

// global variables
extern Platform* platform;

// class definition
class GameSystem {
public:
	// versioning
	std::string identifier;
	std::string version;
	std::string classification;
	std::string architecture;
	std::string buildDate;

	// hardware specifications
	unsigned int displayResolutionX, displayResolutionY;

private:
	// standards
	struct StandardEntry {
		std::string value;
		std::string description;
		bool locked;
	};

	std::map<std::string,StandardEntry> standards;

	typedef std::vector< std::pair<std::string, SDLKey> > KeyBindingStorage;
	typedef std::vector< std::pair<std::string, Uint8> > MouseButtonBindingStorage;

	KeyBindingStorage keyBindings;
	MouseButtonBindingStorage mouseButtonBindings;

public:
	// logging
	enum LogDetail {
		LOG_INFO,	// critical game information that is always displayed
		LOG_VERBOSE,	// verbose information that users generally don't need to see
		LOG_FATAL	// errors created by outside means that we can't survive
	};

	std::vector<std::string> logLines;

	// standards retrieval
	bool isStandard(const char* key) const;

private:
	StandardEntry getStandard(const char* key);

public:
	std::string getString(const char* key);
	Vector4 getColor(const char* key);
	float getFloat(const char* key);
	bool getBool(const char* key);

	// standards manipulation
	void setStandard(const char* key, const char* value, const char* description, bool locked);
	void setStandard(const char* key, Vector4 value, const char* description, bool locked);
	void setStandard(const char* key, float value, const char* description, bool locked);
	void setStandard(const char* key, bool value, const char* description, bool locked);

	// input device bindings
	std::vector<SDLKey> getBindingKeys(std::string action);
	std::vector<Uint8> getBindingMouseButtons(std::string action);

	// logging
	void log(LogDetail detail, std::string report);

	// object management
	GameSystem();
};

#endif // GAMESYSTEM_H
