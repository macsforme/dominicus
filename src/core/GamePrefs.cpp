/*
 *  GamePrefs.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "core/GamePrefs.h"

GamePrefs::GamePrefs() {
	// ship state
	preferences["shipUpdateFrequency"] = "60.0";	// updates per second
	preferences["shipAccelRate"] = "15.0";	// meters per second
	preferences["shipMaxAccel"] = "15.0";	// top speed, meters per second
	preferences["shipTurnRate"] = "60.0"; // degrees per second
	preferences["shipMaxAngle"] = "45.0";	// degrees
	preferences["shipPropellerRPM"] = "1.0";	// revolutions per second

	// rendering settings
	preferences["renderingDisplayUpdateFrequency"] = "2.0";	// tests per second
	preferences["renderingCapFPS"] = "1.0";
	preferences["renderingFPS"] = "60.0";
	preferences["renderingTerrainTextureRepeat"] = "50.0";
	preferences["renderingNoiseDensity"] = "512.0";
	preferences["renderingNoiseRoughness"] = "0.6";
	preferences["renderingNoiseDepth"] = "4.0";

	// 3D view settings
	preferences["perspectiveNearClip"] = "1.0";
	preferences["perspectiveFarClip"] = "1500.0";
	preferences["perspectiveFOV"] = "30.0";
	preferences["shipFollowDistance"] = "5.0";
	preferences["shipFollowAngle"] = "10.0";
	preferences["shipFollowLagFactor"] = "1.0";	// approximately in seconds how long to catch up

	// input settings
	preferences["inputPollingFrequency"] = "120.0";

	// window settings
	preferences["windowStartFullScreen"] = "false";
	preferences["windowWidth"] = "1024.0";
	preferences["windowHeight"] = "768.0";
	preferences["windowColorDepth"] = "24.0";

	// font settings
	preferences["fontFile"] = "DejaVuSans-Bold.ttf";
	preferences["fontStandardSize"] = "10.0";	// point (1/72 inch)
	preferences["fontHeaderSize"] = "24.0";
	preferences["fontLargeHeaderSize"] = "32.0";

	// GUI settings
	preferences["cursorSize"] = "0.2";	// portion of screen height cursor box takes up
	preferences["cursorThickness"] = "0.005";	// portion of cursor box
	preferences["controlBoxSize"] = "0.7";	// portion of screen height control box takes up
	preferences["controlSpotSize"] = "0.05";	// portion of screen height dead spot takes up
	preferences["elementPadding"] = "0.1";

	// key bindings
	keyBindings[SDLK_k] = "toggleCamera";
	keyBindings[SDLK_f] = "toggleFullScreen";
	keyBindings[SDLK_c] = "toggleFPSCap";
	keyBindings[SDLK_t] = "newTerrain";
	keyBindings[SDLK_w] = "toggleWireFrameMode";
	keyBindings[SDLK_ESCAPE] = "quit";
	keyBindings[SDLK_EQUALS] = "speed+";
	keyBindings[SDLK_MINUS] = "speed-";

	keyBindings[SDLK_RIGHT] = "bankRight";
	keyBindings[SDLK_LEFT] = "bankLeft";
	keyBindings[SDLK_UP] = "up";
	keyBindings[SDLK_DOWN] = "down";
}

// retrieves a literal std::string value for the given preference key
const std::string GamePrefs::getString(std::string key) const {
	if(preferences.find(std::string(key)) != preferences.end()) {
		return preferences.find(std::string(key))->second;
	} else {
		std::stringstream err;
		err << "Game preferences requested on non-existent key \"" << key << "\".";

		programLog->report(ProgramLog::LOG_INTERNALERROR, err.str().c_str());

		// won't reach, but to make compiler happy...
		return std::string("");
	}
}

const float GamePrefs::getFloat(std::string key) const {
	return (float) atof(getString(key).c_str());
}

const bool GamePrefs::getBool(std::string key) const {
	return (getString(key) != "false");
}

// sets preference key to the given value
void GamePrefs::set(std::string value, std::string key) {
	preferences[key] = value;
}

// retrieves the action for the given SDL key code
const std::string GamePrefs::getBinding(SDLKey key) {
	if(keyBindings.find(key) != keyBindings.end())
		return keyBindings[key];
	else
		return "";
}

// sets binding for the given SDL key code to the given action
void GamePrefs::setBinding(std::string value, SDLKey key) {
	keyBindings[key] = value;
}
