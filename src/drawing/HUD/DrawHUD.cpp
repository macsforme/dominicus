/*
 *  HUDRenderer.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 8/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "HUDRenderer.h"

HUDRenderer::HUDRenderer(unsigned int newWidth, unsigned int newHeight, SubjectRenderer* newSubjectRenderer) :
		width(newWidth), height(newHeight), subjectRenderer(newSubjectRenderer) {
	// set up font shader
	textVertexShader = ShaderTools::makeShader(
			std::string(Platform::getResourcePath() +  "/shaders/text.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	textFragmentShader = ShaderTools::makeShader(
			std::string(Platform::getResourcePath() + "/shaders/text.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> textShaders;
	textShaders.push_back(textVertexShader);
	textShaders.push_back(textFragmentShader);

	textProgram = ShaderTools::makeProgram(textShaders);

	textPositionAttrib = glGetAttribLocation(textProgram, "position");
	textTexCoordAttrib = glGetAttribLocation(textProgram, "texCoord");
	textTextureUniform = glGetUniformLocation(textProgram, "fontTexture");

	// set up hud shader
	hudVertexShader = ShaderTools::makeShader(
			std::string(Platform::getResourcePath() +  "/shaders/hud.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	hudFragmentShader = ShaderTools::makeShader(
			std::string(Platform::getResourcePath() + "/shaders/hud.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> hudShaders;
	hudShaders.push_back(hudVertexShader);
	hudShaders.push_back(hudFragmentShader);

	hudProgram = ShaderTools::makeProgram(hudShaders);

	hudPositionAttrib = glGetAttribLocation(hudProgram, "position");
	hudColorAttrib = glGetAttribLocation(hudProgram, "color");

	// create font manager
	fontManager = new FontManager();
	fontManager->populateCommonChars(FONT_SIZE);

	// initialize the fps tracking variables
	lastCheck = 0;
	frameCount = 0;
	currentFPS = 0.0f;
}

HUDRenderer::~HUDRenderer() {
	delete(fontManager);
}

Vector2 HUDRenderer::stringMaxAdvances(std::string str) {
	Vector2 advances(0.0f, 0.0f);
	char charArray[str.size() + 1];
	strcpy(charArray, str.c_str());

	for(int i = 0; i < str.size(); ++i) {
		if(fontManager->fontData[charArray[i]][FONT_SIZE].advanceX > advances.x)
			advances.x = fontManager->fontData[charArray[i]][FONT_SIZE].advanceX;
		if(fontManager->fontData[charArray[i]][FONT_SIZE].advanceY > advances.y)
			advances.y = fontManager->fontData[charArray[i]][FONT_SIZE].advanceY;
	}

	return advances;
}

Vector2 HUDRenderer::stringAdvances(std::string str) {
	Vector2 advances(0.0f, 0.0f);
	char charArray[str.size() + 1];
	strcpy(charArray, str.c_str());

	for(int i = 0; i < str.size(); ++i) {
		advances.x += fontManager->fontData[charArray[i]][FONT_SIZE].advanceX;

		if(fontManager->fontData[charArray[i]][FONT_SIZE].advanceY > advances.y)
			advances.y = fontManager->fontData[charArray[i]][FONT_SIZE].advanceY;
	}

	return advances;
}

void HUDRenderer::render() {
	// ===== Upper-Left Console =====
	std::vector<std::string> lines;

	// we need to track the console lines
	std::vector<Vector2> lineDimensions;

	// All floating-point numbers will be rounded to two decimal places.
	// In order to keep the width of the console box persistent in spite
	// of varying text dimensions, we will allot enough space for the
	// tallest and widest numbers to fit into each space.
	Vector2 numAdvances = stringMaxAdvances("0123456789");

	// add all applicable console lines
	std::stringstream buffer;

	// vertices
	buffer << "Vertices: " << subjectRenderer->subject.vertices.size();
	lines.push_back(buffer.str());
	lineDimensions.push_back(
			Vector2(
					stringAdvances(std::string("Vertices: ")).x +
							5.0f * numAdvances.x,
					(stringAdvances(std::string("Vertices: ")).y > numAdvances.y ?
							stringAdvances(std::string("Vertices: ")).y : numAdvances.y)
				)
		);
	buffer.str("");

	// triangles
	unsigned int numTris = 0;
	for(
			std::map< std::string,std::vector<Mesh::Face> >::iterator itr = subjectRenderer->subject.faceGroups.begin();
			itr != subjectRenderer->subject.faceGroups.end();
			++itr
		) numTris += itr->second.size();

	buffer << "Triangles: " << numTris;
	lines.push_back(buffer.str());
	lineDimensions.push_back(
			Vector2(
					stringAdvances(std::string("Triangles: ")).x +
							5.0f * numAdvances.x,
					(stringAdvances(std::string("Triangles: ")).y > numAdvances.y ?
							stringAdvances(std::string("Triangles: ")).y : numAdvances.y)
				)
		);
	buffer.str("");

	// empty space
	buffer << " ";
	lines.push_back(buffer.str());
	lineDimensions.push_back(stringAdvances(std::string(" ")));
	buffer.str("");

	// FPS
	if(Platform::getExecutionTimeMicros() - lastCheck > 1000000 / LOOP_TESTFREQUENCY) {
		currentFPS = frameCount * LOOP_TESTFREQUENCY;
		frameCount = 0;
		lastCheck = Platform::getExecutionTimeMicros();
	}

	++frameCount;

	buffer << "FPS: " << currentFPS;
	lines.push_back(buffer.str());
	lineDimensions.push_back(
			Vector2(
					stringAdvances(std::string("FPS: ")).x +
							4.0f * numAdvances.x,
					(stringAdvances(std::string("FPS: ")).y > numAdvances.y ?
							stringAdvances(std::string("FPS: ")).y : numAdvances.y)
				)
		);
	buffer.str("");

	// modes
	buffer << "Modes: Model";
	lines.push_back(buffer.str());
	lineDimensions.push_back(stringAdvances(std::string("Modes: Model")));
	buffer.str("");

	// horizontal angle
	char horizAngleString[7];
//	sprintf(horizAngleString, "%.2f", subjectRenderer->camera.getAngleX());

/*
	buffer << "Horizontal: " << horizAngleString;
	lines.push_back(buffer.str());
	lineDimensions.push_back(
			Vector2(
					stringAdvances(std::string("Horizontal: ")).x +
							5.0f * numAdvances.x +
							fontManager->fontData['.'][FONT_SIZE].width,
					(stringAdvances(std::string("Horizontal: ")).y > numAdvances.y ?
							stringAdvances(std::string("Horizontal: ")).y : numAdvances.y)
				)
		);
	buffer.str("");

	// vertical angle
	char verticalAngleString[7];
//	sprintf(verticalAngleString, "%.2f", subjectRenderer->camera.getAngleY());

	buffer << "Vertical: " << verticalAngleString;
	lines.push_back(buffer.str());
	lineDimensions.push_back(
			Vector2(
					stringAdvances(std::string("Vertical: ")).x +
							5.0f * numAdvances.x +
							fontManager->fontData['.'][FONT_SIZE].width,
					(stringAdvances(std::string("Vertical: ")).y > numAdvances.y ?
							stringAdvances(std::string("Vertical: ")).y : numAdvances.y)
				)
		);
	buffer.str("");

	// zoom
	char zoom[6];
	snprintf(zoom, 6, "%.2f", subjectRenderer->camera.getZoom());

	buffer << "Zoom: " << zoom;
	lines.push_back(buffer.str());
	lineDimensions.push_back(
			Vector2(
					stringAdvances(std::string("Zoom: ")).x +
							5.0f * numAdvances.x +
							fontManager->fontData['.'][FONT_SIZE].width,
					(stringAdvances(std::string("Zoom: ")).y > numAdvances.y ?
							stringAdvances(std::string("Zoom: ")).y : numAdvances.y)
				)
		);
	buffer.str("");
*/
	// determine the size of the console box
	unsigned int dimX = 0, dimY = 0;
	for(
			std::vector<Vector2>::iterator itr = lineDimensions.begin();
			itr != lineDimensions.end();
			++itr
		) {
		if(itr->x > dimX)
			dimX = itr->x;

		dimY += itr->y;
	}

	// use the HUD shader
	glUseProgram(hudProgram);

	// draw the console box
	glBegin(GL_QUADS);
//	glVertexAttrib4f(hudColorAttrib, 0.0f, 0.0f, 0.0f, 1.0f);
	glVertexAttrib4f(hudColorAttrib, 0.3f, 0.3f, 0.3f, 0.5f);

	glVertexAttrib2f(hudPositionAttrib, normalize(CONSOLE_BORDER / 2, width), normalize(height - dimY - CONSOLE_BORDER * 1.5, height));
	glVertexAttrib2f(hudPositionAttrib, normalize(CONSOLE_BORDER / 2, width), normalize(height - CONSOLE_BORDER / 2, height));
	glVertexAttrib2f(hudPositionAttrib, normalize(CONSOLE_BORDER * 1.5 + dimX, width), normalize(height - CONSOLE_BORDER / 2, height));
	glVertexAttrib2f(hudPositionAttrib, normalize(CONSOLE_BORDER * 1.5 + dimX, width), normalize(height - dimY - CONSOLE_BORDER * 1.5, height));

	glEnd();

	// use the font shader
	glUseProgram(textProgram);

	// activate the font textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fontManager->textureIDs[FONT_SIZE]);
	glUniform1i(textTextureUniform, 0);

	// set the "pen" position
	unsigned int penX = CONSOLE_BORDER, penY = height - CONSOLE_BORDER;
//	unsigned int penX = 0, penY = height;

	// draw each character
	for(unsigned int i = 0; i < lines.size(); ++i) {
		// create a char string with the line contents
		char line[lines[i].size() + 1];
		strcpy(line, lines[i].c_str());

		// subtract the maximum character height from the pen
		penY -= lineDimensions[i].y;

		// print each character
		for(int p = 0; p < lines[i].length(); ++p) {
			char character = line[p];

			// make sure the font is loaded (texture may get messed up this frame)
			if(! fontManager->isCharCached(character, FONT_SIZE))
				fontManager->buildChar(character, FONT_SIZE);
			// get the X/Y offset metrics from the font loader
			int xOffset = fontManager->fontData[character][FONT_SIZE].adjustX;
			int yOffset = fontManager->fontData[character][FONT_SIZE].adjustY;

			glBegin(GL_QUADS);

			glVertexAttrib2f(
					textTexCoordAttrib,
					fontManager->fontData[character][FONT_SIZE].sXCoord,
					fontManager->fontData[character][FONT_SIZE].sYCoord
				);
			glVertexAttrib2f(
					textPositionAttrib,
					normalize(penX + xOffset, width),
					normalize(penY + yOffset, height)
				);

			glVertexAttrib2f(
					textTexCoordAttrib,
					fontManager->fontData[character][FONT_SIZE].sXCoord,
					fontManager->fontData[character][FONT_SIZE].eYCoord
				);
			glVertexAttrib2f(
					textPositionAttrib,
					normalize(penX + xOffset, width),
					normalize(penY + yOffset + fontManager->fontData[character][FONT_SIZE].height, height)
				);

			glVertexAttrib2f(
					textTexCoordAttrib,
					fontManager->fontData[character][FONT_SIZE].eXCoord,
					fontManager->fontData[character][FONT_SIZE].eYCoord
				);
			glVertexAttrib2f(
					textPositionAttrib,
					normalize(penX + xOffset + fontManager->fontData[character][FONT_SIZE].width, width),
					normalize(penY + yOffset + fontManager->fontData[character][FONT_SIZE].height, height)
				);

			glVertexAttrib2f(
					textTexCoordAttrib,
					fontManager->fontData[character][FONT_SIZE].eXCoord,
					fontManager->fontData[character][FONT_SIZE].sYCoord
				);
			glVertexAttrib2f(
					textPositionAttrib,
					normalize(penX + xOffset + fontManager->fontData[character][FONT_SIZE].width, width),
					normalize(penY + yOffset, height)
				);

			glEnd();

			penX += fontManager->fontData[character][FONT_SIZE].advanceX;
		}

		penX = CONSOLE_BORDER;
	}
}
