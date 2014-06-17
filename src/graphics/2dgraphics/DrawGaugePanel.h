// DrawGaugePanel.h
// Crucible Island

#ifndef DRAWGAUGEPANEL_H
#define DRAWGAUGEPANEL_H

// forward declarations
class DrawContainer;
class DrawProgressBar;
class DrawTexture;

// program headers
#include "graphics/GameGraphics.h"
#include "graphics/2dgraphics/DrawContainer.h"
#include "graphics/2dgraphics/DrawProgressBar.h"
#include "graphics/2dgraphics/DrawTexture.h"
#include "logic/UILayoutAuthority.h"
#include "math/VectorMath.h"
#include "platform/OpenGLHeaders.h"

// global variables
extern GameGraphics* gameGraphics;

class DrawGaugePanel : public BaseUIElement {
private:
	DrawContainer* containerDrawer;
	DrawProgressBar* progressBarDrawer;
	DrawTexture* textureDrawer;

public:
	DrawGaugePanel();
	~DrawGaugePanel();

	/*
		Arguments Layout
		----------------
		metrics					UI element metrics
		padding					width/height of curved edges and space between elements in pixels
		border					thickness of border in pixels
		softEdge				thickness of antialiasing of border in pixels
		insideColor				color of inside of container
		borderColor				color of border of container
		outsideColor			color of outside of container
		elements				Number of elements on gauge
		textureNames			std::vector<std::string> with texture names
		progressBarSize			Vector2 with size of progress bar in pixels
		progressions			std::vector<float> with progress bar progressions
		backgroundColorTop		Vector4 with color of top of progress bar
		backgroundColorBottom	Vector4 with color of bottom of progress bar
		progressBarColorsTop	std::vector<Vector4> with colors of tops of progress bars
		progressBarColorsBottom	std::vector<Vector4> with colors of tops of progress bars
	*/

	Vector2 getSize(std::map<std::string, void*> arguments);
	void execute(std::map<std::string, void*> arguments);
};

#endif // DRAWGAUGEPANEL_H
