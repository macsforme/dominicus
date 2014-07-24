// GameAudio.h
// Crucible Island

#ifndef GAMEAUDIO_H
#define GAMEAUDIO_H

// program headers
#include "core/GameSystem.h"
#include "platform/Platform.h"

// library headers
#include <SDL.h>
#include <sstream>
#include <string>
#include <map>

// global variables
extern GameSystem* gameSystem;
extern Platform* platform;

class GameAudio {
public:
	struct GameSound {
		Uint8* buffer;
		Uint32 length;
	};

	std::map<std::string,GameSound> sounds;

	struct GameSoundPlaying {
		GameSound* sound;
		float volume;
		Uint32 position;
	};

	GameSoundPlaying musicPlaying;
	std::vector<GameSoundPlaying> effectsPlaying;

private:
	void loadSound(std::string file, SDL_AudioSpec audioDeviceSpec);

public:
	GameAudio();
	~GameAudio();

	void setBackgroundMusic(std::string choice);
	void playSound(std::string choice, float volume = 1.0f);
};

#endif // GAMEAUDIO_H
