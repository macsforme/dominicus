/*
 * RenderingMaster.cpp
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

#include "drawing/rendering/RenderingMaster.h"

RenderingMaster::RenderingMaster() {
	bCamera = new BackCamera();
	fpCamera = new FirstPersonCamera();
	wvCamera = new WorldViewCamera();

	cameraToggleKeyTrap = new KeyTrap("toggleCamera");

	activeMatrix = &(bCamera->terrainVMatrix);
}

void RenderingMaster::loop() {
	bCamera->loop();
	fpCamera->loop();
	wvCamera->loop();

	cameraToggleKeyTrap->loop();

	// toggle cameras if necessary
	if(cameraToggleKeyTrap->newPress()) {
		if(activeMatrix == &(bCamera->terrainVMatrix))
			activeMatrix = &(fpCamera->vpMatrix);
		else if(activeMatrix == &(fpCamera->vpMatrix))
			activeMatrix = &(wvCamera->vpMatrix);
		else if(activeMatrix == &(wvCamera->vpMatrix))
			activeMatrix = &(bCamera->terrainVMatrix);
	}

//	skyRenderer.render(*activeMatrix);
	if(activeMatrix == &(bCamera->terrainVMatrix)) {
		terrainRenderer.render(bCamera->terrainVMatrix * bCamera->pMatrix);
		shipRenderer.render(bCamera->shipVMatrix, bCamera->pMatrix);
	} else {
		terrainRenderer.render(*activeMatrix);
	}
}
