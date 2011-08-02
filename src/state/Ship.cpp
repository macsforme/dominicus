/*
 * Ship.cpp
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

#include "state/Ship.h"

Ship::Ship() :
		team(TEAM_FOREST),
		power(1.0f),
		position(Vector3(0.0f, 0.0f, 0.0f)),
		direction(Vector3(0.0f, 0.0f, 1.0f)),
		pitch(0.0f),
		roll(0.0f),
		speed(0.0f),
		acceleration(false),
		steering(Vector2(0.0f, 0.0f)),
		lastUpdate(platform->getExecMills()) { }

void Ship::execute() {
	#define SHIPRADIUS 0.5f

	// get our delta T
	unsigned int dt = platform->getExecMills() - lastUpdate;
	lastUpdate += dt;

	// normalize the direction
	direction.norm();

	// update roll/pitch based on steering
	float maxTurn = gameSystem->getFloat("shipTurnRate") * ((float) dt / 1000.0f);

	float rollDiff = ((steering.x * gameSystem->getFloat("shipMaxTilt")) - roll);

	if(absolute(rollDiff) < gameSystem->getFloat("shipMaxTilt"))
		rollDiff *= absolute(rollDiff) / gameSystem->getFloat("shipMaxTilt");

	if(absolute(rollDiff) > maxTurn)
		rollDiff = (rollDiff > 0.0f ? maxTurn : -maxTurn);

	roll += rollDiff;

	float pitchDiff = ((steering.y * gameSystem->getFloat("shipMaxTilt")) - pitch);

	if(absolute(pitchDiff) < gameSystem->getFloat("shipMaxTilt"))
		pitchDiff *= absolute(pitchDiff) / gameSystem->getFloat("shipMaxTilt");

	if(absolute(pitchDiff) > maxTurn)
		pitchDiff = (pitchDiff > 0.0f ? maxTurn : -maxTurn);

	pitch += pitchDiff;

	Vector3 xzDirection = Vector3(direction.x, 0.0f, direction.z);
	xzDirection.norm();

	Matrix3 orientation = completeMatrix(xzDirection);
	orientation = Matrix3(
			-orientation.m31, orientation.m32, -orientation.m33,
			orientation.m21, orientation.m22, orientation.m23,
			orientation.m11, orientation.m12, orientation.m13
		);

	rotateMatrix(
			Vector3(orientation.m11, orientation.m12, orientation.m13),
			radians(pitch),
			orientation
		);
	rotateMatrix(
			Vector3(orientation.m21, orientation.m22, orientation.m23),
			radians(roll / gameSystem->getFloat("shipMaxTilt") * gameSystem->getFloat("shipTurnRate") * (float) dt / 1000.0f),
			orientation
		);

	direction = Vector3(orientation.m31, orientation.m32, orientation.m33);

	// if we're not moving, we're done
	if(speed == 0.0f) return;

	// for collision detection, ask the world quadtree for a list of faces with vertices near our position
	std::vector<size_t> faceIndices = gameState->world->collisionTree->getFaces(
			Vector2(position.x, position.z),
			SHIPRADIUS + mag(direction * speed * dt / 1000.0f) + gameState->world->longestEdge / 2.0f
		);

	std::vector<Mesh::Face> collisionFaces;
	for(size_t i = 0; i < faceIndices.size(); ++i)
		collisionFaces.push_back(gameState->world->mesh.faceGroups.begin()->second[faceIndices[i]]);

static int loopCount = 0;

	// do collision detection against terrain faces
	for(
			std::vector<Mesh::Face>::iterator itr = collisionFaces.begin();
			itr != collisionFaces.end();
			++itr

//			std::vector<Mesh::Face>::iterator itr = gameState->world->mesh.faceGroups.begin()->second.begin();
//			itr != gameState->world->mesh.faceGroups.begin()->second.end();
//			++itr
		) {
		Vector3 movement = direction * speed * dt / 1000.0f;

		// see if we're really close enough to this face to necessitate a test
		Vector3 triDist(
				(gameState->world->mesh.vertices[itr->vertices[0]].x +
				gameState->world->mesh.vertices[itr->vertices[1]].x +
				gameState->world->mesh.vertices[itr->vertices[2]].x) / 3.0f,

				(gameState->world->mesh.vertices[itr->vertices[0]].y +
				gameState->world->mesh.vertices[itr->vertices[1]].y +
				gameState->world->mesh.vertices[itr->vertices[2]].y) / 3.0f,

				(gameState->world->mesh.vertices[itr->vertices[0]].z +
				gameState->world->mesh.vertices[itr->vertices[1]].z +
				gameState->world->mesh.vertices[itr->vertices[2]].z) / 3.0f
			);

		float sideLengths[] = {
				mag(gameState->world->mesh.vertices[itr->vertices[0]] -
						gameState->world->mesh.vertices[itr->vertices[1]]),
				mag(gameState->world->mesh.vertices[itr->vertices[1]] -
						gameState->world->mesh.vertices[itr->vertices[2]]),
				mag(gameState->world->mesh.vertices[itr->vertices[2]] -
						gameState->world->mesh.vertices[itr->vertices[0]])
			};

		float maxLength = 0.0f;
		for(size_t i = 0; i < 3; ++i)
			if(sideLengths[i] > maxLength)
				maxLength = sideLengths[i];

		if(distance(position, triDist) - maxLength > SHIPRADIUS + mag(movement))
			continue;

		Vector3 triangle[] = {
				gameState->world->mesh.vertices[itr->vertices[0]],
				gameState->world->mesh.vertices[itr->vertices[1]],
				gameState->world->mesh.vertices[itr->vertices[2]]
			};
		Vector3 normal = getNormal(triangle[0], triangle[1], triangle[2]);

		// split up the movement into multiple parts if it's greater than the ship radius
		unsigned int splitMovementParts = (int) (mag(movement) / SHIPRADIUS) + 1;
		Vector3 effectiveMovement = movement / (float) splitMovementParts;

		bool collided = false;
		for(unsigned int i = 1; i <= splitMovementParts; ++i) {

			if(
					testTriangleSphereCollision(triangle, position + effectiveMovement * (float) i, SHIPRADIUS)
				) {
++loopCount;
printf("loopCount %i\n", loopCount);

//				setReorientation(reflect(-direction, normal));
				speed /= 2.0f;

				// go back to the beginning and make sure the new direction doesn't collide
				// with any other faces
				// FIXME this is a possible infinite loop
//				itr = gameState->world->mesh.faceGroups.begin()->second.begin();
				itr = collisionFaces.begin();
				collided = true;

				break;
			}
		}

		if(collided)
			continue;
	}

	// do collision detection against the water
	Vector3 movement = direction * speed * dt / 1000.0f;

	if(position.y - mag(movement) <= SHIPRADIUS) {
		float legLength = sqrt(pow(SHIPRADIUS, 2.0f) / 2.0f) * mag(movement);
		Vector3 triangle[] = {
				Vector3(legLength, 0.0f, -legLength),
				Vector3(0.0f, 0.0f, SHIPRADIUS),
				Vector3(-legLength, 0.0f, -legLength)
			};
		Vector3 normal(0.0f, 1.0f, 0.0f);

		// split up the movement into multiple parts if it's greater than the ship radius
		unsigned int splitMovementParts = (int) (mag(movement) / SHIPRADIUS) + 1;
		Vector3 effectiveMovement = movement / (float) splitMovementParts;

		for(unsigned int i = 1; i <= splitMovementParts; ++i) {

			if(
					testTriangleSphereCollision(triangle, /*position +*/ effectiveMovement * (float) i, SHIPRADIUS)
				) {
++loopCount;
printf("loopCount %i\n", loopCount);

//				setReorientation(reflect(-direction, normal));
				speed /= 2.0f;

				break;
			}
		}
	}

//printf("\n");
loopCount = 0;

	// I like to move it, move it
	position += direction * speed * dt / 1000.0f;
}
