/*
 * GameState.cpp
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

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
