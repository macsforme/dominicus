/*
 *  dominicusMain.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/11/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#ifndef DOMINICUSMAIN_H
#define DOMINICUSMAIN_H

// program headers
#include "core/GamePrefs.h"
#include "core/SystemInfo.h"
#include "drawing/DrawingMaster.h"
#include "state/Terrain.h"
#include "input/InputHandler.h"
#include "input/KeyTrap.h"
#include "math/VectorMath.h"
#include "platform/Platform.h"
#include "state/Ship.h"
#include "state/ShipControl.h"
#include "windowing/GameWindow.h"

int dominicusMain(int argc, char* argv[]);

#endif
