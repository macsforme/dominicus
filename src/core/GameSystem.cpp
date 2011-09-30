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
		const char* description, bool locked = false) {
	StandardEntry entry;

	entry.value = value;
	entry.description = description;
	entry.locked = locked;

	standards[key] = entry;
}

void GameSystem::setStandard(const char* key, Vector4 value,
		const char* description, bool locked = false) {
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
		const char* description, bool locked = false) {
	std::stringstream str;
	str << value;

	setStandard(key, str.str().c_str(), description, locked);
}

void GameSystem::setStandard(const char* key, bool value,
		const char* description, bool locked = false) {
	setStandard(key, (value != false ? "true" : "false"), description, locked);
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

GameSystem::GameSystem() :
		identifier(PROGRAM_IDENTIFIER),
		classification(PROGRAM_CLASSIFICATION),
		architecture(PROGRAM_ARCH_STR) {
	// set the build version string
	std::stringstream versionStream;
	versionStream << PROGRAM_VERSION << "." << PROGRAM_REVISION;
	version = versionStream.str();

	// set the build date string
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
	buildDate = fullDateString;

	// core standards
	setStandard(
			"stateUpdateFrequency",
			120.0f,
			"Number of times per second the core state updates.",
			true
		);

	// input standards
	setStandard(
			"inputPollingFrequency",
			120.0f,
			"Number of times per second to poll the input devices.",
			true
		);
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

	// ship state and control standards
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
			"displayStartFullScreen",
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

	// scene rendering standards
	setStandard(
			"renderingPerspectiveFOV",
			30.0f,
			"Field-of-view angle for perspective projection.",
			true
		);
	setStandard(
			"renderingPerspectiveNearClip",
			1.0f,
			"Near clip distance for perspective projection.",
			true
		);
	setStandard(
			"renderingPerspectiveFarClip",
			1500.0f,
			"Far clip distance for perspective projection.",
			true
		);

	// scene rendering camera standards
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

	// scene rendering effect standards
	setStandard(
			"shipPropellerRPM",
			1.0f,
			"Ship propeller spin rate in revolutions per minute.",
			true
		);
	setStandard(
			"terrainDepth",
			5.0f,
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
	setStandard(
			"hudControlBoxSize",
			75.0f,
			"Size of ship control box in percentage of screen height."
		);
	setStandard(
			"hudControlBoxSpotSize",
			5.0f,
			"Size of ship control box dead spot in percentage of screen height."
		);
	setStandard(
			"hudControlBoxColor",
			Vector4(0.3f, 0.3f, 0.3f, 0.6f),
			"Thickness of cursor in pixels."
		);
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
	setStandard(
			"radarSize",
			35.0f,
			"Size of radar in percentage of screen height."
		);
	setStandard(
			"hudElementMargin",
			40.0f,
			"Space between HUD elements in pixels."
		);
	setStandard(
			"hudContainerPadding",
			12.0f,
			"Space between HUD elements' external border and content in pixels."
		);
	setStandard(
			"hudButtonPadding",
			8.0f,
			"Space between HUD buttons' external border and content in pixels."
		);
	setStandard(
			"hudContainerBorder",
			2.0f,
			"Thickness in pixels of HUD container element borders."
		);
	setStandard(
			"hudContainerInsideColor",
			Vector4(0.031f, 0.075f, 0.184f, 0.752f),
			"Background color of HUD container elements."
		);
	setStandard(
			"hudContainerAltInsideColor",
			Vector4(0.2f, 0.2f, 0.2f, 0.7f),
			"Background color of HUD container elements."
		);
	setStandard(
			"hudContainerHighlightColor",
			Vector4(0.863f, 0.863f, 0.863f, 0.247f),
			"Highlight background color of HUD container elements."
		);
	setStandard(
			"hudContainerBorderColor",
			Vector4(0.918f, 1.0f, 0.945f, 0.714),
			"Border color of HUD container elements."
		);
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

	// font standards
	setStandard(
			"fontFile",
			"FreeSansBold.ttf",
			"Font file to load for use by HUD and menus."
		);
	setStandard(
			"fontSizeSmall",
			14.0f,
			"Font size for standard display in points (1/72 inch)."
		);
	setStandard(
			"fontSizeLarge",
			42.0f,
			"Font size for enlarged display in points (1/72 inch)."
		);
	setStandard(
			"fontColorDark",
			Vector4(0.725f, 0.725f, 0.725f, 1.0f),
			"Dark font color."
		);
	setStandard(
			"fontColorLight",
			Vector4(1.0f, 1.0f, 1.0f, 1.0f),
			"Light font color."
		);
	setStandard(
			"fontColorMedium",
//			Vector4(0.863f, 0.863f, 0.863f, 1.0f),
			Vector4(0.675f, 0.722f, 0.722f, 1.0f),
			"Medium font color."
		);

	// game color standards
	setStandard(
			"colorClear",
//			Vector4(0.0f, 0.0f, 0.0f, 1.0f),
//			Vector4(1.0f, 1.0f, 1.0f, 1.0f),
//			Vector4(0.447058823529412f, 0.407843137254902f, 0.298039215686275f, 1.0f),
//			Vector4(0.3f, 0.3f, 0.3f, 1.0f),
//			Vector4(0.58f, 0.71f, 0.79f, 1.0f),
			Vector4(174.0f / 255.0f, 187.0f / 255.0f, 224.0f / 255.0f, 1.0f),
			"Color of empty space."
		);

	// key bindings
	keyBindings.push_back(std::make_pair("quit", SDLK_F12));
	keyBindings.push_back(std::make_pair("fullScreenToggle", SDLK_F1));
	keyBindings.push_back(std::make_pair("dashboardToggle", SDLK_LSHIFT));
	keyBindings.push_back(std::make_pair("dashboardToggle", SDLK_RSHIFT));
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
	keyBindings.push_back(std::make_pair("cameraSwitch", SDLK_k));
	keyBindings.push_back(std::make_pair("fpsCapToggle", SDLK_c));

	// get the display resolution
	SDL_VideoInfo* vidInfo = (SDL_VideoInfo*) SDL_GetVideoInfo();
	if(vidInfo == NULL)
		log(LOG_FATAL, "Could not obtain screen resolution from SDL.");

	displayResolutionX = (unsigned short int) vidInfo->current_w;
	displayResolutionY = (unsigned short int) vidInfo->current_h;

	// log the build version
	std::stringstream buildInfo;
	buildInfo <<
			"Game Version: " <<
			identifier << " " <<
			version << " " <<
			classification << " " <<
			architecture << " " <<
			buildDate;
	log(LOG_INFO, buildInfo.str().c_str());
}
