/*
 *  DrawConsole.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/5/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "drawing/HUD/console/DrawConsole.h"

DrawConsole::DrawConsole(HUDArrangement* hudArrangement, FontManager* fontManager) :
		fontManager(fontManager) {
	// set up the HUD laying arrangement info
	myElement = new HUDElement(Vector2(1.5f / gameWindow->aspectRatio, 0.333f));

	HUDArrangement::HUDElementEntry myEntry;
	myEntry.element = myElement;
	myEntry.bearing1 = HUDArrangement::BEARING_BOTTOM;
	myEntry.bearing2 = HUDArrangement::BEARING_LEFT;

	hudArrangement->elementList.push_back(myEntry);
	hudArrangement->rearrange();

	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/default.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/default.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	program = ShaderTools::makeProgram(shaders);

	// set attribute locations
	positionAttrib = 0;
	texCoordAttrib = 1;
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "texCoord");

	ShaderTools::linkProgram(program);

	// get uniform locations
	mvpMatrixUniform = glGetUniformLocation(program, "mvpMatrix");
	textureUniform = glGetUniformLocation(program, "texture");

	float insideColor[4] = { 0.0f, 0.01f, 0.13f, 0.8f };
	float highlightColor[4] = { 0.29f, 0.31f, 0.42f, 1.0f };
	float borderColor[4] = { 0.52f, 0.57f, 0.54f, 1.0f };
	float outsideColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// initialize container drawing utility
	containerUtility = new DrawHUDContainerUtility(
			-0.96f,
			0.5f,
			insideColor,
			highlightColor,
			borderColor,
			outsideColor
		);

	// initialize other variables
	textBlock = NULL;
}

void DrawConsole::draw() {
	// account for our container padding
	const Vector2 containerPadding(
			0.02f / gameWindow->aspectRatio,
			0.02f
		);
	const Vector2 contentSize = myElement->size - containerPadding * 2.0f;

	// draw the container

	// background
	containerUtility->drawFiller(myElement->position, contentSize, false, 0.0f);

	// sides
	containerUtility->drawBorder(
			Vector2(myElement->position.x - contentSize.x / 2.0f -
					containerPadding.x / 2.0f, myElement->position.y),
			Vector2(containerPadding.x, contentSize.y),
			180.0f,
			false,
			0.0f);
	containerUtility->drawBorder(
			Vector2(myElement->position.x + contentSize.x / 2.0f +
				containerPadding.x / 2.0f, myElement->position.y),
			Vector2(containerPadding.x, contentSize.y),
			0.0f,
			false,
			0.0f);

	containerUtility->drawBorder(
			Vector2(myElement->position.x, myElement->position.y -
					contentSize.y / 2.0f - containerPadding.y / 2.0f),
			Vector2(contentSize.x, containerPadding.y),
			270.0f,
			false,
			0.005f);
	containerUtility->drawBorder(
			Vector2(myElement->position.x, myElement->position.y +
					contentSize.y / 2.0f + containerPadding.y / 2.0f),
			Vector2(contentSize.x, containerPadding.y),
			90.0f,
			false,
			0.005f);

	// corners
	containerUtility->drawCurve(
			myElement->position + contentSize / 2.0f + containerPadding / 2.0f,
			containerPadding,
			0.0f,
			false,
			0.0f,
			false
		);
	containerUtility->drawCurve(
			Vector2(
					myElement->position.x - contentSize.x / 2.0f - containerPadding.x / 2.0f,
					myElement->position.y + contentSize.y / 2.0f + containerPadding.y / 2.0f
				),
			containerPadding,
			90.0f,
			false,
			0.0f,
			false
		);
	containerUtility->drawCurve(
			myElement->position - contentSize / 2.0f - containerPadding / 2.0f,
			containerPadding,
			180.0f,
			false,
			0.0f,
			false
		);
	containerUtility->drawCurve(
			Vector2(
					myElement->position.x + contentSize.x / 2.0f + containerPadding.x / 2.0f,
					myElement->position.y - contentSize.y / 2.0f - containerPadding.y / 2.0f
				),
			containerPadding,
			270.0f,
			false,
			0.0f,
			false
		);

	// update the text block if we need to
	extern std::vector<std::string> logStrings;
	static unsigned int numLogLines = 0;

	if(textBlock == NULL || numLogLines != logStrings.size()) {
		// create a string of combined log lines
		std::string logString = "";

		for(
				std::vector<std::string>::iterator itr = logStrings.begin();
				itr != logStrings.end();
				++itr
			)
			logString += *itr + "\n";

		// no trailing newline
		logString = logString.substr(0, logString.length() - 1);

		if(textBlock != NULL)
			delete(textBlock);

		textBlock = new TextBlock(logString.c_str(),
				(unsigned int)((myElement->size.x  - containerPadding.x * 2.0f) / 2.0f * gameWindow->width),
				(unsigned int)((myElement->size.y  - containerPadding.y * 2.0f) / 2.0f * gameWindow->height),
				fontManager,
				gamePrefs.getInt("fontStandardSize")
		);
		numLogLines = logStrings.size();
	}

	// draw the text
	unsigned int fontSize = gamePrefs.getInt("fontStandardSize");

	glUseProgram(program);

	float mvpMatrixArray[] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrixArray);

	GLuint textureID = fontManager->textureIDs[fontSize];

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(textureUniform, 0);

	// align the text to the bottom-left of the content box
	Vector2 drawSize(contentSize);
	drawSize.x *= (float) textBlock->width / (contentSize.x / 2.0f * (float) gameWindow->width);
	drawSize.y *= (float) textBlock->height / (contentSize.y / 2.0f * (float) gameWindow->height);

	Vector2 drawCenter(myElement->position);
	drawCenter.x -= contentSize.x / 2.0f;
	drawCenter.x += drawSize.x / 2.0f;
	drawCenter.y -= contentSize.y / 2.0f;
	drawCenter.y += drawSize.y / 2.0f;

	for(
			std::vector<TextBlock::CharEntry>::iterator itr = textBlock->entries.begin();
			itr != textBlock->entries.end();
			++itr
		) {
		glBegin(GL_QUADS);

		glVertexAttrib2f(
				texCoordAttrib,
				fontManager->fontData[itr->character][fontSize].sX,
				fontManager->fontData[itr->character][fontSize].sY
			);
		glVertexAttrib3f(positionAttrib,
				drawCenter.x + itr->sX * drawSize.x / 2.0f,
				drawCenter.y + itr->sY * drawSize.y / 2.0f,
				-0.97f
			);

		glVertexAttrib2f(
				texCoordAttrib,
				fontManager->fontData[itr->character][fontSize].sX,
				fontManager->fontData[itr->character][fontSize].eY
			);
		glVertexAttrib3f(positionAttrib,
				drawCenter.x + itr->sX * drawSize.x / 2.0f,
				drawCenter.y + itr->eY * drawSize.y / 2.0f,
				-0.97f
			);

		glVertexAttrib2f(
				texCoordAttrib,
				fontManager->fontData[itr->character][fontSize].eX,
				fontManager->fontData[itr->character][fontSize].eY
			);
		glVertexAttrib3f(positionAttrib,
				drawCenter.x + itr->eX * drawSize.x / 2.0f,
				drawCenter.y + itr->eY * drawSize.y / 2.0f,
				-0.97f
			);

		glVertexAttrib2f(
				texCoordAttrib,
				fontManager->fontData[itr->character][fontSize].eX,
				fontManager->fontData[itr->character][fontSize].sY
			);
		glVertexAttrib3f(positionAttrib,
				drawCenter.x + itr->eX * drawSize.x / 2.0f,
				drawCenter.y + itr->sY * drawSize.y / 2.0f,
				-0.97f
			);

		glEnd();
	}
}
