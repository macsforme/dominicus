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

#include <cstdlib>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "math/VectorMath.h"

class GameSystem {
	std::vector<std::string> logLines;

	struct StandardEntry {
		std::string value;
		std::string description;
		bool locked;
	};

	std::map<std::string,StandardEntry> standards;

	StandardEntry getStandard(const char* key);

public:
	std::string buildDate;

	GameSystem();

	enum LogDetail {
		LOG_INFO, // critical game information that is always displayed
		LOG_VERBOSE, // verbose information that users generally don't need to see
		LOG_FATAL // errors created by outside means that we can't survive
	};

	void log(LogDetail detail, std::string report);

	std::string getString(const char* key) { return getStandard(key).value; }
	Vector4 getColor(const char* key);
	float getFloat(const char* key) { return (float) atof(getString(key).c_str()); }
	bool getBool(const char* key) { return getString(key) != "false"; }

	void setStandard(const char* key, const char* value, const char* description = "", bool locked = false);
	void setStandard(const char* key, Vector4 value, const char* description = "", bool locked = false);
	void setStandard(const char* key, float value, const char* description = "", bool locked = false);
	void setStandard(const char* key, bool value, const char* description = "", bool locked = false);
	std::string versionString;

	unsigned int displayResolutionX, displayResolutionY; // hardware specification

	void flushPreferences();

	std::vector<std::string> highScores;
	unsigned int extractScoreFromLine(std::string scoreString);

	std::vector< std::pair<unsigned int, unsigned int> > getAllowedWindowResolutions();
	void applyScreenResolution(std::string resolution);

	enum FrameRateLimiting {
		LIMIT_VSYNC,
		LIMIT_30,
		LIMIT_60,
		LIMIT_120,
		LIMIT_OFF
	};

};

#endif // GAMESYSTEM_H
