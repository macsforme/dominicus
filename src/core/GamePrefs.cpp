/*
 *  GamePrefs.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "GamePrefs.h"

GamePrefs::GamePrefs() {
	// ship state
	preferences["shipUpdateFrequency"] = "60";	// updates per second
	preferences["shipAccelRate"] = "15";	// meters per second
	preferences["shipMaxAccel"] = "15";	// top speed, meters per second
	preferences["shipTurnRate"] = "60"; // degrees per second
	preferences["shipMaxAngle"] = "45";	// degrees

	// rendering settings
	preferences["renderingDisplayUpdateFrequency"] = "2";	// tests per second
	preferences["renderingCapFPS"] = "1";
	preferences["renderingFPS"] = "60";

	// 3D view settings
	preferences["perspectiveNearClip"] = "1.0";
	preferences["perspectiveFarClip"] = "1500.0";
	preferences["perspectiveFOV"] = "30.0";
	preferences["shipFollowDistance"] = "5";
	preferences["shipFollowAngle"] = "10";
	preferences["shipFollowLagFactor"] = "1.0";	// approximately in seconds how long to catch up

	// input settings
	preferences["inputPollingFrequency"] = "120";

	// window settings
	preferences["windowStartFullScreen"] = "0";
	preferences["windowWidth"] = "1024";
	preferences["windowHeight"] = "768";
	preferences["windowColorDepth"] = "24";

	// font settings
	preferences["fontFile"] = "DejaVuSans-Bold.ttf";
	preferences["fontStandardSize"] = "10";	// point (1/72 inch)
	preferences["fontHeaderSize"] = "24";
	preferences["fontLargeHeaderSize"] = "32";

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

const std::string GamePrefs::getString(std::string key) const {
	// find the setting and return it
	if(preferences.find(std::string(key)) != preferences.end())
		return preferences.find(std::string(key))->second;
	else
		return std::string("");
}

const int GamePrefs::getInt(std::string key) const {
	return atoi(getString(key).c_str());
}

const float GamePrefs::getFloat(std::string key) const {
	return atof(getString(key).c_str());
}

const bool GamePrefs::getBool(std::string key) const {
	return (getString(key) != "0");
}

void GamePrefs::set(std::string value, std::string key) {
	preferences[key] = value;
}

const std::string GamePrefs::getBinding(SDLKey key) {
	if(keyBindings.find(key) != keyBindings.end())
		return keyBindings[key];
	else
		return "";
}

void GamePrefs::setBinding(std::string value, SDLKey key) {
	keyBindings[key] = value;
}
