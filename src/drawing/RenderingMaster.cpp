/*
 *  RenderingMaster.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 7/7/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "RenderingMaster.h"

RenderingMaster::RenderingMaster() {
	// set common OpenGL state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.447058823529412f, 0.407843137254902f, 0.298039215686275f, 1.0f);

	if(gamePrefs.getBool("windowStartFullScreen"))
		setViewPortSize(systemInfo.screenWidth, systemInfo.screenHeight);
	else
		setViewPortSize(gamePrefs.getInt("windowWidth"), gamePrefs.getInt("windowHeight"));
}

RenderingMaster::~RenderingMaster() {
	// undo common OpenGL state
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void RenderingMaster::setViewPortSize(unsigned short int width, unsigned short int height) {
	// set the viewport size in GL
	glViewport(
			0,
			0,
			width,
			height
		);
}

void RenderingMaster::preFrame() {
	// complete any pre-frame operations

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderingMaster::postFrame() {
	// complete any post-frame operations

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

unsigned long int RenderingMaster::loop() {
	// execute pre-frame instructions
	preFrame();

	// draw the frame

	// execute post-frame instructions
	postFrame();

	// calculate the maximum amount of time we can sleep in order
	// to maintain our desired frequency
	static unsigned long int last = platform.getExecutionTimeMicros();
	static unsigned long int sleepMicros = 1;

	unsigned long int now = platform.getExecutionTimeMicros();
	unsigned long int idealSleepTime = (
			gamePrefs.getInt("renderingFPS") != 0 ?
			1000000 / gamePrefs.getInt("renderingFPS") : 0
		);

	// adjust the target sleep micros by the factor we are off by
	sleepMicros = (sleepMicros > 0 ? sleepMicros : 1) * ((double) idealSleepTime / (double) (now - last));

	last = now;

	return sleepMicros;
}

