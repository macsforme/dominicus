// ExplosionRenderer.cpp
// Crucible Island

#include "graphics/3dgraphics/ExplosionRenderer.h"

#include "core/GameSystem.h"
#include "geometry/Sphere.h"
#include "graphics/GameGraphics.h"
#include "math/MatrixMath.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

ExplosionRenderer::ExplosionRenderer() : sphere(makeSphere((size_t) gameSystem->getFloat("explosionSphereDensity"))) {
	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	GLfloat* vertDataBufferArray = new GLfloat[sphere.faceGroups[""].size() * 18];	// 3 vertices + 3 normals + 3 faces

	// insert the vertex attribute data
	for(size_t i = 0; i < sphere.faceGroups[""].size(); ++i) {
		for(size_t p = 0; p < 3; ++p) {
			vertDataBufferArray[i * 18 + p * 6 + 0] = sphere.vertices[sphere.faceGroups[""][i].vertices[p]].x;
			vertDataBufferArray[i * 18 + p * 6 + 1] = sphere.vertices[sphere.faceGroups[""][i].vertices[p]].y;
			vertDataBufferArray[i * 18 + p * 6 + 2] = sphere.vertices[sphere.faceGroups[""][i].vertices[p]].z;

			vertDataBufferArray[i * 18 + p * 6 + 3] = sphere.vertices[sphere.faceGroups[""][i].normals[p]].x;
			vertDataBufferArray[i * 18 + p * 6 + 4] = sphere.vertices[sphere.faceGroups[""][i].normals[p]].y;
			vertDataBufferArray[i * 18 + p * 6 + 5] = sphere.vertices[sphere.faceGroups[""][i].normals[p]].z;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, sphere.faceGroups[""].size() * 18 * sizeof(GLfloat), vertDataBufferArray, GL_STATIC_DRAW);

	delete[] vertDataBufferArray;

	glGenBuffers(1, &(vertexBuffers["elements"]));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLuint* vertElementBufferArray = new GLuint[sphere.faceGroups[""].size() * 3];

	for(size_t i = 0; i < sphere.faceGroups[""].size() * 3; ++i) {
		vertElementBufferArray[i] = i;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.faceGroups[""].size() * 3 * sizeof(GLuint), vertElementBufferArray, GL_STATIC_DRAW);

	delete[] vertElementBufferArray;
}

ExplosionRenderer::~ExplosionRenderer() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

void ExplosionRenderer::execute(DrawStackArgList arguments) {
	// update missile caches
	size_t i = 0;
	while(i < explodingMissiles.size()) {
		bool isActive = false;

		for(size_t p = 0; p < 73; ++p) {
			if(explodingMissiles[i].explosions[p].beginTime + explodingMissiles[i].explosions[p].duration >= gameState->lastUpdateGameTime) {
				isActive = true;

				break;
			}
		}

		if(! isActive)
			explodingMissiles.erase(explodingMissiles.begin() + i);
		else
			++i;
	}

	i = 0;
	while(i < gameState->missiles.size()) {
		if(gameState->missiles[i].alive) {
			bool isInCache = false;

			for(size_t p = 0; p < activeMissiles.size(); ++p) {
				if(activeMissiles[p] == i) {
					isInCache = true;

					break;
				}
			}

			if(! isInCache)
				activeMissiles.push_back(i);
		}

		++i;
	}

	for(size_t i = 0; i < activeMissiles.size(); ++i) {
		if(! gameState->missiles[activeMissiles[i]].alive) {
			ExplodingMissile explodingMissile;

			Explosion explosion;
			explosion.beginTime = gameState->lastUpdateGameTime;
			explosion.duration = gameSystem->getFloat("explosionDuration") * 0.5f * 1000.0f;
			explosion.radius = gameSystem->getFloat("explosionRadius");
			explosion.position = gameState->missiles[activeMissiles[i]].position;
			explosion.movement = Vector3(0.0f, 0.0f, 0.0f);

			explodingMissile.explosions[0] = explosion;

			float baseBeginTime = gameSystem->getFloat("explosionDuration") * 0.25f * 1000.0f;

			for(size_t p = 1; p < 9; ++p) {
				explosion.beginTime = explodingMissile.explosions[0].beginTime;
				explosion.beginTime += baseBeginTime * 0.5f + (float) rand() / (float) RAND_MAX * baseBeginTime;

				// don't draw every sphere every time
				if(rand() % 3 < 2)
					explosion.duration = gameSystem->getFloat("explosionDuration") * 0.5f * 1000.0f;
				else
					explosion.duration = 0;

				explosion.radius = gameSystem->getFloat("explosionRadius") * 0.75f;

				explosion.position = explodingMissile.explosions[0].position;
				explosion.position.x += gameSystem->getFloat("explosionRadius") * 0.25f * ((p - 1) % 4 < 2 ? -1.0f : 1.0f);
				explosion.position.y += gameSystem->getFloat("explosionRadius") * 0.25f * ((p - 1) % 4 == 0 || (p - 1) % 4 == 3 ? -1.0f : 1.0f);
				explosion.position.z += gameSystem->getFloat("explosionRadius") * 0.25f * (p < 5 ? -1.0f : 1.0f);

				explosion.movement = Vector3(
						(float) rand() / (float) RAND_MAX * (rand() % 2 == 0 ? 1.0f : -1.0f),
						(float) rand() / (float) RAND_MAX * (rand() % 2 == 0 ? 1.0f : -1.0f),
						(float) rand() / (float) RAND_MAX * (rand() % 2 == 0 ? 1.0f : -1.0f)
					);

				explosion.movement.norm();
				explosion.movement *= explosion.radius;

				explodingMissile.explosions[p] = explosion;
			}

			for(size_t p = 9; p < 73; ++p) {
				explosion.beginTime = explodingMissile.explosions[(p - 9) / 8 + 1].beginTime;
				explosion.beginTime += baseBeginTime * 0.5f + (float) rand() / (float) RAND_MAX * baseBeginTime;

				// don't draw every sphere every time
				if(rand() % 10 < 3)
					explosion.duration = gameSystem->getFloat("explosionDuration") * 0.5f * 1000.0f;
				else
					explosion.duration = 0;

				explosion.radius = gameSystem->getFloat("explosionRadius") * 0.5f;

				explosion.position = explodingMissile.explosions[(p - 9) / 8 + 1].position;
				explosion.position.x += gameSystem->getFloat("explosionRadius") * 0.1f * ((p - 9) % 4 < 2 ? -1.0f : 1.0f);
				explosion.position.y += gameSystem->getFloat("explosionRadius") * 0.1f * ((p - 9) % 4 == 0 || (p - 1) % 4 == 3 ? -1.0f : 1.0f);
				explosion.position.z += gameSystem->getFloat("explosionRadius") * 0.1f * ((p - 9) % 8 < 4 ? -1.0f : 1.0f);

				explosion.movement = Vector3(
						(float) rand() / (float) RAND_MAX * (rand() % 2 == 0 ? 1.0f : -1.0f),
						(float) rand() / (float) RAND_MAX * (rand() % 2 == 0 ? 1.0f : -1.0f),
						(float) rand() / (float) RAND_MAX * (rand() % 2 == 0 ? 1.0f : -1.0f)
					);

				explosion.movement.norm();
				explosion.movement *= explosion.radius;

				explodingMissile.explosions[p] = explosion;
			}

			explodingMissiles.push_back(explodingMissile);

			activeMissiles.erase(activeMissiles.begin() + i);
			i = 0;
		}
	}

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	if(gameGraphics->supportsMultisampling) glEnable(GL_MULTISAMPLE);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_TEXTURE_2D);

	// enable shader
	glUseProgram(gameGraphics->getProgramID("explosion"));

	// set uniforms
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("explosion"), "pMatrix"), 1, GL_FALSE, (gameState->binoculars ? gameGraphics->ppBinoMatrixArray : gameGraphics->ppMatrixArray));

	// set the overall drawing state
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("explosion"), "position"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("explosion"), "normal"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("explosion"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("explosion"), "normal"));

	// draw the geometry
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	for(size_t i = 0; i < explodingMissiles.size(); ++i) {
		for(size_t p = 0; p < 73; ++p) {
			if(
					explodingMissiles[i].explosions[p].beginTime > gameState->lastUpdateGameTime ||
					explodingMissiles[i].explosions[p].beginTime + explodingMissiles[i].explosions[p].duration <=
						gameState->lastUpdateGameTime
				)
				continue;

			float progression =
					(float) (gameState->lastUpdateGameTime - explodingMissiles[i].explosions[p].beginTime) /
					(float) explodingMissiles[i].explosions[p].duration;
			glUniform1f(glGetUniformLocation(gameGraphics->getProgramID("explosion"), "progression"), progression);

			Vector4 missilePosition(
					explodingMissiles[i].explosions[p].position.x + explodingMissiles[i].explosions[p].movement.x * progression,
					explodingMissiles[i].explosions[p].position.y + explodingMissiles[i].explosions[p].movement.y * progression,
					explodingMissiles[i].explosions[p].position.z + explodingMissiles[i].explosions[p].movement.z * progression,
					0.0f
				);
			missilePosition = missilePosition * gameGraphics->currentCamera->mvMatrix;
			Vector4 fortressPosition(
					gameState->fortress.position.x,
					gameState->fortress.position.y,
					gameState->fortress.position.z,
					0.0f
				);
			fortressPosition = fortressPosition * gameGraphics->currentCamera->mvMatrix;
			Vector4 fortressVector = fortressPosition - missilePosition;
			glUniform3f(glGetUniformLocation(gameGraphics->getProgramID("explosion"), "fortressVector"), fortressVector.x, fortressVector.y, fortressVector.z);

			Matrix4 mvMatrix; mvMatrix.identity();

			float scaleFactor;
			if(progression <= 0.5f)
				scaleFactor = progression * 2.0f * 0.6f;
			else if(progression <= 0.75f)
				scaleFactor = (progression - 0.5f) * 4.0f * 0.3f + 0.6f;
			else
				scaleFactor = (progression - 0.75f) * 4.0f * 0.1f + 0.9f;

			scaleFactor *= explodingMissiles[i].explosions[p].radius;

			scaleMatrix(scaleFactor, scaleFactor, scaleFactor, mvMatrix);

			translateMatrix(
					explodingMissiles[i].explosions[p].position.x + explodingMissiles[i].explosions[p].movement.x * progression,
					explodingMissiles[i].explosions[p].position.y + explodingMissiles[i].explosions[p].movement.y * progression,
					explodingMissiles[i].explosions[p].position.z + explodingMissiles[i].explosions[p].movement.z * progression,
					mvMatrix
				);

			mvMatrix = mvMatrix * gameGraphics->currentCamera->mvMatrix;

			float mvMatrixArray[] = {
					mvMatrix.m11, mvMatrix.m12, mvMatrix.m13, mvMatrix.m14,
					mvMatrix.m21, mvMatrix.m22, mvMatrix.m23, mvMatrix.m24,
					mvMatrix.m31, mvMatrix.m32, mvMatrix.m33, mvMatrix.m34,
					mvMatrix.m41, mvMatrix.m42, mvMatrix.m43, mvMatrix.m44
				};
			glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("explosion"), "mvMatrix"), 1, GL_FALSE, mvMatrixArray);

			glDrawElements(GL_TRIANGLES, sphere.faceGroups[""].size() * 3, GL_UNSIGNED_INT, NULL);
		}
	}

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("explosion"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("explosion"), "normal"));
}

void ExplosionRenderer::reloadState() {
	// clear missile caches
	activeMissiles.clear();
	explodingMissiles.clear();
}
