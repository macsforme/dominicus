/*
 *  Terrain.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Terrain.h"

Terrain::Terrain() {
	// randomly generate our input variables within the set limits
	unsigned int density = 128;
	float rough = 0.6f;
	float gradDist = 0.45f;
	float height = 0.5f + (float) (rand() % 100) / 100.0f * 0.5f;

	// generate the initial diamond-square heightmap
	DiamondSquare diamondSquare(density, rough);

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
	for(int p = 0; p < density; ++p) {
		for(int j = 0; j < density; ++j) {
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
					((diamondSquare.data[mapIndex][(int) (angle / 360.0f * (float) density)]	// the value from the heightmap
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
				alphaHeightMap[p][j] = 0;
		}
	}

	// adjust the heightmap for the alpha values
	float comboHeightMap[density][density];

	for(int i = 0; i < density; ++i)
		for(int p = 0; p < density; ++p)
			comboHeightMap[i][p] = (diamondSquare.data[i][p] + 1.0f) / 2.0f *
					(float) alphaHeightMap[i][p];


	// smooth out the bumps with two different algorithms
	float smoothHeightMap[density][density];
	
	for(int i = 0; i < density; ++i) {
		for(int p = 0; p < density; ++p) {
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

	for(int i = 0; i < density; ++i) {
		for(int p = 0; p < density; ++p) {
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

	for(int i = 0; i < density; ++i) {
		for(int p = 0; p < density; ++p) {
			if(smoothHeightMap[i][p] > max)
				max = smoothHeightMap[i][p];
		}
	}

	for(int i = 0; i < density; ++i)
		for(int p = 0; p < density; ++p)
			smoothHeightMap[i][p] = smoothHeightMap[i][p] / max * height;

	// create a mesh with that terrain data
	for(int i = 0; i < density; ++i) {
		for(int p = 0; p < density; ++p) {
			mesh.addVertex(Vector3(
					((float) i / (float) density * 2.0f - 1.0f) * TERRAIN_MAXWIDTH / 2.0f,
					smoothHeightMap[i][p] * TERRAIN_MAXHEIGHT,
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
