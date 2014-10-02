// GameAudio.h
// Crucible Island

#ifndef GAMEAUDIO_H
#define GAMEAUDIO_H

#include <map>
#include <SDL.h>
#include <string>
#include <vector>

class GameAudio {
public:
	struct GameSound {
		uint8_t* buffer;
		uint32_t length;
	};

	std::map<std::string,GameSound> sounds;

	struct GameSoundPlaying {
		GameSound* sound;
		float volume;
		uint32_t position;
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
