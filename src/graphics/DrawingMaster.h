// DrawingMaster.h
// Dominicus

#ifndef DRAWINGMASTER_H
#define DRAWINGMASTER_H

#include <map>
#include <string>

#include "core/MainLoopMember.h"
#include "graphics/DrawTypes.h"
#include "graphics/UILayoutAuthority.h"

class DrawingMaster : public MainLoopMember {
public:
	std::map<std::string, BaseDrawNode*> drawers;

	DrawStack drawStack;

	UILayoutAuthority* uiLayoutAuthority;

	DrawingMaster();
	~DrawingMaster();

	void buildDrawers();
	void destroyDrawers();
	void newGraphics();

	unsigned int execute(bool unScheduled = false);
};

#endif // DRAWINGMASTER_H
