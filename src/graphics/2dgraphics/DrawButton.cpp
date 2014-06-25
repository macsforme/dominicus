// DrawButton.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawButton.h"

#include <map>
#include <string>

#include "graphics/GameGraphics.h"
#include "graphics/UILayoutAuthority.h"

extern GameGraphics* gameGraphics;

DrawButton::DrawButton(DrawContainer* newContainerDrawer, DrawLabel* newLabelDrawer) :
		containerDrawer(newContainerDrawer),
		labelDrawer(newLabelDrawer) { }

DrawStackArgList DrawButton::instantiateArgList() {
	DrawStackArgList argList;

	argList["border"] = (void*) new float;			// thickness of border in pixels
	argList["borderColor"] = (void*) new Vector4;	// color of border of container
	argList["fontColor"] = (void*) new Vector4;		// font base color
	argList["fontSize"] = (void*) new float;		// font point size for this label
	argList["insideColor"] = (void*) new Vector4;	// color of inside of container
	argList["metrics"] = (void*) new UIMetrics;		// UI element metrics
	argList["outsideColor"] = (void*) new Vector4;	// color of outside of container
	argList["padding"] = (void*) new float;			// width/height of curved edge in pixels
	argList["softEdge"] = (void*) new float;		// thickness of antialiasing of border in pixels
	argList["size"] = (void*) new Vector2;			// manually-specified width/height of button in screen dimensions (optional)
	argList["text"] = (void*) new std::string;		// text to draw

	return argList;
}

void DrawButton::deleteArgList(DrawStackArgList argList) {
	if(argList.find("border") != argList.end()) delete (float*) argList["border"];
	if(argList.find("borderColor") != argList.end()) delete (Vector4*) argList["borderColor"];
	if(argList.find("fontColor") != argList.end()) delete (Vector4*) argList["fontColor"];
	if(argList.find("fontSize") != argList.end()) delete (float*) argList["fontSize"];
	if(argList.find("insideColor") != argList.end()) delete (Vector4*) argList["insideColor"];
	if(argList.find("metrics") != argList.end()) delete (UIMetrics*) argList["metrics"];
	if(argList.find("outsideColor") != argList.end()) delete (Vector4*) argList["outsideColor"];
	if(argList.find("padding") != argList.end()) delete (float*) argList["padding"];
	if(argList.find("softEdge") != argList.end()) delete (float*) argList["softEdge"];
	if(argList.find("size") != argList.end()) delete (Vector2*) argList["size"];
	if(argList.find("text") != argList.end()) delete (std::string*) argList["text"];
}

Vector2 DrawButton::getSize(DrawStackArgList argList) {
	if(argList.find("size") != argList.end())
		return *((Vector2*) argList["size"]);

	Vector2 padding = Vector2(
		*((float*) argList["padding"]) / (float) gameGraphics->resolutionX * 2.0f,
		*((float*) argList["padding"]) / (float) gameGraphics->resolutionY * 2.0f
	);

	Vector2 newSize = labelDrawer->getSize(argList);

	newSize.x += padding.x * 2.0f;
	newSize.y += padding.y * 2.0f;

	return newSize;
}

void DrawButton::execute(DrawStackArgList argList) {
	std::map<std::string, void*> containerArgs = argList;
	if(containerArgs.find("size") == containerArgs.end()) {
		containerArgs["size"] = new Vector2; *((Vector2*) containerArgs["size"]) = getSize(argList);
		containerDrawer->execute(containerArgs);
		delete (Vector2*) containerArgs["size"];
	} else {
		containerDrawer->execute(containerArgs);
	}

	labelDrawer->execute(argList);
}
