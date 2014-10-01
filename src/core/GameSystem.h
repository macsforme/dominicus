// GameSystem.h
// Crucible Island

#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

// Game identifier string
#define PROGRAM_IDENTIFIER "Crucible Island"

// Version number (positive integer increased by 1 at each release)
#define PROGRAM_VERSION "1"

// Build identifier string (DEVEL | ALPHA# | BETA# | STABLE)
#define PROGRAM_BUILDSTRING "DEVEL"

#include <map>
#include <SDL.h>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

#include "math/VectorMath.h"

class GameSystem {
public:
	std::string versionString;

	unsigned int displayResolutionX, displayResolutionY; // hardware specification

private:
	struct StandardEntry {
		std::string value;
		std::string description;
		bool locked;
	};

	std::map<std::string,StandardEntry> standards;

	typedef std::vector< std::pair<std::string, SDLKey> > KeyBindingStorage;
	typedef std::vector< std::pair<std::string, uint8_t> > MouseButtonBindingStorage;

	KeyBindingStorage keyBindings;
	MouseButtonBindingStorage mouseButtonBindings;

public:
	enum LogDetail {
		LOG_INFO, // critical game information that is always displayed
		LOG_VERBOSE, // verbose information that users generally don't need to see
		LOG_FATAL // errors created by outside means that we can't survive
	};

	std::vector<std::string> logLines;

	void log(LogDetail detail, std::string report);

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

	std::vector< std::pair<unsigned int, std::string> > highScores;

	// window element scaling
	std::vector< std::pair<unsigned int, unsigned int> > getAllowedWindowResolutions();
	void applyScreenResolution(std::string resolution);

	enum FrameRateLimiting {
		LIMIT_VSYNC,
		LIMIT_30,
		LIMIT_60,
		LIMIT_120,
		LIMIT_OFF
	};

	GameSystem();
};

#endif // GAMESYSTEM_H
