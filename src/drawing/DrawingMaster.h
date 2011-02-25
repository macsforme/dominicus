/*
 * DrawingMaster.h
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

#ifndef DRAWINGMASTER_H
#define DRAWINGMASTER_H

// program headers
#include "core/GamePrefs.h"
#include "core/MainLoopMember.h"
#include "core/ProgramLog.h"
#include "core/SystemInfo.h"
#include "drawing/controlbox/DrawControlBox.h"
#include "drawing/cursor/DrawCursor.h"
#include "drawing/HUD/DrawHUD.h"
#include "drawing/rendering/RenderingMaster.h"
#include "input/KeyTrap.h"
#include "platform/OpenGLHeaders.h"
#include "platform/Platform.h"
#include "windowing/GameWindow.h"

// library headers
#include <sstream>
#include <SDL/SDL.h>

// global variables
extern GamePrefs* gamePrefs;
extern GameWindow* gameWindow;
extern SystemInfo* systemInfo;
extern Platform* platform;
extern ProgramLog* programLog;

class DrawingMaster : public MainLoopMember {
private:
	FontManager* fontManager;

	DrawControlBox* drawControlBox;
	DrawCursor* drawCursor;
	DrawHUD* hudDrawing;

	KeyTrap* fpsCapKeyTrap;
	bool capFPS;

public:
	RenderingMaster* renderingMaster;

	DrawingMaster();
	~DrawingMaster();

	void setViewPortSize(unsigned short int width, unsigned short int height);
	void preFrame();
	void postFrame();

	unsigned int execute();
};

#endif
