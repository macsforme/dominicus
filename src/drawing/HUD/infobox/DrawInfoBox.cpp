/*
 *  DrawInfoBox.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 11/26/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "drawing/HUD/infobox/DrawInfoBox.h"

DrawInfoBox::DrawInfoBox(HUDArrangement* hudArrangement, FontManager* fontManager) :
		fontManager(fontManager) {
	// set up the HUD laying arrangement info
	myElement = new HUDElement(Vector2(0.5f / gameWindow->aspectRatio, 0.5f));

	HUDArrangement::HUDElementEntry myEntry;
	myEntry.element = myElement;
	myEntry.bearing1 = HUDArrangement::BEARING_TOP;
	myEntry.bearing2 = HUDArrangement::BEARING_RIGHT;

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
	colorAttrib = 2;
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "texCoord");
	glBindAttribLocation(program, 2, "color");

	ShaderTools::linkProgram(program);

	// get uniform locations
	mvpMatrixUniform = glGetUniformLocation(program, "mvpMatrix");
	textureUniform = glGetUniformLocation(program, "texture");
	useTextureUniform = glGetUniformLocation(program, "useTexture");
	useLightingUniform = glGetUniformLocation(program, "useLighting");

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
	lastCheck = 0;
	frameCount = 0;
	currentFPS = 0;
	currentExecTime = 0.0f;
}

void DrawInfoBox::draw() {
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
			0.0f
		);
	containerUtility->drawBorder(
			Vector2(myElement->position.x + contentSize.x / 2.0f +
				containerPadding.x / 2.0f, myElement->position.y),
			Vector2(containerPadding.x, contentSize.y),
			0.0f,
			false,
			0.0f
		);

	containerUtility->drawBorder(
			Vector2(myElement->position.x, myElement->position.y -
					contentSize.y / 2.0f - containerPadding.y / 2.0f),
			Vector2(contentSize.x, containerPadding.y),
			270.0f,
			false,
			0.005f
		);
	containerUtility->drawBorder(
			Vector2(myElement->position.x, myElement->position.y +
					contentSize.y / 2.0f + containerPadding.y / 2.0f),
			Vector2(contentSize.x, containerPadding.y),
			90.0f,
			false,
			0.005f
		);

	// corners
	containerUtility->drawCurve(
			myElement->position + contentSize / 2.0 + containerPadding / 2.0,
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

	// get ready for our content
	std::string outputLines = "";

	// update the FPS
	if(platform.getExecMills() - lastCheck >
			(1000 / (unsigned int) gamePrefs.getFloat("renderingDisplayUpdateFrequency"))) {
		currentFPS = (unsigned int) (frameCount * gamePrefs.getFloat("renderingDisplayUpdateFrequency"));
		frameCount = 0;
		lastCheck = platform.getExecMills();

		delete(textBlock);
		textBlock = NULL;
	}

	++frameCount;

	std::stringstream fpsLine;
	fpsLine << "FPS: " << currentFPS << "\n";
	outputLines += fpsLine.str();

	// update the execution time
	if(platform.getExecMills() - (unsigned int) (currentExecTime * 1000.0f) > 100) {
		currentExecTime = round((float) platform.getExecMills() / 1000.0f, 3);

		delete(textBlock);
		textBlock = NULL;
	}

	char timeStr[4];
	sprintf(timeStr, "%.1f", currentExecTime);

	std::stringstream execTimeLine;
	execTimeLine << "Time: " << timeStr << "\n";
	outputLines += execTimeLine.str();

	// add the controls info
	outputLines += "\n+/-: speed\narrows: direction\n";
	outputLines += "\nf: fullscreen\nk: view\nc: fps cap\nt: new terrain\nesc: quit\n";

	// if need be, regenerate the text block
	if(textBlock == NULL)
		textBlock = new TextBlock(outputLines.c_str(),
				(unsigned int)((myElement->size.x  - containerPadding.x * 2.0f) / 2.0f * gameWindow->width),
				(unsigned int)((myElement->size.y  - containerPadding.y * 2.0f) / 2.0f * gameWindow->height),
				fontManager,
				(int) gamePrefs.getFloat("fontStandardSize")
			);

	// draw the text
	unsigned int fontSize = (unsigned int) gamePrefs.getFloat("fontStandardSize");

	glUseProgram(program);

	float mvpMatrixArray[] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrixArray);

	glUniform1i(useTextureUniform, 1);
	glUniform1i(useLightingUniform, 0);

	glVertexAttrib4f(colorAttrib, 1.0f, 1.0f, 1.0f, 1.0f);

	GLuint textureID = fontManager->textureIDs[fontSize];

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(textureUniform, 0);

	// align the text to the top-left of the content box
	Vector2 drawSize(contentSize);
	drawSize.x *= (float) textBlock->width / (contentSize.x / 2.0f * (float) gameWindow->width);
	drawSize.y *= (float) textBlock->height / (contentSize.y / 2.0f * (float) gameWindow->height);

	Vector2 drawCenter(myElement->position);
	drawCenter.x -= contentSize.x / 2.0f;
	drawCenter.x += drawSize.x / 2.0f;
	drawCenter.y += contentSize.y / 2.0f;
	drawCenter.y -= drawSize.y / 2.0f;

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
