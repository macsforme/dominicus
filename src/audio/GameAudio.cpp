// GameAudio.cpp
// Dominicus

#include "audio/GameAudio.h"

#include <cstdlib>
#include <cstring>
#include <sstream>

#include "core/GameSystem.h"
#include "platform/Platform.h"

extern GameAudio* gameAudio;
extern GameSystem* gameSystem;
extern Platform* platform;

void mixAudio(void* userData, uint8_t* stream, int length) {
	// play the background music
	if(gameAudio->musicPlaying.sound == NULL)
		return;

	if(gameAudio->musicPlaying.position == gameAudio->musicPlaying.sound->length)
		gameAudio->musicPlaying.position = 0;

	uint32_t amount = gameAudio->musicPlaying.sound->length - gameAudio->musicPlaying.position;
	if(amount > length)
		amount = length;

	SDL_MixAudio(
			stream,
			&gameAudio->musicPlaying.sound->buffer[gameAudio->musicPlaying.position],
			amount,
			(int) (gameSystem->getFloat("audioMusicVolume") * gameAudio->musicPlaying.volume * (float) SDL_MIX_MAXVOLUME)
		);

	gameAudio->musicPlaying.position += amount;

	// play audio effects in queue
	size_t i = 0;
	while(i < gameAudio->effectsPlaying.size()) {
		if(gameAudio->effectsPlaying[i].position == gameAudio->effectsPlaying[i].sound->length) {
			gameAudio->effectsPlaying.erase(gameAudio->effectsPlaying.begin() + i);

			continue;
		}

		amount = gameAudio->effectsPlaying[i].sound->length - gameAudio->effectsPlaying[i].position;
		if(amount > length)
			amount = length;

		SDL_MixAudio(
				stream,
				&gameAudio->effectsPlaying[i].sound->buffer[gameAudio->effectsPlaying[i].position],
				amount,
				(int) (gameSystem->getFloat("audioEffectsVolume") * gameAudio->effectsPlaying[i].volume * (float) SDL_MIX_MAXVOLUME)
			);

		gameAudio->effectsPlaying[i].position += amount;

		++i;
	}
}

GameAudio::GameAudio() {
	// initialize the sound device
	SDL_AudioSpec audioDeviceSpec;
	audioDeviceSpec.freq = 44100;
	audioDeviceSpec.format = AUDIO_S16LSB;
	audioDeviceSpec.channels = 2;
	audioDeviceSpec.samples = 512;
	audioDeviceSpec.callback = mixAudio;
	audioDeviceSpec.userdata = NULL;

	if(SDL_OpenAudio(&audioDeviceSpec, NULL) < 0)
		gameSystem->log(GameSystem::LOG_FATAL, "Unable to open the audio device.");

	// load sound files
	loadSound("alterDownEffect", audioDeviceSpec);
	loadSound("alterUpEffect", audioDeviceSpec);
	loadSound("backEffect", audioDeviceSpec);
	loadSound("empEffect", audioDeviceSpec);
	loadSound("explosionEffect", audioDeviceSpec);
	loadSound("menuSong", audioDeviceSpec);
	loadSound("missileEffect", audioDeviceSpec);
	loadSound("playingSong", audioDeviceSpec);
	loadSound("selectEffect", audioDeviceSpec);
	loadSound("shellEffect", audioDeviceSpec);

	musicPlaying.sound = NULL;

	SDL_PauseAudio(0);
}

GameAudio::~GameAudio() {
	SDL_CloseAudio();

	for(std::map<std::string,GameSound>::iterator itr = sounds.begin(); itr != sounds.end(); ++itr)
		free(itr->second.buffer);
}

void GameAudio::loadSound(std::string file, SDL_AudioSpec audioDeviceSpec) {
	SDL_AudioSpec originalAudioSpec;
	uint8_t* buffer;
	uint32_t length;
	std::stringstream fullFilePath;
	fullFilePath << platform->dataPath << "/data/audio/" << file << ".wav";

	if(SDL_LoadWAV(fullFilePath.str().c_str(), &originalAudioSpec, &buffer, &length) == NULL)
		gameSystem->log(GameSystem::LOG_FATAL, std::string("Unable to open audio file " + fullFilePath.str() + "; Error: " + SDL_GetError()));

	SDL_AudioCVT conversionInfo;
	if(
			SDL_BuildAudioCVT(
					&conversionInfo,
					originalAudioSpec.format,
					originalAudioSpec.channels,
					originalAudioSpec.freq,
					audioDeviceSpec.format,
					audioDeviceSpec.channels,
					audioDeviceSpec.freq
				) < 0
		)
		gameSystem->log(GameSystem::LOG_FATAL, std::string("Unable to convert audio file " + fullFilePath.str()));

	conversionInfo.buf = (uint8_t*) malloc(length * conversionInfo.len_mult);
	memcpy(conversionInfo.buf, buffer, length);
	conversionInfo.len = length;
	if(SDL_ConvertAudio(&conversionInfo) < 0)
		gameSystem->log(GameSystem::LOG_FATAL, std::string("Unable to convert audio file " + fullFilePath.str()));
	SDL_FreeWAV(buffer);

	GameSound sound;
	sound.buffer = conversionInfo.buf;
	sound.length = conversionInfo.len_cvt;

	sounds[file] = sound;
}

void GameAudio::setBackgroundMusic(std::string choice) {
	if(sounds.find(choice) == sounds.end())
		gameSystem->log(GameSystem::LOG_FATAL, std::string("Non-existent sound " + choice + " requested."));

	SDL_LockAudio();
	musicPlaying.sound = &sounds[choice];
	musicPlaying.volume = 1.0f;
	musicPlaying.position = 0;
	SDL_UnlockAudio();
}

void GameAudio::playSound(std::string choice, float volume) {
	if(sounds.find(choice) == sounds.end())
		gameSystem->log(GameSystem::LOG_FATAL, std::string("Non-existent sound " + choice + " requested."));

	GameSoundPlaying playingSound;
	playingSound.sound = &sounds[choice];
	playingSound.volume = volume;
	playingSound.position = 0;

	SDL_LockAudio();
	effectsPlaying.push_back(playingSound);
	SDL_UnlockAudio();
}
