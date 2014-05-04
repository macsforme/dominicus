// GameSystem.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "core/GameSystem.h"

bool GameSystem::isStandard(const char* key) const {
	if(standards.find(key) != standards.end())
		return true;
	else
		return false;
}

GameSystem::StandardEntry GameSystem::getStandard(const char* key) {
	return standards[key];
}

std::string GameSystem::getString(const char* key) {
	return getStandard(key).value;
}

Vector4 GameSystem::getColor(const char* key) {
	int colors[4];
	sscanf(getString(key).c_str(), "%2x%2x%2x%2x", &colors[0], &colors[1], &colors[2], &colors[3]);

	return Vector4(
			(float) colors[0] / 255.0f,
			(float) colors[1] / 255.0f,
			(float) colors[2] / 255.0f,
			(float) colors[3] / 255.0f
		);
}

float GameSystem::getFloat(const char* key) {
	return (float) atof(getString(key).c_str());
}

bool GameSystem::getBool(const char* key) {
	return (getString(key) != "false");
}

void GameSystem::setStandard(const char* key, const char* value,
		const char* description, bool locked) {
	StandardEntry entry;

	entry.value = value;
	entry.description = description;
	entry.locked = locked;

	standards[key] = entry;
}

void GameSystem::setStandard(const char* key, Vector4 value,
		const char* description, bool locked) {
	char str[9];
	sprintf(
			str,
			"%02x%02x%02x%02x",
			(int) (value.x * 255.0f),
			(int) (value.y * 255.0f),
			(int) (value.z * 255.0f),
			(int) (value.w * 255.0f)
		);

	setStandard(key, str, description, locked);
}

void GameSystem::setStandard(const char* key, float value,
		const char* description, bool locked) {
	std::stringstream str;
	str << value;

	setStandard(key, str.str().c_str(), description, locked);
}

void GameSystem::setStandard(const char* key, bool value,
		const char* description, bool locked) {
	setStandard(key, (value != false ? "true" : "false"), description, locked);
}

void GameSystem::flushPreferences() {
	platform->setPreference("preferencesVersion", 1.0f);
	platform->setPreference("displayStartFullscreen", (getBool("displayStartFullscreen") == true ? 1.0f : 0.0f));
	platform->setPreference("audioMusicVolume", getFloat("audioMusicVolume"));
	platform->setPreference("audioEffectsVolume", getFloat("audioEffectsVolume"));
	platform->setPreference("gameStartingLevel", getString("gameStartingLevel").c_str());
	if(highScores.size() == 0) {
		platform->setPreference("highScores", "");
	} else {
		std::stringstream stringStream;
		stringStream << "\"";
		for(size_t i = 0; i < highScores.size(); ++i)
			stringStream << (i > 0 ? "\n" : "") << highScores[i].second << "\t" << highScores[i].first;
		stringStream << "\"";
		platform->setPreference("highScores", stringStream.str().c_str());
	}
}

std::vector<SDLKey> GameSystem::getBindingKeys(std::string action) {
	std::vector<SDLKey> keys;

	for(
			KeyBindingStorage::iterator itr = keyBindings.begin();
			itr != keyBindings.end();
			++itr
		)
		if(itr->first == action)
			keys.push_back(itr->second);

	return keys;
}

std::vector<Uint8> GameSystem::getBindingMouseButtons(std::string action) {
	std::vector<Uint8> buttons;

	for(
			MouseButtonBindingStorage::iterator itr = mouseButtonBindings.begin();
			itr != mouseButtonBindings.end();
			++itr
		)
		if(itr->first == action)
			buttons.push_back(itr->second);

	return buttons;
}

void GameSystem::log(LogDetail detail, std::string report) {
	std::stringstream fullReport;

	if(platform != NULL)
		fullReport << platform->getExecMills() << " ";
	else
		fullReport << "0 ";

	if(detail == LOG_INFO)
		fullReport << "INFO: " << report;
	else if(detail == LOG_VERBOSE)
		fullReport << "VERBOSE: " << report;
	else
		fullReport << "FATAL: " << report;

		logLines.push_back(fullReport.str());

	if(detail == LOG_FATAL) {
		Platform::consoleOut(fullReport.str() + "\n");

		exit(1);
	}
}

GameSystem::GameSystem() {
	// set the build version string
	std::stringstream versionStream;
	versionStream <<
			PROGRAM_IDENTIFIER << " " <<
			PROGRAM_VERSION << " " <<
			"(" << PROGRAM_BUILDSTRING << ") " <<
			PROGRAM_ARCH_STR << " ";

	const char* dateString = __DATE__;

	std::string monthString = std::string(dateString).substr(0, 3);
	int month = 0;
	if(strcmp(monthString.c_str(), "Jan") == 0) month = 1;
	else if(strcmp(monthString.c_str(), "Feb") == 0) month = 2;
	else if(strcmp(monthString.c_str(), "Mar") == 0) month = 3;
	else if(strcmp(monthString.c_str(), "Apr") == 0) month = 4;
	else if(strcmp(monthString.c_str(), "May") == 0) month = 5;
	else if(strcmp(monthString.c_str(), "Jun") == 0) month = 6;
	else if(strcmp(monthString.c_str(), "Jul") == 0) month = 7;
	else if(strcmp(monthString.c_str(), "Aug") == 0) month = 8;
	else if(strcmp(monthString.c_str(), "Sep") == 0) month = 9;
	else if(strcmp(monthString.c_str(), "Oct") == 0) month = 10;
	else if(strcmp(monthString.c_str(), "Nov") == 0) month = 11;
	else month = 12;

	const int day = atoi(std::string(dateString).substr(4, 2).c_str());
	const int year = atoi(std::string(dateString).substr(7, 4).c_str());

	char fullDateString[11];
	sprintf(fullDateString, "%04i-%02i-%02i", year, month, day);
	versionStream << fullDateString;
	versionString = versionStream.str();

	// state standards
	setStandard(
			"stateUpdateFrequency",
			120.0f,
			"Number of times per second the core state updates.",
			true
		);
	setStandard(
			"stateShipOrbitMargin",
			1000.0f,
			"Radius of margin between maximum edge of island and first ship orbit.",
			true
		);
	setStandard(
			"stateShipMargin",
			150.0f,
			"Lateral distance between ships orbiting island.",
			true
		);
	setStandard(
			"stateShipSpeed",
			200.0f,
			"Ship speed.",
			true
		);
	setStandard(
			"stateShipEntryTime",
//			20.0f,
			10.0f,
			"Time it takes between ship introduction and beginning of orbit.",
			true
		);
	setStandard(
			"stateShipAddRate",
//			20.0f,
			3.0f,
			"Time between ships being added to the world.",
			true
		);

	// input standards
	setStandard(
			"inputPollingFrequency",
			120.0f,
			"Number of times per second to poll the input devices.",
			true
		);
/*
	setStandard(
			"inputAllowedCallsignChars",
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_ ",
			"Characters allowed to be used in callsigns.",
			true
		);
	setStandard(
			"inputAllowedPasswordChars",
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890",
			"Characters allowed to be used in passwords.",
			true
		);
*/
	// ship state and control standards
/*
	setStandard(
			"shipUpdateFrequency",
			60.0f,
			"Frequency per second of ship position updates by internal state.",
			true
		);
	setStandard(
			"shipAccelRate",
			15.0f,
			"Maximum ship acceleration rate in meters per second.",
			true
		);
	setStandard(
			"shipMaxSpeed",
			15.0f,
			"Maximum ship speed in meters per second.",
			true
		);
	setStandard(
			"shipTurnRate",
			60.0f,
			"Maximum angle a ship can turn in degrees per second.",
			true
		);
	setStandard(
			"shipMaxTilt",
			60.0f,
			"Maximum pitch or roll a ship can tilt in degrees.",
			true
		);
*/
	// display and drawing standards
	setStandard(
			"displayFPSCap",
			true,
			"Whether or not to cap the frames per second to a certain number."
		);
	setStandard(
			"displayFPS",
			60.0f,
			"Number of frames per second to draw."
		);
	setStandard(
			"displayStartFullscreen",
			false,
			"Whether or not to start the program in full screen mode."
		);
	setStandard(
			"displayWindowedResolutionX",
			1024.0f,
			"Windowed horizontal display resolution."
		);
	setStandard(
			"displayWindowedResolutionY",
			768.0f,
			"Windowed vertical display resolution."
		);
	setStandard(
			"displayColorDepth",
			24.0f,
			"Color depth of display (may only affect full screen mode)."
		);
	setStandard(
			"colorClear",
			Vector4(174.0f / 255.0f, 187.0f / 255.0f, 224.0f / 255.0f, 1.0f),
			"Color of empty space."
		);

	// scene rendering standards

	setStandard(
			"renderingPerspectiveFOV",
			30.0f,
			"Field-of-view angle for perspective projection.",
			true
		);
	setStandard(
			"renderingPerspectiveNearClip",
			0.5f,
			"Near clip distance for perspective projection.",
			true
		);
	setStandard(
			"renderingPerspectiveFarClip",
			20000.0f,
//			6000.0f,
			"Far clip distance for perspective projection.",
			true
		);

	// scene rendering camera standards
/*
	setStandard(
			"renderingCameraFollowDistance",
			5.0f,
			"Following distance of ship camera.",
			true
		);
	setStandard(
			"renderingCameraAngle",
			10.0f,
			"Following angle from horizon of ship camera.",
			true
		);
	setStandard(
			"renderingCameraLag",
			1.0f,
			"How many seconds worth of turning the camera should lag behind the ship.",
			true
		);
*/
	// scene rendering effect standards
/*
	setStandard(
			"shipPropellerRPM",
			1.0f,
			"Ship propeller spin rate in revolutions per minute.",
			true
		);
*/
	setStandard(
			"terrainDepth",
			10.0f,
			"How far below the water the ground extends.",
			true
		);
	setStandard(
			"terrainTextureRepeat",
			50.0f,
			"Number of times to repeat the ground texture over the maximum surface area.",
			true
		);
	setStandard(
			"terrainNoiseTextureDensity",
			512.0f,
			"Terrain mixing noise texture resolution.",
			true
		);

	setStandard(
			"terrainNoiseTextureRoughness",
			0.6f,
			"Terrain mixing noise texture roughness factor.",
			true
		);
	setStandard(
			"terrainNoiseTextureDepth",
			4.0f,
			"Terrain mixing noise texture color depth.",
			true
		);

	// HUD standards
	setStandard(
			"hudFPSTestFrequency",
			1.0f,
			"Frequency per second of the FPS test."
		);
/*
	setStandard(
			"hudCursorSize",
			100.0f,
			"Height of cursor in pixels."
		);
	setStandard(
			"hudCursorThickness",
			2.0f,
			"Thickness of cursor in pixels."
		);
	setStandard(
			"hudCursorColor",
			Vector4(1.0f, 1.0f, 1.0f, 1.0f),
			"Thickness of cursor in pixels."
		);
*/
	setStandard(
			"hudControlBoxSize",
			75.0f,
			"Size of ship control box in percentage of screen height."
		);
	setStandard(
			"hudControlBoxCornerSize",
			50.0f,
			"Percentage of control box quadrant which curved portion should take up."
		);
	setStandard(
			"hudControlBoxSpotSize",
			10.0f,
			"Size of ship control box dead spot in percentage of screen height."
		);
	setStandard(
			"hudControlBoxColor",
			Vector4(0.3f, 0.3f, 0.3f, 0.6f),
			"Thickness of cursor in pixels."
		);
/*
	setStandard(
			"hudConsoleMaxLines",
			8.0f,
			"Maximum number of lines of text the console will display."
		);
	setStandard(
			"hudConsoleLineExpiration",
			5.0f,
			"Number of seconds to display a given line on the console."
		);
*/
	setStandard(
			"radarSize",
			35.0f,
			"Size of radar panel in percentage of screen height."
		);
	setStandard(
			"hudElementMargin",
			36.0f,
			"Space between HUD elements in pixels (must be even number)."
		);
/*
	setStandard(
			"hudContainerPadding",
			12.0f,
			"Space between HUD elements' external border and content in pixels."
		);
*/
	setStandard(
			"hudButtonPadding",
			12.0f,
			"Space between HUD buttons' external border and content in pixels."
		);
	setStandard(
			"hudBigButtonPadding",
			16.0f,
			"Space between large HUD buttons' external border and content in pixels."
		);
	setStandard(
			"hudContainerBorder",
			2.0f,
			"Thickness in pixels of HUD container element borders."
		);
	setStandard(
			"hudContainerSoftEdge",
			2.0f,
			"Thickness in pixels of HUD container element border antialiased portion."
		);
	setStandard(
			"hudContainerInsideColor",
			Vector4(0.15f, 0.15f, 0.15f, 0.75f),
			"Background color of HUD container elements."
		);
	setStandard(
			"hudContainerHighlightColor",
			Vector4(0.863f, 0.863f, 0.863f, 0.247f),
			"Highlight background color of HUD container elements."
	);
	setStandard(
			"hudContainerBorderColor",
			Vector4(0.918f, 1.0f, 0.945f, 0.714f),
			"Border color of HUD container elements."
		);
	setStandard(
			"hudContainerOutsideColor",
			Vector4(0.918f, 1.0f, 0.945f, 0.0f),
			"Outside color of HUD container elements."
		);
/*
	setStandard(
			"hudFieldWidth",
			20.0f,
			"Standard field width (in number of '#' characters)."
		);
	setStandard(
			"hudFieldInactiveColor",
			Vector4(0.031f, 0.075f, 0.184f, 0.752f),
			"Background color for inactive text fields."
		);
	setStandard(
			"hudFieldActiveColor",
			Vector4(0.0f, 0.0f, 1.0f, 1.0f),
			"Background color for active text fields."
		);
*/
	setStandard(
			"hudGaugePadding",
			20.0f,
			"Gauge panel padding in pixels."
		);
	setStandard(
			"hudGaugeWidth",
			200.0f,
			"Width of gauges in pixels."
		);
	setStandard(
			"hudGaugeHeight",
			30.0f,
			"Height of gauges in pixels."
		);
	setStandard(
			"hudGaugeBackgroundColor",
			Vector4(0.3f, 0.3f, 0.3f, 1.0f),
			"Background color of gauges."
		);
	setStandard(
			"hudGaugeColorFalloff",
			Vector4(0.3f, 0.3f, 0.3f, 0.75f),
			"Factor to be multiplied into gauge color for falloff at bottom."
		);
	setStandard(
			"hudGaugeHealthBarColor",
			Vector4(0.0f, 1.0f, 0.0f, 1.0f),
			"Color of health gauge."
		);
	setStandard(
			"hudGaugeAmmoBarColor",
			Vector4(0.0f, 1.0f, 1.0f, 1.0f),
			"Color of ammunition gauge."
		);
	setStandard(
			"hudGaugeShockChargingBarColor",
			Vector4(1.0f, 1.0f, 0.0f, 1.0f),
			"Color of shock gauge while charging."
		);
	setStandard(
			"hudGaugeShockChargedBarColor",
			Vector4(1.0f, 0.0f, 0.0f, 1.0f),
			"Color of shock gauge when charged."
		);

	// font standards
	setStandard(
			"fontFile",
			"FreeSansBold.ttf",
			"Font file to load for use by HUD and menus."
		);
	setStandard(
			"fontSizeSmall",
			16.0f,
			"Font size for small display in points (1/72 inch)."
		);
	setStandard(
			"fontSizeMedium",
			24.0f,
			"Font size for standard display in points (1/72 inch)."
		);
	setStandard(
			"fontSizeLarge",
			34.0f,
			"Font size for enlarged display in points (1/72 inch)."
		);
	setStandard(
			"fontSizeSuper",
			46.0f,
			"Font size for title display in points (1/72 inch)."
		);
	setStandard(
			"fontColorLight",
			Vector4(1.0f, 1.0f, 1.0f, 1.0f),
			"Light font color."
		);
	setStandard(
			"fontColorDark",
			Vector4(0.5f, 0.5f, 0.5f, 1.0f),
			"Medium font color."
		);

	// audio standards
	setStandard(
			"audioTickRate",
			50.0f,
			"Audio manager tick rate."
		);
	setStandard(
			"audioMusicVolume",
			0.5f,
			"Music volume."
		);
	setStandard(
			"audioEffectsVolume",
			0.5f,
			"Audio effects volume."
		);

	// general game standards
	setStandard(
			"gameStartingLevel",
			"Easy",
			"Starting difficulty level."
		);
	setStandard(
			"islandMaximumWidth",
			1000.0f,
			"Maximum island width."
		);
	setStandard(
			"islandMaximumHeight",
			100.0f,
			"Maximum island height."
		);
	setStandard(
			"islandTerrainDensity",
			256.0f,
			"Density of island terrain tessellation."
		);
	setStandard(
			"islandTerrainRoughness",
			0.5f,
			"Roughness of island terrain randomization."
		);



	setStandard(
			"islandTerrainGradDist",
			0.5f,
			"Island terrain generation gradual distance factor."
		);
	setStandard(
			"islandTerrainBlends",
			4.0f,
			"Island terrain generation blending factor."
		);
	setStandard(
			"islandTerrainSink",
			0.5f,
			"Island terrain generation sink to sea level factor."
		);

	// key bindings
//	keyBindings.push_back(std::make_pair("quit", SDLK_F12));
//	keyBindings.push_back(std::make_pair("fullScreenToggle", SDLK_F1));
//	keyBindings.push_back(std::make_pair("dashboardToggle", SDLK_LSHIFT));
//	keyBindings.push_back(std::make_pair("dashboardToggle", SDLK_RSHIFT));
//	keyBindings.push_back(std::make_pair("cancel", SDLK_ESCAPE));
//	keyBindings.push_back(std::make_pair("playerInfoDisplayToggle", SDLK_F3));
//	keyBindings.push_back(std::make_pair("radarDisplayToggle", SDLK_F4));
//	keyBindings.push_back(std::make_pair("debugInfoDisplayToggle", SDLK_F5));
//	keyBindings.push_back(std::make_pair("accelerate", SDLK_SPACE));
//	keyBindings.push_back(std::make_pair("firePrimary", SDLK_3));
//	keyBindings.push_back(std::make_pair("fireSecondary", SDLK_4));
//	keyBindings.push_back(std::make_pair("yawRight", SDLK_RIGHT));
//	keyBindings.push_back(std::make_pair("yawLeft", SDLK_LEFT));
//	keyBindings.push_back(std::make_pair("pitchUp", SDLK_UP));
//	keyBindings.push_back(std::make_pair("pitchDown", SDLK_DOWN));

	// temporary key bindings for development
//	keyBindings.push_back(std::make_pair("cameraSwitch", SDLK_k));
//	keyBindings.push_back(std::make_pair("fpsCapToggle", SDLK_c));
/*
std::pair<unsigned int, std::string> highScoresPair;
highScoresPair = std::make_pair(10, "kierra"); highScores.push_back(highScoresPair);
highScoresPair = std::make_pair(9, "snick"); highScores.push_back(highScoresPair);
highScoresPair = std::make_pair(8, "orbit"); highScores.push_back(highScoresPair);
highScoresPair = std::make_pair(7, "contamination"); highScores.push_back(highScoresPair);
highScoresPair = std::make_pair(6, "Bulldozer"); highScores.push_back(highScoresPair);
highScoresPair = std::make_pair(5, "Augustus"); highScores.push_back(highScoresPair);
highScoresPair = std::make_pair(4, "Constitution"); highScores.push_back(highScoresPair);
highScoresPair = std::make_pair(3, "Wasp^32"); highScores.push_back(highScoresPair);
highScoresPair = std::make_pair(2, "Hornet"); highScores.push_back(highScoresPair);
highScoresPair = std::make_pair(1, "Josh"); highScores.push_back(highScoresPair);
*/
//highScores.clear();

	// load standards from preferences (or save standard preferences if no file)
	if(platform->getPreferenceFloat("preferencesVersion") == 1.0f) {
		setStandard("displayStartFullscreen", (platform->getPreferenceFloat("displayStartFullscreen")  == 1.0f ? true : false), "");
		setStandard("audioMusicVolume", platform->getPreferenceFloat("audioMusicVolume"), "");
		setStandard("audioEffectsVolume", platform->getPreferenceFloat("audioEffectsVolume"), "");
		setStandard("gameStartingLevel", platform->getPreferenceString("gameStartingLevel").c_str(), "");
		std::string highScoresString = platform->getPreferenceString("highScores");
		size_t i = highScoresString.find('\t');
		while(i != std::string::npos) {
			std::string highScoreName = highScoresString.substr(0, i);
			highScoresString = highScoresString.substr(i + 1);
			i = highScoresString.find('\n');
			std::string highScoreScore = highScoresString.substr(0, i);
			highScoresString = highScoresString.substr(i + 1);
			i = highScoresString.find('\t');
			highScores.push_back(std::make_pair((unsigned int) atoi(highScoreScore.c_str()), highScoreName));
		}
	} else {
		flushPreferences();
	}

	// get the display resolution
	SDL_VideoInfo* vidInfo = (SDL_VideoInfo*) SDL_GetVideoInfo();
	if(vidInfo == NULL)
		log(LOG_FATAL, "Could not obtain screen resolution from SDL.");

	displayResolutionX = (unsigned short int) vidInfo->current_w;
	displayResolutionY = (unsigned short int) vidInfo->current_h;

	// log the build version
	std::stringstream buildInfo;
	buildInfo << "Game Version: " << versionString;
	log(LOG_INFO, buildInfo.str().c_str());
}
