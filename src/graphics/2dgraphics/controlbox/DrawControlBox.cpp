// DrawControlBox.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/2dgraphics/controlbox/DrawControlBox.h"

DrawControlBox::DrawControlBox() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "hudContainer"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "hudContainer"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["insideColor"] = glGetUniformLocation(shaderProgram, "insideColor");
	uniforms["borderColor"] = glGetUniformLocation(shaderProgram, "borderColor");
	uniforms["outsideColor"] = glGetUniformLocation(shaderProgram, "outsideColor");
	uniforms["softEdge"] = glGetUniformLocation(shaderProgram, "softEdge");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["primCoord"] = glGetAttribLocation(shaderProgram, "primCoord");
	attributes["curveOriginCoord"] = glGetAttribLocation(shaderProgram, "curveOriginCoord");
	attributes["border1Dist"] = glGetAttribLocation(shaderProgram, "border1Dist");
	attributes["border2Dist"] = glGetAttribLocation(shaderProgram, "border2Dist");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));
}

void DrawControlBox::execute(std::map<std::string, void*> arguments) {
	// set up geometry
	Vector2 boxSize(
			gameSystem->getFloat("hudControlBoxSize") / 100.0f / gameGraphics->aspectRatio,
			gameSystem->getFloat("hudControlBoxSize") / 100.0f
		);
	float cornerPortion = gameSystem->getFloat("hudControlBoxCornerSize") / 100.0f;
	Vector2 spotSize(
			gameSystem->getFloat("hudControlBoxSpotSize") / 100.0f / gameGraphics->aspectRatio,
			gameSystem->getFloat("hudControlBoxSpotSize") / 100.0f
		);
	Vector4 color = gameSystem->getColor("hudControlBoxColor");

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);
	std::vector<VertexEntry> quadVertices;

	// corner geometry
	drawCurve(
			&quadVertices,
			Vector2(-(boxSize.x - cornerPortion / 2.0f * boxSize.x), -(boxSize.y - cornerPortion / 2.0f * boxSize.y)),
			Vector2(boxSize.x * cornerPortion, boxSize.y * cornerPortion),
			180.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(-(boxSize.x - cornerPortion / 2.0f * boxSize.x), boxSize.y - cornerPortion / 2.0f * boxSize.y),
			Vector2(boxSize.x * cornerPortion, boxSize.y * cornerPortion),
			90.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(boxSize.x - cornerPortion / 2.0f * boxSize.x, boxSize.y - cornerPortion / 2.0f * boxSize.y),
			Vector2(boxSize.x * cornerPortion, boxSize.y * cornerPortion),
			0.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(boxSize.x - cornerPortion / 2.0f * boxSize.x, -(boxSize.y - cornerPortion / 2.0f * boxSize.y)),
			Vector2(boxSize.x * cornerPortion, boxSize.y * cornerPortion),
			270.0f
		);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLuint* cornerElementBufferArray = new GLuint[quadVertices.size()];
	for(size_t i = 0; i < quadVertices.size(); ++i)
		cornerElementBufferArray[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadVertices.size() * sizeof(GLuint), cornerElementBufferArray,
			GL_STREAM_DRAW);

	delete[] cornerElementBufferArray;

	size_t cornerVertexBufferArraySize = quadVertices.size() * 10;
	GLfloat* cornerVertexBufferArray = new GLfloat[cornerVertexBufferArraySize];

	for(size_t i = 0; i < quadVertices.size(); ++i) {
		cornerVertexBufferArray[i * 8 + 0] = quadVertices[i].position.x;
		cornerVertexBufferArray[i * 8 + 1] = quadVertices[i].position.y;
		cornerVertexBufferArray[i * 8 + 2] = quadVertices[i].primCoord.x;
		cornerVertexBufferArray[i * 8 + 3] = quadVertices[i].primCoord.y;
		cornerVertexBufferArray[i * 8 + 4] = quadVertices[i].curveOriginCoord.x;
		cornerVertexBufferArray[i * 8 + 5] = quadVertices[i].curveOriginCoord.y;
//FIXME is this right? does it matter?
		cornerVertexBufferArray[i * 8 + 6] = 2.0f + gameSystem->getFloat("hudContainerSoftEdge") * 2.0f / (cornerPortion / 2.0f * gameGraphics->resolutionY);
		cornerVertexBufferArray[i * 8 + 7] = 2.0f + gameSystem->getFloat("hudContainerSoftEdge") * 2.0f / (cornerPortion / 2.0f * gameGraphics->resolutionY);
	}
	glBufferData(GL_ARRAY_BUFFER, cornerVertexBufferArraySize * sizeof(GLfloat), cornerVertexBufferArray,
			GL_STREAM_DRAW);

	delete[] cornerVertexBufferArray;

	// set uniforms
	glUniform4f(uniforms["insideColor"], color.x, color.y, color.z, 0.0f);
	glUniform4f(uniforms["borderColor"], color.x, color.y, color.z, color.w);
	glUniform4f(uniforms["outsideColor"], color.x, color.y, color.z, color.w);
	glUniform1f(uniforms["softEdge"], gameSystem->getFloat("hudContainerSoftEdge") * 2.0f / (cornerPortion / 2.0f * boxSize.y * gameGraphics->resolutionY));

	// draw the data stored in GPU memory
	glVertexAttribPointer(attributes["position"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) 0);
	glVertexAttribPointer(attributes["primCoord"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) (2 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["curveOriginCoord"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) (4 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["border1Dist"], 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["border2Dist"], 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) (7 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["primCoord"]);
	glEnableVertexAttribArray(attributes["curveOriginCoord"]);
	glEnableVertexAttribArray(attributes["border1Dist"]);
	glEnableVertexAttribArray(attributes["border2Dist"]);

	glDrawElements(GL_QUADS, quadVertices.size(), GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["primiCoord"]);
	glDisableVertexAttribArray(attributes["curveOriginCoord"]);
	glDisableVertexAttribArray(attributes["border1Dist"]);
	glDisableVertexAttribArray(attributes["border2Dist"]);

	// center geometry
	quadVertices.clear();
	drawCurve(
			&quadVertices,
			Vector2(-spotSize.x / 2.0f, -spotSize.y / 2.0f),
			spotSize,
			180.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(-spotSize.x / 2.0f, spotSize.y / 2.0f),
			spotSize,
			90.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(spotSize.x / 2.0f, spotSize.y / 2.0f),
			spotSize,
			0.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(spotSize.x / 2.0f, -spotSize.y / 2.0f),
			spotSize,
			270.0f
		);

	// update vertex buffers
	GLuint* centerElementBufferArray = new GLuint[quadVertices.size()];
	for(size_t i = 0; i < quadVertices.size(); ++i)
		centerElementBufferArray[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadVertices.size() * sizeof(GLuint), centerElementBufferArray,
			GL_STREAM_DRAW);

	delete[] centerElementBufferArray;

	size_t centerVertexBufferArraySize = quadVertices.size() * 10;
	GLfloat* centerVertexBufferArray = new GLfloat[centerVertexBufferArraySize];

	for(size_t i = 0; i < quadVertices.size(); ++i) {
		centerVertexBufferArray[i * 8 + 0] = quadVertices[i].position.x;
		centerVertexBufferArray[i * 8 + 1] = quadVertices[i].position.y;
		centerVertexBufferArray[i * 8 + 2] = quadVertices[i].primCoord.x;
		centerVertexBufferArray[i * 8 + 3] = quadVertices[i].primCoord.y;
		centerVertexBufferArray[i * 8 + 4] = quadVertices[i].curveOriginCoord.x;
		centerVertexBufferArray[i * 8 + 5] = quadVertices[i].curveOriginCoord.y;
		centerVertexBufferArray[i * 8 + 6] = 2.0f;
		centerVertexBufferArray[i * 8 + 7] = 2.0f;
	}

	glBufferData(GL_ARRAY_BUFFER, centerVertexBufferArraySize * sizeof(GLfloat), centerVertexBufferArray,
			GL_STREAM_DRAW);

	delete[] centerVertexBufferArray;

	// set uniforms
	glUniform4f(uniforms["insideColor"], color.x, color.y, color.z, color.w);
	glUniform4f(uniforms["borderColor"], color.x, color.y, color.z, 0.0f);
	glUniform4f(uniforms["outsideColor"], color.x, color.y, color.z, 0.0f);
	glUniform1f(uniforms["softEdge"], gameSystem->getFloat("hudContainerSoftEdge") * 2.0f / (spotSize.y * boxSize.y * gameGraphics->resolutionY));

	// draw the data stored in GPU memory
	glVertexAttribPointer(attributes["position"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) 0);
	glVertexAttribPointer(attributes["primCoord"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) (2 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["curveOriginCoord"], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) (4 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["border1Dist"], 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["border2Dist"], 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*) (7 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["primCoord"]);
	glEnableVertexAttribArray(attributes["curveOriginCoord"]);
	glEnableVertexAttribArray(attributes["border1Dist"]);
	glEnableVertexAttribArray(attributes["border2Dist"]);

	glDrawElements(GL_QUADS, quadVertices.size(), GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["primiCoord"]);
	glDisableVertexAttribArray(attributes["curveOriginCoord"]);
	glDisableVertexAttribArray(attributes["border1Dist"]);
	glDisableVertexAttribArray(attributes["border2Dist"]);

	// undo state
	glDisable(GL_BLEND);
}

void DrawControlBox::drawCurve(
		std::vector<VertexEntry>* quadVertices,
		Vector2 position,
		Vector2 size,
		float rotation,
		bool highlight,
		bool concave
	) {
	VertexEntry vertices[4];

	// set attributes that stay the same
	for(size_t i = 0; i < 4; ++i) {
		vertices[i].curveOriginCoord = Vector2(
				-(sqrt(2.0f) * cos(radians(rotation + 45.0f))),
				-(sqrt(2.0f) * sin(radians(rotation + 45.0f)))
			);
		vertices[i].highlight = highlight;
		vertices[i].concave = concave;
	}

	// set distinct attributes (position, primCoord)
	vertices[0].position = Vector2(
			position.x - size.x / 2.0f,
			position.y - size.y / 2.0f
		);
	vertices[0].primCoord = Vector2(-1.0f, -1.0f);

	vertices[1].position = Vector2(
			position.x - size.x / 2.0f,
			position.y + size.y / 2.0f
		);
	vertices[1].primCoord = Vector2(-1.0f, 1.0f);

	vertices[2].position = Vector2(
			position.x + size.x / 2.0f,
			position.y + size.y / 2.0f
		);
	vertices[2].primCoord = Vector2(1.0f, 1.0f);

	vertices[3].position = Vector2(
			position.x + size.x / 2.0f,
			position.y - size.y / 2.0f
		);
	vertices[3].primCoord = Vector2(1.0f, -1.0f);

	// push back the quad vertices
	for(size_t i = 0; i < 4; ++i)
		quadVertices->push_back(vertices[i]);
}

void DrawControlBox::drawBorder(
		std::vector<VertexEntry>* quadVertices,
		Vector2 position,
		Vector2 size,
		float rotation,
		bool highlight
	) {
	VertexEntry vertices[4];

	// set attributes that stay the same
	for(size_t i = 0; i < 4; ++i) {
		vertices[i].curveOriginCoord = Vector2(
				-cos(radians(rotation)),
				-sin(radians(rotation))
			);
		vertices[i].highlight = highlight;
		vertices[i].concave = false;
	}

	// set distinct attributes (position, primCoord)
	vertices[0].position = Vector2(
			position.x - size.x / 2.0f,
			position.y - size.y / 2.0f
		);
	vertices[0].primCoord = Vector2(
			-absolute(cos(radians(rotation))),
			-absolute(sin(radians(rotation)))
		);

	vertices[1].position = Vector2(
			position.x - size.x / 2.0f,
			position.y + size.y / 2.0f
		);
	vertices[1].primCoord = Vector2(
			-absolute(cos(radians(rotation))),
			absolute(sin(radians(rotation)))
		);

	vertices[2].position = Vector2(
			position.x + size.x / 2.0f,
			position.y + size.y / 2.0f
		);
	vertices[2].primCoord = Vector2(
			absolute(cos(radians(rotation))),
			absolute(sin(radians(rotation)))
		);

	vertices[3].position = Vector2(
			position.x + size.x / 2.0f,
			position.y - size.y / 2.0f
		);
	vertices[3].primCoord = Vector2(
			absolute(cos(radians(rotation))),
			-absolute(sin(radians(rotation)))
		);

	// push back the quad vertices
	for(size_t i = 0; i < 4; ++i)
		quadVertices->push_back(vertices[i]);
}

void DrawControlBox::drawFiller(
		std::vector<VertexEntry>* quadVertices,
		Vector2 position,
		Vector2 size,
		bool highlight
	) {
	VertexEntry vertices[4];

	// set attributes that stay the same
	for(size_t i = 0; i < 4; ++i) {
		vertices[i].primCoord = Vector2(0.0f, 0.0f);
		vertices[i].curveOriginCoord = Vector2(0.0f, 0.0f);
		vertices[i].highlight = highlight;
		vertices[i].concave = false;
	}

	// set distinct attribute (position)
	vertices[0].position = Vector2(
			position.x - size.x / 2.0f,
			position.y - size.y / 2.0f
		);

	vertices[1].position = Vector2(
			position.x - size.x / 2.0f,
			position.y + size.y / 2.0f
		);

	vertices[2].position = Vector2(
			position.x + size.x / 2.0f,
			position.y + size.y / 2.0f
		);

	vertices[3].position = Vector2(
			position.x + size.x / 2.0f,
			position.y - size.y / 2.0f
		);

	// push back the quad vertices
	for(size_t i = 0; i < 4; ++i)
		quadVertices->push_back(vertices[i]);
}
