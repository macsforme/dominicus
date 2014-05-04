// GameSystem.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

// Game identifier string
#define PROGRAM_IDENTIFIER "Crucible Island"

// Version number (positive integer increased by 1 at each release)
#define PROGRAM_VERSION "1"

// Build identifier string (DEVEL# | ALPHA# | BETA# | STABLE)
#define PROGRAM_BUILDSTRING "DEVEL1"

// forward declarations
class Platform;

// program headers
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "math/VectorMath.h"

// library headers
#include <map>
#include <stdint.h>
#include <SDL.h>
#include <sstream>
#include <string>
#include <vector>

// global variables
extern Platform* platform;

// class definition
class GameSystem {
public:
	// versioning
	std::string versionString;

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

	void log(LogDetail detail, std::string report);

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
	void setStandard(const char* key, const char* value, const char* description = "", bool locked = false);
	void setStandard(const char* key, Vector4 value, const char* description = "", bool locked = false);
	void setStandard(const char* key, float value, const char* description = "", bool locked = false);
	void setStandard(const char* key, bool value, const char* description = "", bool locked = false);

	// standards/preferences integration
	void flushPreferences();
	
	// input device bindings
	std::vector<SDLKey> getBindingKeys(std::string action);
	std::vector<Uint8> getBindingMouseButtons(std::string action);

	// high score data
	std::vector< std::pair<unsigned int, std::string> > highScores;
	
	// object management
	GameSystem();
};

#endif // GAMESYSTEM_H
