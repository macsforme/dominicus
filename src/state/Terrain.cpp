/*
 * Terrain.cpp
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

#include "state/Terrain.h"

Terrain::Terrain() {
	// randomly generate our input variables within the set limits
	const unsigned int dsFactor = 64;
	const unsigned int ssFactor = 2;
	float rough = 0.6f;
	float gradDist = 0.45f;
	float height = 0.5f + (float) (rand() % 100) / 100.0f * 0.5f;

	// generate the initial diamond-square heightmap
	DiamondSquare diamondSquare(dsFactor, rough);

	// do sine-wave smoothing
	const unsigned int density = dsFactor * ssFactor;
	float sineDSHeightMap[density][density];

	for(unsigned int i = 0; i < dsFactor; ++i) {
		for(unsigned int p = 0; p < dsFactor; ++p) {
			// create a mesh with that terrain data
			float xPlusWave[ssFactor];
			float xMinusWave[ssFactor];
			float zPlusWave[ssFactor];
			float zMinusWave[ssFactor];

			float seValue = diamondSquare.data[i][p];
			float swValue = diamondSquare.data[i + 1 == dsFactor ? 0 : i + 1][p];
			float neValue = diamondSquare.data[i][p + 1 == dsFactor ? 0 : p + 1];
			float nwValue = diamondSquare.data[i + 1 == dsFactor ? 0 : i + 1]
					[p + 1 == dsFactor ? 0 : p + 1];

			for(unsigned int j = 0; j < ssFactor; ++j) {
				float jFactor = (float) j / (float) ssFactor;

				xPlusWave[j] = swValue + sin(radians(90.0f * jFactor)) * (nwValue - swValue);
				xMinusWave[j] = seValue + sin(radians(90.0f * jFactor)) * (neValue - seValue);
				zPlusWave[j] = neValue + sin(radians(90.0f * jFactor)) * (nwValue - neValue);
				zMinusWave[j] = seValue + sin(radians(90.0f * jFactor)) * (swValue - seValue);

//				xPlusWave[j] = swValue + jFactor * (nwValue - swValue);
//				xMinusWave[j] = seValue + jFactor * (neValue - seValue);
//				zPlusWave[j] = neValue + jFactor * (nwValue - neValue);
//				zMinusWave[j] = seValue + jFactor * (swValue - seValue);
			}

			for(unsigned int j = 0; j < ssFactor; ++j) {
				for(unsigned int k = 0; k < ssFactor; ++k) {
					sineDSHeightMap[i * ssFactor + j][p * ssFactor + k] = (
							zPlusWave[j] * (float) k / (float) ssFactor +
							zMinusWave[j] * ((float) ssFactor - (float)k) / (float) ssFactor +
							xPlusWave[k] * (float) j / (float) ssFactor +
							xMinusWave[k] * ((float) ssFactor - (float) j) / (float) ssFactor
						) / 4.0f;
				}
			}
		}
	}

	// generate an "alphaBump" mappings for height variance in island
	float alphaHeightMap[density][density];

	// create a middle point for where to start the bump
	Vector2 midpoint = Vector2(0.0f, 0.0f);	// if there's only one, center it

	// determine the bump radius based on distance to edge
	Vector2 posCoord(absolute(midpoint.x), absolute(midpoint.y));

	float radius = minimum(1.0f - posCoord.x, 1.0f - posCoord.y);

	// select one random row from the heightmap for randomness
	unsigned int mapIndex = (rand() % 100) * density / 100;

	// fill the mask with values
	for(unsigned int p = 0; p < density; ++p) {
		for(unsigned int j = 0; j < density; ++j) {
			// coordinate of these indices
			Vector2 coord(
				(float) p / (float) density * 2.0f - 1.0f,
				(float) j / (float) density * 2.0f - 1.0f
			);

			// calculate the angle formed by a right triangle with the centerpoint and this one
			Vector2 point(coord);
			point -= midpoint;
			point.norm();
			Vector2 zeroAngle(1.0f, 0.0f);
			float angle = degrees(acos(dot(zeroAngle, point)));
			if(point.y < 0.0f)
				angle = 360.0f - angle;

			// number of variation levels we allow from incoming data
			int levels = 128;

			// vary the circumference
			float dist = distance(coord, midpoint);

			dist +=
					(float)
					((int)	// precision reduction
					((sineDSHeightMap[mapIndex][(int) (angle / 360.0f * (float) density)]	// the value from the heightmap
					+ 1.0f)	// bumped up to positive-only values
					/ 2.0f	// divided back down to 0.0 - 1.0 range
					* levels))	// precision reduction to 0-levels
					/ (float) levels	// back down to float, 0-1.0 range
					/ 2.0f	// final range 0-0.5
					;

			if(dist < radius - gradDist)
					alphaHeightMap[p][j] = 1.0f;
			else if(dist < radius)
					alphaHeightMap[p][j] = (radius - dist) / gradDist;
			else
				alphaHeightMap[p][j] = 0.0f;
		}
	}

	// adjust the heightmap for the alpha values
	float comboHeightMap[density][density];

	for(unsigned int i = 0; i < density; ++i)
		for(unsigned int p = 0; p < density; ++p)
			comboHeightMap[i][p] = (sineDSHeightMap[i][p] + 1.0f) / 2.0f *
					(float) alphaHeightMap[i][p];

	// smooth out the bumps with two different algorithms
	float smoothHeightMap[density][density];

	for(unsigned int i = 0; i < density; ++i) {
		for(unsigned int p = 0; p < density; ++p) {
			unsigned int
						nCoord = (p + 1 == density ? 0 : p + 1),
						sCoord = (p == 0 ? density - 1 : p - 1),
						wCoord = (i == 0 ? density - 1 : i - 1),
						eCoord = (i + 1 == density ? 0 : i + 1);

			smoothHeightMap[i][p] =
					comboHeightMap[i][p] * 0.25f +
					(
							comboHeightMap[wCoord][sCoord] +
							comboHeightMap[wCoord][nCoord] +
							comboHeightMap[eCoord][nCoord] +
							comboHeightMap[eCoord][sCoord]
						) / 4.0f * 0.25f;
		}
	}

	for(unsigned int i = 0; i < density; ++i) {
		for(unsigned int p = 0; p < density; ++p) {
			unsigned int
					nCoord = (p + 1 == density ? 0 : p + 1),
					sCoord = (p == 0 ? density - 1 : p - 1),
					wCoord = (i == 0 ? density - 1 : i - 1),
					eCoord = (i + 1 == density ? 0 : i + 1);

			float avg =
					comboHeightMap[i][p] * 0.0f + (
							comboHeightMap[wCoord][sCoord] +
							comboHeightMap[wCoord][nCoord] +
							comboHeightMap[eCoord][nCoord] +
							comboHeightMap[eCoord][sCoord]
						) / 4.0f * 1.0f;

			if(avg < comboHeightMap[i][p])
				smoothHeightMap[i][p] += avg * 0.5f;
			else
				smoothHeightMap[i][p] += comboHeightMap[i][p] * 0.5f;
		}
	}

	// re-map all the values to 0.0f - 1.0
	float max = 0.0f;

	for(unsigned int i = 0; i < density; ++i) {
		for(unsigned int p = 0; p < density; ++p) {
			if(smoothHeightMap[i][p] > max)
				max = smoothHeightMap[i][p];
		}
	}

	for(unsigned int i = 0; i < density; ++i)
		for(unsigned int p = 0; p < density; ++p)
			comboHeightMap[i][p] = comboHeightMap[i][p] / max * height;

	// create a mesh with that terrain data
	for(unsigned int i = 0; i < density; ++i) {
		for(unsigned int p = 0; p < density; ++p) {
			mesh.addVertex(Vector3(
					((float) i / (float) density * 2.0f - 1.0f) * TERRAIN_MAXWIDTH / 2.0f,
					comboHeightMap[i][p] * TERRAIN_MAXHEIGHT,
					((float) p / (float) density * 2.0f - 1.0f) * TERRAIN_MAXWIDTH / 2.0f
				));

			if(i > 0 && p > 0) {
				mesh.addFace(
						i - 1 + (p * density),
						i - 1 + ((p - 1) * density),
						i + ((p - 1) * density)
					);
//				mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);

				mesh.addFace(
						i + ((p - 1) * density),
						i + p * density,
						i - 1 + (p * density)
					);
//				mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
			}
		}
	}
}
