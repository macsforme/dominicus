/*
 *  DrawConsole.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/8/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DrawConsole.h"

void DrawConsole::drawCurve(Vector2 position, Vector2 dimensions, float rotation,
		bool highlight = false, float zMod = 0.0f, bool concave = false) {
	// draws the curved portion of the console container at the specified place and size
	glUniform2f(
			curveOriginCoordUniform,
			-(sqrt(2.0f) * cos(radians(rotation + 45.0f))),
			-(sqrt(2.0f) * sin(radians(rotation + 45.0f)))
		);
	glUniform1f(border1DistUniform, 2.0f - (CONTAINER_BORDER_WIDTH * 2.0f / dimensions.x));
	glUniform1f(border2DistUniform, 2.0f);
	glUniform1f(zModUniform, zMod);

	if(concave) {
		glUniform4f(insideColorUniform, OUTSIDE_COLOR);

		if(highlight)
			glUniform4f(outsideColorUniform, HIGHLIGHT_COLOR);
		else
			glUniform4f(outsideColorUniform, INSIDE_COLOR);
	} else {
		if(highlight)
			glUniform4f(insideColorUniform, HIGHLIGHT_COLOR);
		else
			glUniform4f(insideColorUniform, INSIDE_COLOR);

		glUniform4f(outsideColorUniform, OUTSIDE_COLOR);
	}
	glUniform4f(borderColorUniform, BORDER_COLOR);

	const float screenRatio = (float) screenWidth / (float) screenHeight;

	glBegin(GL_QUADS);

	glVertexAttrib2f(
			positionAttrib,
			position.x - dimensions.x / 2,
			(position.y - dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(primCoordAttrib, -1.0f, -1.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x - dimensions.x / 2,
			(position.y + dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(primCoordAttrib, -1.0f, 1.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			(position.y + dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(primCoordAttrib, 1.0f, 1.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			(position.y - dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(primCoordAttrib, 1.0f, -1.0f);

	glEnd();
}

void DrawConsole::drawBorder(Vector2 position, Vector2 dimensions, float rotation,
		bool highlight = false, float zMod = 0.0f) {
	// draws the straight border portion of the console container at the specified place and size
	glUniform2f(
			curveOriginCoordUniform,
			-cos(radians(rotation)),
			-sin(radians(rotation))
		);

	glUniform1f(border1DistUniform, 2.0f -
			(CONTAINER_BORDER_WIDTH * 2.0f / (
					absolute(dimensions.x * cos(radians(rotation))) +
					absolute(dimensions.y * sin(radians(rotation)))
			))
		);
	glUniform1f(border2DistUniform, 2.0f);
	glUniform1f(zModUniform, zMod);

	if(highlight)
		glUniform4f(insideColorUniform, HIGHLIGHT_COLOR);
	else
		glUniform4f(insideColorUniform, INSIDE_COLOR);

	glUniform4f(borderColorUniform, BORDER_COLOR);
	glUniform4f(outsideColorUniform, OUTSIDE_COLOR);

	const float screenRatio = (float) screenWidth / (float) screenHeight;

	// rotate the primCoord arguments around as rotated

	glBegin(GL_QUADS);

	glVertexAttrib2f(
			positionAttrib,
			(position.x - dimensions.x / 2),
			(position.y - dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(
			primCoordAttrib,
			-absolute(cos(radians(rotation))),
			-absolute(sin(radians(rotation)))
		);

	glVertexAttrib2f(
			positionAttrib,
			(position.x - dimensions.x / 2),
			(position.y + dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(
			primCoordAttrib,
			-absolute(cos(radians(rotation))),
			absolute(sin(radians(rotation)))
		);

	glVertexAttrib2f(
			positionAttrib,
			(position.x + dimensions.x / 2),
			(position.y + dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(
			primCoordAttrib,
			absolute(cos(radians(rotation))),
			absolute(sin(radians(rotation)))
		);

	glVertexAttrib2f(
			positionAttrib,
			(position.x + dimensions.x / 2),
			(position.y - dimensions.y / 2) * screenRatio
		);
		glVertexAttrib2f(
			primCoordAttrib,
			absolute(cos(radians(rotation))),
			-absolute(sin(radians(rotation)))
		);

	glEnd();
}

void DrawConsole::drawFiller(Vector2 position, Vector2 dimensions, bool highlight = false, float zMod = 0.0f) {
	// draws the filler portion of the console container at the specified place and size
	glUniform2f(curveOriginCoordUniform, 0.0f, 0.0f);
	glUniform1f(border1DistUniform, 1.0f);
	glUniform1f(border2DistUniform, 1.0f);

	if(highlight)
		glUniform4f(insideColorUniform, HIGHLIGHT_COLOR);
	else
		glUniform4f(insideColorUniform, INSIDE_COLOR);

	glUniform4f(borderColorUniform, BORDER_COLOR);
	glUniform4f(outsideColorUniform, OUTSIDE_COLOR);
	glUniform1f(zModUniform, zMod);

	const float screenRatio = (float) screenWidth / (float) screenHeight;

	glBegin(GL_QUADS);

	glVertexAttrib2f(
			positionAttrib,
			position.x - dimensions.x / 2,
			(position.y - dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(primCoordAttrib, 0.0f, 0.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x - dimensions.x / 2,
			(position.y + dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(primCoordAttrib, 0.0f, 0.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			(position.y + dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(primCoordAttrib, 0.0f, 0.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			(position.y - dimensions.y / 2) * screenRatio
		);
	glVertexAttrib2f(primCoordAttrib, 0.0f, 0.0f);

	glEnd();
}

DrawConsole::DrawConsole(unsigned int screenWidth, unsigned int screenHeight) :
		screenWidth(screenWidth), screenHeight(screenHeight) {
	// set up container shader
	containerVertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/consoleContainer.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	containerFragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/consoleContainer.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	// compile the container shader
	std::vector<GLuint> containerShaders;
	containerShaders.push_back(containerVertexShader);
	containerShaders.push_back(containerFragmentShader);

	containerProgram = ShaderTools::makeProgram(containerShaders);

	// get uniform locations
	insideColorUniform = glGetUniformLocation(containerProgram, "insideColor");
	borderColorUniform = glGetUniformLocation(containerProgram, "borderColor");
	outsideColorUniform = glGetUniformLocation(containerProgram, "outsideColor");

	curveOriginCoordUniform = glGetUniformLocation(containerProgram, "curveOriginCoord");
	border1DistUniform = glGetUniformLocation(containerProgram, "border1Dist");
	border2DistUniform = glGetUniformLocation(containerProgram, "border2Dist");
	zModUniform = glGetUniformLocation(containerProgram, "zMod");

	// get attribute locations
	positionAttrib = glGetAttribLocation(containerProgram, "position");
	primCoordAttrib = glGetAttribLocation(containerProgram, "primCoord");
}

void DrawConsole::render() {
	glUseProgram(containerProgram);
/*
	drawCurve(Vector2(0.75f, 0.75f), Vector2(0.5f, 0.5f), 0.0f);
	drawCurve(Vector2(-0.75f, 0.75f), Vector2(0.5f, 0.5f), 90.0f);
	drawCurve(Vector2(-0.75f, -0.75f), Vector2(0.5f, 0.5f), 180.0f);
	drawCurve(Vector2(0.75f, -0.75f), Vector2(0.5f, 0.5f), 270.0f);

	drawBorder(Vector2(0.75f, 0.0f), Vector2(0.5f, 1.0f), 0.0f);
	drawBorder(Vector2(0.0f, 0.75f), Vector2(1.0f, 0.5f), 90.0f);
	drawBorder(Vector2(-0.75f, 0.0f), Vector2(0.5f, 1.0f), 180.0f);
	drawBorder(Vector2(0.0f, -0.75f), Vector2(1.0f, 0.5f), 270.0f);

	drawFiller(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
*/
/*
	drawCurve(Vector2(-0.5f, -0.5f), Vector2(1.0f, 1.0f), 180, true);
	drawCurve(Vector2(-0.5f, 0.5f), Vector2(1.0f, 1.0f), 90);
	drawCurve(Vector2(0.5f, 0.5f), Vector2(1.0f, 1.0f), 0, true);
	drawCurve(Vector2(0.5f, -0.5f), Vector2(1.0f, 1.0f), 270);
*/

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
