/*
 *  DrawingMaster.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 7/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "drawing/DrawingMaster.h"

DrawingMaster::DrawingMaster() {
	// initialize variables
	renderingMaster = new RenderingMaster();
	fontManager = new FontManager();

	hudDrawing = new DrawHUD(fontManager);
	drawControlBox = new DrawControlBox();
	drawCursor = new DrawCursor();

	// initialize fonts
	fontManager->populateCommonChars((int) gamePrefs.getFloat("fontStandardSize"));

	// set the OpenGL viewport size
	setViewPortSize(gameWindow->width, gameWindow->height);

	// set the FPS cap key trap
	fpsCapKeyTrap = new KeyTrap("toggleFPSCap");
	capFPS = gamePrefs.getBool("renderingCapFPS");
}

DrawingMaster::~DrawingMaster() {
	// any OpenGL commands to run upon context destruction

	// delete our variables
	delete(fpsCapKeyTrap);
}

void DrawingMaster::setViewPortSize(unsigned short int width, unsigned short int height) {
	// set the viewport size in GL
	glViewport(
			0,
			0,
			width,
			height
		);
}

void DrawingMaster::preFrame() {
	// complete any pre-frame operations
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glClearColor(0.447058823529412f, 0.407843137254902f, 0.298039215686275f, 1.0f);
//	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClearColor(0.58f, 0.71f, 0.79f, 1.0f);
//	glClearColor(0.0f, 0.02f, 0.24f, 1.0f);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawingMaster::postFrame() {
	// complete any post-frame operations
	glDisable(GL_BLEND);

	glDisable(GL_CULL_FACE);

	glDisable(GL_DEPTH_TEST);

	// swap buffers
	SDL_GL_SwapBuffers();

	// check for OpenGL errors
	GLenum renderingError = glGetError();
	while(renderingError != GL_NO_ERROR) {
		// log the error and check for another one
		std::stringstream err;
		err << "OpenGL returned an error: "
				<< renderingError;
		ProgramLog::report(LOG_DEBUG, err.str().c_str());

		renderingError = glGetError();
	}
}

unsigned int DrawingMaster::execute() {
	// execute pre-frame instructions
	preFrame();

	// draw the frame
	renderingMaster->loop();
	hudDrawing->draw();
	drawControlBox->draw();
	drawCursor->draw();

	// execute post-frame instructions
	postFrame();

	// see if we need to cap our FPS
	fpsCapKeyTrap->loop();
	if(fpsCapKeyTrap->newPress())
		capFPS = !capFPS;

	if(! capFPS)
		return 0;

	// calculate and return sleep time from superclass
	unsigned int frequency = (unsigned int) gamePrefs.getFloat("renderingFPS");
	unsigned int idealSleepTime = (
			frequency != 0 ?
			1000 / frequency : 0
		);
	return getSleepTime(idealSleepTime);
}
