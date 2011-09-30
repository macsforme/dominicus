// GameState.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "state/GameState.h"

GameState::GameState() {
	// instantiate the world
	world = new World();

	// create persistent noise textures
	unsigned int noiseDensity = (unsigned int) gameSystem->getFloat("terrainNoiseTextureDensity");
	DiamondSquare noise1(
			noiseDensity,
			gameSystem->getFloat("terrainNoiseTextureRoughness")
		);
	noiseTexture = new Texture(
			noiseDensity,
			noiseDensity,
			Texture::FORMAT_RGB
		);
	for(size_t i = 0; i < (size_t) noiseDensity; ++i)
		for(size_t p = 0; p < (size_t) noiseDensity; ++p)
			noiseTexture->setColorAt(
					i,
					p,
					(uint8_t) (noise1.data[i][p] * 128.0f + 127.0f),
					(uint8_t) (noise1.data[i][p] * 128.0f + 127.0f),
					(uint8_t) (noise1.data[i][p] * 128.0f + 127.0f),
					0xFF
				);

	DiamondSquare noise2(
			noiseDensity,
			gameSystem->getFloat("terrainNoiseTextureRoughness")
		);
	fourDepthNoiseTexture = new Texture(
			noiseDensity,
			noiseDensity,
			Texture::FORMAT_RGB
		);
	for(size_t i = 0; i < (size_t) noiseDensity; ++i)
		for(size_t p = 0; p < (size_t) noiseDensity; ++p)
			fourDepthNoiseTexture->setColorAt(
					i,
					p,
					(uint8_t) (noise2.data[i][p] * 128.0f + 127.0f),
					(uint8_t) (noise2.data[i][p] * 128.0f + 127.0f),
					(uint8_t) (noise2.data[i][p] * 128.0f + 127.0f),
					0xFF
				);
	fourDepthNoiseTexture->setDepth(16);
}

GameState::~GameState() {
	// destroy variables
	delete(world);

	delete(noiseTexture);
	delete(fourDepthNoiseTexture);
}

unsigned int GameState::execute() {
	// update all world aspects
	for(
		std::vector<Ship*>::iterator itr = ships.begin();
		itr != ships.end();
		++itr
		)
		(*itr)->execute();

	// calculate and return sleep time from superclass
	unsigned int frequency = (unsigned int) gameSystem->getFloat("stateUpdateFrequency");
	static const unsigned int idealSleepTime = (
			frequency != 0 ?
			1000 / frequency : 0
		);
	return getSleepTime(idealSleepTime);
}
