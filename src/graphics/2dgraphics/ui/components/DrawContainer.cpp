/*
 * DrawContainer.cpp
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

#include "graphics/2dgraphics/ui/components/DrawContainer.h"

DrawContainer::DrawContainer() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "hudContainer"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "hudContainer"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["insideColor"] = glGetUniformLocation(shaderProgram, "insideColor");
	uniforms["highlightColor"] = glGetUniformLocation(shaderProgram, "highlightColor");
	uniforms["borderColor"] = glGetUniformLocation(shaderProgram, "borderColor");
	uniforms["outsideColor"] = glGetUniformLocation(shaderProgram, "outsideColor");
	uniforms["pixFrac"] = glGetUniformLocation(shaderProgram, "pixFrac");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["primCoord"] = glGetAttribLocation(shaderProgram, "primCoord");
	attributes["curveOriginCoord"] = glGetAttribLocation(shaderProgram, "curveOriginCoord");
	attributes["border1Dist"] = glGetAttribLocation(shaderProgram, "border1Dist");
	attributes["border2Dist"] = glGetAttribLocation(shaderProgram, "border2Dist");
	attributes["highlight"] = glGetAttribLocation(shaderProgram, "highlight");
	attributes["concave"] = glGetAttribLocation(shaderProgram, "concave");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));
}

void DrawContainer::execute(std::map<std::string, void*> arguments) {
	// set up geometry
	Vector2 position = ((UIMetrics*) arguments["metrics"])->position;
	Vector2 size = ((UIMetrics*) arguments["metrics"])->size;

	Vector2 padding = Vector2(
			*((float*) arguments["padding"]) / (float) gameGraphics->resolutionX * 2.0f,
			*((float*) arguments["padding"]) / (float) gameGraphics->resolutionY * 2.0f
		);
	float border = *((float*) arguments["border"]) / *((float*) arguments["padding"]) * 2.0f;

	std::vector<DrawContainer::VertexEntry> quadVertices;

	drawCurve(
			&quadVertices,
			Vector2(-size.x / 2.0f + padding.x / 2.0f, -size.y / 2.0f + padding.y / 2.0f),
			padding,
			180.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(-size.x / 2.0f + padding.x / 2.0f, size.y / 2.0f - padding.y / 2.0f),
			padding,
			90.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(size.x / 2.0f - padding.x / 2.0f, size.y / 2.0f - padding.y / 2.0f),
			padding,
			0.0f
		);
	drawCurve(
			&quadVertices,
			Vector2(size.x / 2.0f - padding.x / 2.0f, -size.y / 2.0f + padding.y / 2.0f),
			padding,
			270.0f
		);

	drawBorder(
			&quadVertices,
			Vector2(-size.x / 2.0f + padding.x / 2.0f, 0.0f),
			Vector2(padding.x, size.y - padding.y * 2.0f),
			180.0f
		);
	drawBorder(
			&quadVertices,
			Vector2(0.0f, size.y / 2.0f - padding.y / 2.0f),
			Vector2(size.x - padding.x * 2.0f, padding.y),
			90.0f
		);

	drawBorder(
			&quadVertices,
			Vector2(size.x / 2.0f - padding.x / 2.0f, 0.0f),
			Vector2(padding.x, size.y - padding.y * 2.0f),
			0.0f
		);

	drawBorder(
			&quadVertices,
			Vector2(0.0f, -size.y / 2.0f + padding.y / 2.0f),
			Vector2(size.x - padding.x * 2.0f, padding.y),
			270.0f
		);

	drawFiller(
			&quadVertices,
			Vector2(0.0f, 0.0f),
			size - padding * 2.0f,
			false
		);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLuint* elementBufferArray = new GLuint[quadVertices.size()];
	for(size_t i = 0; i < quadVertices.size(); ++i)
		elementBufferArray[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadVertices.size() * sizeof(GLuint), elementBufferArray,
			GL_STREAM_DRAW);

	delete[] elementBufferArray;

	size_t vertexBufferArraySize = quadVertices.size() * 10;
	GLfloat* vertexBufferArray = new GLfloat[vertexBufferArraySize];

	for(size_t i = 0; i < quadVertices.size(); ++i) {
		vertexBufferArray[i * 10 + 0] = quadVertices[i].position.x + position.x;
		vertexBufferArray[i * 10 + 1] = quadVertices[i].position.y + position.y;
		vertexBufferArray[i * 10 + 2] = quadVertices[i].primCoord.x;
		vertexBufferArray[i * 10 + 3] = quadVertices[i].primCoord.y;
		vertexBufferArray[i * 10 + 4] = quadVertices[i].curveOriginCoord.x;
		vertexBufferArray[i * 10 + 5] = quadVertices[i].curveOriginCoord.y;
		vertexBufferArray[i * 10 + 6] = 2.0f - border;
		vertexBufferArray[i * 10 + 7] = 2.0f;
		vertexBufferArray[i * 10 + 8] = quadVertices[i].highlight;
		vertexBufferArray[i * 10 + 9] = (quadVertices[i].concave ? 1.0f : 0.0f);
/*
printf("vertex posX %.4f\tposY %.4f\tprimX %.4f\tprimY %.4f\tcOrigX %.4f\tcOrigY %.4f\toutDist %.4f\tinDist %.4f\thi %.4f\tconc %.4f\n",
		vertexBufferArray[i * 10 + 0],
		vertexBufferArray[i * 10 + 1],
		vertexBufferArray[i * 10 + 2],
		vertexBufferArray[i * 10 + 3],
		vertexBufferArray[i * 10 + 4],
		vertexBufferArray[i * 10 + 5],
		vertexBufferArray[i * 10 + 6],
		vertexBufferArray[i * 10 + 7],
		vertexBufferArray[i * 10 + 8],
		vertexBufferArray[i * 10 + 9]
);
*/
	}
//printf("\n");
	glBufferData(GL_ARRAY_BUFFER, vertexBufferArraySize * sizeof(GLfloat), vertexBufferArray,
			GL_STREAM_DRAW);

	delete[] vertexBufferArray;

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	Vector4 insideColor = *((Vector4*) arguments["insideColor"]);
	if((float*) arguments["containerTimer"] != NULL)
		insideColor.w *= *((float*) arguments["containerTimer"]);
	Vector4 highlightColor = *((Vector4*) arguments["highlightColor"]);
	Vector4 borderColor = *((Vector4*) arguments["borderColor"]);
	Vector4 outsideColor = *((Vector4*) arguments["outsideColor"]);

	glUniform4f(uniforms["insideColor"], insideColor.x, insideColor.y, insideColor.z, insideColor.w);
	glUniform4f(uniforms["highlightColor"], highlightColor.x, highlightColor.y, highlightColor.z,
			highlightColor.w);
	glUniform4f(uniforms["borderColor"], borderColor.x, borderColor.y, borderColor.z, borderColor.w);
	glUniform4f(uniforms["outsideColor"], outsideColor.x, outsideColor.y, outsideColor.z, outsideColor.w);
	glUniform1f(uniforms["pixFrac"], 1.0f / *((float*) arguments["padding"]));

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT),
			(GLvoid*) 0);
	glVertexAttribPointer(attributes["primCoord"], 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT),
			(GLvoid*) (2 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["curveOriginCoord"], 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT),
			(GLvoid*) (4 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["border1Dist"], 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT),
			(GLvoid*) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["border2Dist"], 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT),
			(GLvoid*) (7 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["highlight"], 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT),
			(GLvoid*) (8 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["concave"], 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GL_FLOAT),
			(GLvoid*) (9 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["primCoord"]);
	glEnableVertexAttribArray(attributes["curveOriginCoord"]);
	glEnableVertexAttribArray(attributes["border1Dist"]);
	glEnableVertexAttribArray(attributes["border2Dist"]);
	glEnableVertexAttribArray(attributes["highlight"]);
	glEnableVertexAttribArray(attributes["concave"]);

	glDrawElements(GL_QUADS, quadVertices.size(), GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["primiCoord"]);
	glDisableVertexAttribArray(attributes["curveOriginCoord"]);
	glDisableVertexAttribArray(attributes["border1Dist"]);
	glDisableVertexAttribArray(attributes["border2Dist"]);
	glDisableVertexAttribArray(attributes["highlight"]);
	glDisableVertexAttribArray(attributes["concave"]);

	// undo state
	glDisable(GL_BLEND);
}

void DrawContainer::drawCurve(
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

void DrawContainer::drawBorder(
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

void DrawContainer::drawFiller(
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

/*

// console container constants
#define INSIDE_COLOR 0.047058823529412f, 0.043137254901961f, 0.137254901960784f, 0.6f
#define HIGHLIGHT_COLOR 0.274509803921569f, 0.298039215686275f, 0.403921568627451f, 1.0f
#define BORDER_COLOR 0.52156862745098f, 0.568627450980392f, 0.537254901960784f, 1.0f
#define OUTSIDE_COLOR 0.0f, 0.0f, 0.0f, 0.0f
#define CONTAINER_BORDER_WIDTH 8.0f / screenWidth * 2.0f

void DrawHUDContainerUtility::render() {
	glUseProgram(containerProgram);

	drawCurve(Vector2(0.75f, 0.75f), Vector2(0.5f, 0.5f), 0.0f);
	drawCurve(Vector2(-0.75f, 0.75f), Vector2(0.5f, 0.5f), 90.0f);
	drawCurve(Vector2(-0.75f, -0.75f), Vector2(0.5f, 0.5f), 180.0f);
	drawCurve(Vector2(0.75f, -0.75f), Vector2(0.5f, 0.5f), 270.0f);

	drawBorder(Vector2(0.75f, 0.0f), Vector2(0.5f, 1.0f), 0.0f);
	drawBorder(Vector2(0.0f, 0.75f), Vector2(1.0f, 0.5f), 90.0f);
	drawBorder(Vector2(-0.75f, 0.0f), Vector2(0.5f, 1.0f), 180.0f);
	drawBorder(Vector2(0.0f, -0.75f), Vector2(1.0f, 0.5f), 270.0f);

	drawFiller(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));


	drawCurve(Vector2(-0.5f, -0.5f), Vector2(1.0f, 1.0f), 180, true);
	drawCurve(Vector2(-0.5f, 0.5f), Vector2(1.0f, 1.0f), 90);
	drawCurve(Vector2(0.5f, 0.5f), Vector2(1.0f, 1.0f), 0, true);
	drawCurve(Vector2(0.5f, -0.5f), Vector2(1.0f, 1.0f), 270);

	// left border
	drawCurve(Vector2(-0.8f, -0.3f), Vector2(0.1f, 0.1f), 180);
	drawBorder(Vector2(-0.8f, 0.0f), Vector2(0.1f, 0.5f), 180);
	drawCurve(Vector2(-0.80f, 0.3f), Vector2(0.1f, 0.1f), 90);

	// middle
	drawBorder(Vector2(0.0f, -0.3f), Vector2(1.5f, 0.1f), 270);
	drawFiller(Vector2(0.0f, 0.0f), Vector2(1.5f, 0.5f));
	drawBorder(Vector2(-0.5f, 0.3f), Vector2(0.5f, 0.1f), 90);	// ?

	// right twist-up
	drawCurve(Vector2(0.7f, 0.4f - CONTAINER_BORDER_WIDTH), Vector2(0.1f, 0.1f), 180, false, 0.0f, true);
	drawCurve(Vector2(0.6f + CONTAINER_BORDER_WIDTH, 0.5f - CONTAINER_BORDER_WIDTH), Vector2(0.1f, 0.1f), 0);

	// right tab
	drawBorder(Vector2(0.5f, 0.5f - CONTAINER_BORDER_WIDTH), Vector2(0.1f + 2.0f * CONTAINER_BORDER_WIDTH, 0.1f), 90);
	drawFiller(Vector2(0.5f, 0.4f - CONTAINER_BORDER_WIDTH), Vector2(0.3f, 0.1f));

	// right twist-down
	drawCurve(Vector2(0.3f, 0.4f - CONTAINER_BORDER_WIDTH), Vector2(0.1f, 0.1f), 270, false, 0.0f, true);
	drawCurve(Vector2(0.4f - CONTAINER_BORDER_WIDTH, 0.5f - CONTAINER_BORDER_WIDTH), Vector2(0.1f, 0.1f), 90);

	// left twist-up
	drawCurve(Vector2(0.2f, 0.4f - CONTAINER_BORDER_WIDTH), Vector2(0.1f, 0.1f), 180, true, 0.0f, true);
	drawCurve(Vector2(0.1f + CONTAINER_BORDER_WIDTH, 0.5f - CONTAINER_BORDER_WIDTH), Vector2(0.1f, 0.1f), 0, true);

	// left tab
	drawBorder(Vector2(0.0f, 0.5f - CONTAINER_BORDER_WIDTH), Vector2(0.1f + 2.0f * CONTAINER_BORDER_WIDTH, 0.1f), 90, true);
	drawFiller(Vector2(0.0f, 0.4f - CONTAINER_BORDER_WIDTH), Vector2(0.3f, 0.1f), true);

	// left twist-down
	drawCurve(Vector2(-0.2f, 0.4f - CONTAINER_BORDER_WIDTH), Vector2(0.1f, 0.1f), 270, true, 0.0f, true);
	drawCurve(Vector2(-0.1f - CONTAINER_BORDER_WIDTH, 0.5f - CONTAINER_BORDER_WIDTH), Vector2(0.1f, 0.1f), 90, true);

	// bar under tabs
//	drawFiller(Vector2(0.25f, 0.3f), Vector2(1.0f, 0.1f));
	drawBorder(Vector2(0.0f, 0.3f), Vector2(0.5f, 0.1f), 90, false, -0.01f);
	drawFiller(Vector2(0.5f, 0.3f), Vector2(0.5f, 0.1f));

	// right border
	drawCurve(Vector2(0.8f, -0.3f), Vector2(0.1f, 0.1f), 270);
	drawBorder(Vector2(0.8f, 0.0f), Vector2(0.1f, 0.5f), 0);
	drawCurve(Vector2(0.80f, 0.3f), Vector2(0.1f, 0.1f), 0);
}
*/
