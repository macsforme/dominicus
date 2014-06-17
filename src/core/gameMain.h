// gameMain.h
// Crucible Island

#ifndef GAMEMAIN_H
#define GAMEMAIN_H

// program headers
#include "audio/GameAudio.h"
#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "input/InputHandler.h"
#include "input/Keyboard.h"
#include "logic/GameLogic.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "state/GameState.h"

// library headers
#include <map>
#include <SDL.h>
#include <vector>

// main game function declaration
int gameMain(int argc, char* argv[]);

#endif // GAMEMAIN_H
