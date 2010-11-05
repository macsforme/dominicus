/*
 *  GamePrefs.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef GAMEPREFS_H
#define GAMEPREFS_H

// library headers
#include <map>
#include <string>
#include <SDL/SDL.h>

class GamePrefs {
private:
	std::map<std::string,std::string> preferences;
	std::map<SDLKey, std::string> keyBindings;
	std::map<Uint8, std::string> mouseButtonBindings;

public:
	GamePrefs();

	// preferences
	const std::string getString(std::string key) const;
	const int getInt(std::string key) const;
	const float getFloat(std::string key) const;
	const bool getBool(std::string key) const;
	void set(std::string value, std::string key);

	// key bindings
	const std::string getBinding(SDLKey key);
	void setBinding(std::string value, SDLKey key);
};

#endif
