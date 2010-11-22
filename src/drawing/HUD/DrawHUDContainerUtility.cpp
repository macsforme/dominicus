/*
 *  DrawHUDContainerUtility.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/8/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DrawHUDContainerUtility.h"

DrawHUDContainerUtility::DrawHUDContainerUtility(
		float zStart,
		float borderWidth,
		float newInsideColor[4],
		float newHighlightColor[4],
		float newBorderColor[4],
		float newOutsideColor[4]
	) :	zStart(zStart), borderWidth(borderWidth) {
	// set variables
	memcpy(insideColor, newInsideColor, 4 * sizeof(float));
	memcpy(highlightColor, newHighlightColor, 4 * sizeof(float));
	memcpy(borderColor, newBorderColor, 4 * sizeof(float));
	memcpy(outsideColor, newOutsideColor, 4 * sizeof(float));

	// set up container shader
	containerVertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/hudContainer.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	containerFragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/hudContainer.fragment.glsl").c_str(),
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

void DrawHUDContainerUtility::drawCurve(Vector2 position, Vector2 dimensions, float rotation,
		bool highlight = false, float zMod = 0.0f, bool concave = false) {
	// draws the curved portion of the console container at the specified place and size
	glUseProgram(containerProgram);

	glUniform2f(
			curveOriginCoordUniform,
			-(sqrt(2.0f) * cos(radians(rotation + 45.0f))),
			-(sqrt(2.0f) * sin(radians(rotation + 45.0f)))
		);
	glUniform1f(border1DistUniform, 2.0f - borderWidth);
	glUniform1f(border2DistUniform, 2.0f);
	glUniform1f(zModUniform, zStart + zMod);

	if(concave) {
		glUniform4fv(insideColorUniform, 1, outsideColor);

		if(highlight)
			glUniform4fv(outsideColorUniform, 1, highlightColor);
		else
			glUniform4fv(outsideColorUniform, 1, insideColor);
	} else {
		if(highlight)
			glUniform4fv(insideColorUniform, 1, highlightColor);
		else
			glUniform4fv(insideColorUniform, 1, insideColor);

		glUniform4fv(outsideColorUniform, 1, outsideColor);
	}
	glUniform4fv(borderColorUniform, 1, borderColor);

	glBegin(GL_QUADS);

	glVertexAttrib2f(
			positionAttrib,
			position.x - dimensions.x / 2,
			position.y - dimensions.y / 2
		);
	glVertexAttrib2f(primCoordAttrib, -1.0f, -1.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x - dimensions.x / 2,
			position.y + dimensions.y / 2
		);
	glVertexAttrib2f(primCoordAttrib, -1.0f, 1.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			position.y + dimensions.y / 2
		);
	glVertexAttrib2f(primCoordAttrib, 1.0f, 1.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			position.y - dimensions.y / 2
		);
	glVertexAttrib2f(primCoordAttrib, 1.0f, -1.0f);

	glEnd();
}

void DrawHUDContainerUtility::drawBorder(Vector2 position, Vector2 dimensions, float rotation,
		bool highlight, float zMod) {
	// draws the straight border portion of the console container at the specified place and size
	glUseProgram(containerProgram);

	glUniform2f(
			curveOriginCoordUniform,
			-cos(radians(rotation)),
			-sin(radians(rotation))
		);
	glUniform1f(border1DistUniform, 2.0f - borderWidth);

	glUniform1f(border2DistUniform, 2.0f);
	glUniform1f(zModUniform, zStart + zMod);

	if(highlight)
		glUniform4fv(insideColorUniform, 1, highlightColor);
	else
		glUniform4fv(insideColorUniform, 1, insideColor);

	glUniform4fv(borderColorUniform, 1, borderColor);
	glUniform4fv(outsideColorUniform, 1, outsideColor);

	// rotate the primCoord arguments around as rotated

	glBegin(GL_QUADS);

	glVertexAttrib2f(
			positionAttrib,
			(position.x - dimensions.x / 2),
			position.y - dimensions.y / 2
		);
	glVertexAttrib2f(
			primCoordAttrib,
			-absolute(cos(radians(rotation))),
			-absolute(sin(radians(rotation)))
		);

	glVertexAttrib2f(
			positionAttrib,
			(position.x - dimensions.x / 2),
			position.y + dimensions.y / 2
		);
	glVertexAttrib2f(
			primCoordAttrib,
			-absolute(cos(radians(rotation))),
			absolute(sin(radians(rotation)))
		);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			position.y + dimensions.y / 2
		);
	glVertexAttrib2f(
			primCoordAttrib,
			absolute(cos(radians(rotation))),
			absolute(sin(radians(rotation)))
		);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			position.y - dimensions.y / 2
		);
	glVertexAttrib2f(
			primCoordAttrib,
			absolute(cos(radians(rotation))),
			-absolute(sin(radians(rotation)))
	);

	glEnd();
}

void DrawHUDContainerUtility::drawFiller(Vector2 position, Vector2 dimensions, bool highlight = false,
		float zMod = 0.0f) {
	// draws the filler portion of the console container at the specified place and size
	glUseProgram(containerProgram);

	glUniform2f(curveOriginCoordUniform, 0.0f, 0.0f);
	glUniform1f(border1DistUniform, 1.0f);
	glUniform1f(border2DistUniform, 1.0f);

	if(highlight)
		glUniform4fv(insideColorUniform, 1, highlightColor);
	else
		glUniform4fv(insideColorUniform, 1, insideColor);

	glUniform4fv(borderColorUniform, 1, borderColor);
	glUniform4fv(outsideColorUniform, 1, outsideColor);
	glUniform1f(zModUniform, zStart + zMod);

	glBegin(GL_QUADS);

	glVertexAttrib2f(
			positionAttrib,
			position.x - dimensions.x / 2,
			position.y - dimensions.y / 2
		);
	glVertexAttrib2f(primCoordAttrib, 0.0f, 0.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x - dimensions.x / 2,
			position.y + dimensions.y / 2
		);
	glVertexAttrib2f(primCoordAttrib, 0.0f, 0.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			position.y + dimensions.y / 2
		);
	glVertexAttrib2f(primCoordAttrib, 0.0f, 0.0f);

	glVertexAttrib2f(
			positionAttrib,
			position.x + dimensions.x / 2,
			position.y - dimensions.y / 2
		);
	glVertexAttrib2f(primCoordAttrib, 0.0f, 0.0f);

	glEnd();
}

/*
// console container constants
#define INSIDE_COLOR 0.047058823529412f, 0.043137254901961f, 0.137254901960784f, 0.6f
#define HIGHLIGHT_COLOR 0.274509803921569f, 0.298039215686275f, 0.403921568627451f, 1.0f
#define BORDER_COLOR 0.52156862745098f, 0.568627450980392f, 0.537254901960784f, 1.0f
#define OUTSIDE_COLOR 0.0f, 0.0f, 0.0f, 0.0f
#define CONTAINER_BORDER_WIDTH 8.0f / screenWidth * 2.0f
*/

/*
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
