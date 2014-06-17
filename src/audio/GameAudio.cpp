// GameAudio.cpp
// Crucible Island

#include "audio/GameAudio.h"

extern GameAudio* gameAudio;

void mixAudio(void* userData, Uint8* stream, int length) {
	GameAudio::GameSound* sound = NULL;
	if(gameAudio->sounds.find(gameAudio->backgroundMusic) != gameAudio->sounds.end())
		sound = &gameAudio->sounds[gameAudio->backgroundMusic];

	if(sound == NULL)
		return;

	if(sound->position == sound->length)
		sound->position = 0;

	Uint32 amount = sound->length - sound->position;
	if(amount > length) amount = length;

	SDL_MixAudio(
			stream,
			&sound->buffer[sound->position],
			amount,
			(int) (gameSystem->getFloat("audioMusicVolume") * (float) SDL_MIX_MAXVOLUME)
		);
	sound->position += amount;
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
	loadSound("menuSong", audioDeviceSpec);
	loadSound("playingSong", audioDeviceSpec);

	backgroundMusic = "";

	SDL_PauseAudio(0);
}

GameAudio::~GameAudio() {
	SDL_CloseAudio();

	for(std::map<std::string,GameSound>::iterator itr = sounds.begin(); itr != sounds.end(); ++itr)
		free(itr->second.buffer);
}

void GameAudio::loadSound(std::string file, SDL_AudioSpec audioDeviceSpec) {
	SDL_AudioSpec originalAudioSpec;
	Uint8* buffer;
	Uint32 length;
	std::stringstream fullFilePath;
	fullFilePath << platform->dataPath << "/data/audio/" << file << ".wav";

	if(SDL_LoadWAV(fullFilePath.str().c_str(), &originalAudioSpec, &buffer, &length) == NULL)
		gameSystem->log(GameSystem::LOG_FATAL, std::string("Unable to open audio file " + fullFilePath.str()));

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

	conversionInfo.buf = (Uint8*) malloc(length * conversionInfo.len_mult);
	memcpy(conversionInfo.buf, buffer, length);
	conversionInfo.len = length;
	if(SDL_ConvertAudio(&conversionInfo) < 0)
		gameSystem->log(GameSystem::LOG_FATAL, std::string("Unable to convert audio file " + fullFilePath.str()));
	SDL_FreeWAV(buffer);

	GameSound sound;
	sound.buffer = conversionInfo.buf;
	sound.position = 0;
	sound.length = conversionInfo.len_cvt;

	sounds[file] = sound;
}

void GameAudio::setBackgroundMusic(std::string choice) {
	SDL_LockAudio();
	backgroundMusic = choice;
	sounds[choice].position = 0;
	SDL_UnlockAudio();
}
