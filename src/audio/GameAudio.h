// GameAudio.h
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#ifndef GAMEAUDIO_H
#define GAMEAUDIO_H

// program headers
#include "core/GameSystem.h"
#include "core/MainLoopMember.h"
#include "platform/Platform.h"

// library headers
#include <SDL.h>
#include <sstream>
#include <string>
#include <map>

// global variables
extern GameSystem* gameSystem;
extern Platform* platform;

class GameAudio : public MainLoopMember {
public:
	struct GameSound {
		Uint8* buffer;
		Uint32 position;
		Uint32 length;
	};

	std::map<std::string,GameSound> sounds;

	std::string backgroundMusic;

private:
	void loadSound(std::string file, SDL_AudioSpec audioDeviceSpec);

public:
	GameAudio();
	~GameAudio();

	void setBackgroundMusic(std::string choice);

	unsigned int execute();
};

#endif
