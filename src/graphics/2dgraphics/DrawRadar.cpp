// DrawRadar.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawRadar.h"

DrawRadar::DrawRadar() : lastRotation(0), containerDrawer(new DrawContainer()), spotDrawer(new DrawSpot()) {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "colorTexture"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "colorTexture"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["texture"] = glGetUniformLocation(shaderProgram, "texture");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["texCoord"] = glGetAttribLocation(shaderProgram, "texCoord");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertexBufferArray[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STREAM_DRAW);

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);

	// create the progression texture (next power of 2)
	size_t textureDimension = pow(2.0f, (float) ((int) log2(gameSystem->getFloat("radarSize") / 100.0f * (float) gameGraphics->resolutionY - gameSystem->getFloat("hudGaugePadding") * 2.0f) + 1));
	Texture progressionTexture(textureDimension, textureDimension, Texture::FORMAT_RGBA);
	for(size_t i = 0; i < textureDimension; ++i) {
		for(size_t p = 0; p < textureDimension; ++p) {
			float pixelDistance = distance(
					Vector2(textureDimension / 2, textureDimension / 2),
					Vector2((float) i, (float) p)
				);
			float alphaValue = 127.0f;

			if(pixelDistance > textureDimension / 2)
				alphaValue *= 0.0f;

			else if(textureDimension / 2 - pixelDistance <= gameSystem->getFloat("hudContainerSoftEdge"))
				alphaValue *= (textureDimension / 2 - pixelDistance) / gameSystem->getFloat("hudContainerSoftEdge");

			alphaValue *= getAngle(Vector2((float) i, (float) p) - Vector2((float) textureDimension / 2.0f, (float) textureDimension / 2.0f)) / 360.0f;
			progressionTexture.setColorAt(i, p, 0, 0, 0, (uint8_t) alphaValue);
		}
	}

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &progressionTextureID);
	glBindTexture(GL_TEXTURE_2D, progressionTextureID);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(progressionTexture.format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			progressionTexture.width,
			progressionTexture.height,
			0,
			(progressionTexture.format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			progressionTexture.getDataPointer()
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_2D);
}

DrawRadar::~DrawRadar() {
	// delete buffers
	if(glIsBuffer(vertexBuffers["vertices"]))
		glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	if(glIsBuffer(vertexBuffers["elements"]))
		glDeleteBuffers(1, &(vertexBuffers["elements"]));

	// delete shader program
	if(glIsProgram(shaderProgram))
		glDeleteProgram(shaderProgram);

	// delete variables
	delete containerDrawer;

//FIXME destroy textures in GL
}

void DrawRadar::reloadGraphics() {
	// create a texture with the terrain heightmap for radar
	size_t resolution = (size_t) gameSystem->getFloat("islandTerrainDensity");
	Texture* radarTexture = new Texture(
			resolution,
			resolution,
			Texture::FORMAT_RGBA
		);

	for(
			std::vector<Mesh::Face>::iterator itr = gameState->island.faceGroups.begin()->second.begin();
			itr != gameState->island.faceGroups.begin()->second.end();
			++itr
		) {
		uint8_t colorValue, alphaValue;

		if(gameState->island.vertices[itr->vertices[0]].y >= 0.0f) {
			colorValue = (uint8_t) (
					(gameState->island.vertices[itr->vertices[0]].y + gameSystem->getFloat("terrainDepth")) /
					gameSystem->getFloat("islandMaximumHeight") * 255.0f);
			alphaValue = 0xFF;
//		} else if(gameState->island.vertices[itr->vertices[0]].y > -gameSystem->getFloat("terrainDepth")) {
//			colorValue = 0;
//			alphaValue = (gameState->island.vertices[itr->vertices[0]].y + gameSystem->getFloat("terrainDepth")) / gameSystem->getFloat("terrainDepth") * 0.5f * 0xFF;
		} else {
			colorValue = 0;
			alphaValue = 0;
		}

		radarTexture->setColorAt(
				(uint8_t) ((gameState->island.vertices[itr->vertices[0]].x / (gameSystem->getFloat("islandMaximumWidth") / 2.0f) / 2.0f + 0.5f) * (float) resolution),
				(uint8_t) ((gameState->island.vertices[itr->vertices[0]].z / (gameSystem->getFloat("islandMaximumWidth") / 2.0f) / 2.0f + 0.5f) * (float) resolution),
				colorValue,
				colorValue,
				colorValue,
				alphaValue
			);
	}
	radarTexture->setDepth(8);

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &radarTextureID);
	glBindTexture(GL_TEXTURE_2D, radarTextureID);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(radarTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			radarTexture->width,
			radarTexture->height,
			0,
			(radarTexture->format == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			radarTexture->getDataPointer()
	);

	glDisable(GL_TEXTURE_2D);

	delete radarTexture;

	// clear the missile cache
	missileCache.clear();
}

Vector2 DrawRadar::getSize(std::map<std::string, void*> arguments) {
	return Vector2(
			gameSystem->getFloat("radarSize") / 100.0f / gameGraphics->aspectRatio * 2.0f,
			gameSystem->getFloat("radarSize") / 100.0f * 2.0f
		);
}

void DrawRadar::execute(std::map<std::string, void*> arguments) {
	// draw container
	containerDrawer->execute(arguments);

	// collect arguments
	UIMetrics* metrics = ((UIMetrics*) arguments["metrics"]);

	// get the actual size so possibly incorrect metrics don't skew the aspect ratio
	Vector2 actualSize = getSize(arguments);
	Vector2 padding = Vector2(
			*((float*) arguments["padding"]) / (float) gameGraphics->resolutionX * 2.0f,
			*((float*) arguments["padding"]) / (float) gameGraphics->resolutionY * 2.0f
		);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	// compute projection matrix for terrain height map
	Matrix4 heightMapMatrix;
	heightMapMatrix.identity();

	translateMatrix(
			-gameState->fortress.position.x / gameSystem->getFloat("islandMaximumWidth") * 2.0f,
			-gameState->fortress.position.z / gameSystem->getFloat("islandMaximumWidth") * 2.0f,
			0.0f,
			heightMapMatrix
		);
	rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), -radians(90.0f), heightMapMatrix);
	rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), -radians(gameState->fortress.rotation), heightMapMatrix);

	scaleMatrix(
			gameSystem->getFloat("islandMaximumWidth") * 0.5f / gameSystem->getFloat("radarRadius"),
			gameSystem->getFloat("islandMaximumWidth") * 0.5f / gameSystem->getFloat("radarRadius"),
			1.0f,
			heightMapMatrix
		);
	scaleMatrix(actualSize.x / 2.0f - padding.x, actualSize.y / 2.0f - padding.y, 1.0f, heightMapMatrix);

	translateMatrix(metrics->position.x, metrics->position.y, 0.0f, heightMapMatrix);

	float heightMapMatrixArray[] = {
			heightMapMatrix.m11, heightMapMatrix.m12, heightMapMatrix.m13, heightMapMatrix.m14,
			heightMapMatrix.m21, heightMapMatrix.m22, heightMapMatrix.m23, heightMapMatrix.m24,
			heightMapMatrix.m31, heightMapMatrix.m32, heightMapMatrix.m33, heightMapMatrix.m34,
			heightMapMatrix.m41, heightMapMatrix.m42, heightMapMatrix.m43, heightMapMatrix.m44
		};

	// compute projection matrix for progression indicator
	Matrix4 progressionMatrix;
	progressionMatrix.identity();

	rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), -radians(90.0f), progressionMatrix);
	rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), -radians(gameState->fortress.rotation - gameState->lastUpdateGameTime / (gameSystem->getFloat("radarRefreshSpeed") * 1000.0f) * 360.0f), progressionMatrix);

	scaleMatrix(actualSize.x / 2.0f - padding.x, actualSize.y / 2.0f - padding.y, 1.0f, progressionMatrix);
	translateMatrix(metrics->position.x, metrics->position.y, 0.0f, progressionMatrix);

	float progressionMatrixArray[] = {
			progressionMatrix.m11, progressionMatrix.m12, progressionMatrix.m13, progressionMatrix.m14,
			progressionMatrix.m21, progressionMatrix.m22, progressionMatrix.m23, progressionMatrix.m24,
			progressionMatrix.m31, progressionMatrix.m32, progressionMatrix.m33, progressionMatrix.m34,
			progressionMatrix.m41, progressionMatrix.m42, progressionMatrix.m43, progressionMatrix.m44
		};

	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SCISSOR_TEST);
	glScissor(
			(GLint) (((metrics->position.x - actualSize.x / 2.0f + padding.x) + 1.0f) / 2.0f * (float) gameGraphics->resolutionX),
			(GLint) (((metrics->position.y - actualSize.y / 2.0f + padding.y) + 1.0f) / 2.0f * (float) gameGraphics->resolutionY),
			(GLsizei) ((actualSize.x - padding.x * 2.0f) / 2.0f * (float) gameGraphics->resolutionX),
			(GLsizei) ((actualSize.y - padding.y * 2.0f) / 2.0f * (float) gameGraphics->resolutionY)
		);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, heightMapMatrixArray);
	glUniform1i(uniforms["texture"], 0);

	// activate the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, radarTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (5 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["texCoord"]);
	glEnableVertexAttribArray(attributes["color"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	// also draw the progression texture
	glBindTexture(GL_TEXTURE_2D, progressionTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, progressionMatrixArray);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["texCoord"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_SCISSOR_TEST);

	// draw missile locations
	float currentRotation = gameState->lastUpdateGameTime % (unsigned int) (gameSystem->getFloat("radarRefreshSpeed") * 1000.0f) / (gameSystem->getFloat("radarRefreshSpeed") * 1000.0f) * 360.0f;

	size_t i = 0;
	while(i < missileCache.size()) {
		float missileAngle = getAngle(
				Vector2(missileCache[i].position.x, missileCache[i].position.z) -
				Vector2(gameState->fortress.position.x, gameState->fortress.position.z)
			);
		if(missileAngle > lastRotation && missileAngle <= currentRotation) {
			missileCache.erase(missileCache.begin() + i);
		} else {
			++i;
		}
	}

	for(size_t i = 0; i < gameState->missiles.size(); ++i) {
		if(! gameState->missiles[i].alive)
			continue;

		if(distance(
				Vector2(gameState->missiles[i].position.x, gameState->missiles[i].position.z),
				Vector2(gameState->fortress.position.x, gameState->fortress.position.z)
			) >= gameSystem->getFloat("radarRadius"))
				continue;

		float missileAngle = getAngle(
				Vector2(gameState->missiles[i].position.x, gameState->missiles[i].position.z) -
				Vector2(gameState->fortress.position.x, gameState->fortress.position.z)
			);
		if(missileAngle > lastRotation && missileAngle <= currentRotation)
			missileCache.push_back(gameState->missiles[i]);
	}
//missileCache.clear(); for(size_t i = 0; i < gameState->missiles.size(); ++i) if(gameState->missiles[i].alive) missileCache.push_back(gameState->missiles[i]);
	lastRotation = currentRotation;

	Vector2 spotSize(
			gameSystem->getFloat("radarSpotSize") / gameGraphics->resolutionY / gameGraphics->aspectRatio,
			gameSystem->getFloat("radarSpotSize") / gameGraphics->resolutionY
		);
	Vector2 spotPosition(0.0f, 0.0f);
	float spotEdge = gameSystem->getFloat("hudContainerSoftEdge");
	Vector4 insideColor = gameSystem->getColor("radarSpotColor");
	Vector4 outsideColor(
			gameSystem->getColor("radarSpotColor").x,
			gameSystem->getColor("radarSpotColor").y,
			gameSystem->getColor("radarSpotColor").z,
			0.0f
		);

	std::map<std::string,void*> drawerArguments;
	drawerArguments["size"] = (void*) &spotSize;
	drawerArguments["position"] = (void*) &spotPosition;
	drawerArguments["softEdge"] = (void*) &spotEdge;
	drawerArguments["insideColor"] = (void*) &insideColor;
	drawerArguments["outsideColor"] = (void*) &outsideColor;

	for(size_t i = 0; i < missileCache.size(); ++i) {
		Vector4 missilePosition(
				missileCache[i].position.x - gameState->fortress.position.x,
				missileCache[i].position.z - gameState->fortress.position.z,
				0.0f,
				1.0f
			);
		Matrix4 missileMatrix;
		missileMatrix.identity();

		rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), -radians(90.0f), missileMatrix);
		rotateMatrix(Vector3(0.0f, 0.0f, -1.0f), -radians(gameState->fortress.rotation), missileMatrix);

		scaleMatrix(
				(actualSize.x / 2.0f - padding.x) / gameSystem->getFloat("radarRadius"),
				(actualSize.y / 2.0f - padding.y) / gameSystem->getFloat("radarRadius"),
				1.0f,
				missileMatrix
			);

		translateMatrix(metrics->position.x, metrics->position.y, 0.0f, missileMatrix);

		missilePosition = missilePosition * missileMatrix;

		spotPosition = Vector2(missilePosition.x / missilePosition.w, missilePosition.y / missilePosition.w);

		spotDrawer->execute(drawerArguments);
	}

	// draw EMP wave
	if(gameState->fortress.emp > 0.0f && gameState->fortress.emp < 1.0f) {
		insideColor = gameSystem->getColor("radarEMPColor");
		outsideColor = Vector4(
				gameSystem->getColor("radarEMPColor").x,
				gameSystem->getColor("radarEMPColor").y,
				gameSystem->getColor("radarEMPColor").z,
				0.0f
			);
		spotPosition = metrics->position;
		spotSize = Vector2(
					(1.0f - gameState->fortress.emp) * gameSystem->getFloat("stateEMPRange") * (actualSize.x / 2.0f - padding.x) / gameSystem->getFloat("radarRadius"),
					(1.0f - gameState->fortress.emp) * gameSystem->getFloat("stateEMPRange") * (actualSize.y / 2.0f - padding.y) / gameSystem->getFloat("radarRadius")
			);

		spotDrawer->execute(drawerArguments);
	}

	// draw fortress location
	insideColor = gameSystem->getColor("hudGaugeHealthBarColor");
	outsideColor = Vector4(
			gameSystem->getColor("hudGaugeHealthBarColor").x,
			gameSystem->getColor("hudGaugeHealthBarColor").y,
			gameSystem->getColor("hudGaugeHealthBarColor").z,
			0.0f
		);
	spotPosition = metrics->position;
	spotSize = Vector2(
			gameSystem->getFloat("radarCenterSpotSize") / gameGraphics->resolutionY / gameGraphics->aspectRatio,
			gameSystem->getFloat("radarCenterSpotSize") / gameGraphics->resolutionY
		);

	spotDrawer->execute(drawerArguments);
}
