// DrawGaugePanel.cpp
// Dominicus

#include "graphics/2dgraphics/DrawGaugePanel.h"

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

#include "graphics/GameGraphics.h"
#include "graphics/UILayoutAuthority.h"

extern GameGraphics* gameGraphics;

DrawStackArgList DrawGaugePanel::instantiateArgList() {
	DrawStackArgList argList;

	argList["backgroundColorBottom"] = (void*) new Vector4;					// base color of bottom of progress bars
	argList["backgroundColorTop"] = (void*) new Vector4;					// base color of top of progress bars
	argList["border"] = (void*) new float;									// thickness of border in pixels
	argList["borderColor"] = (void*) new Vector4;							// color of border of container
	argList["elements"] = (void*) new size_t;								// number of elements on gauge
	argList["fontColor"] = (void*) new Vector4;								// font base color
	argList["fontSize"] = (void*) new float;								// font point size for this label
	argList["insideColor"] = (void*) new Vector4;							// color of inside of container
	argList["metrics"] = (void*) new UIMetrics;								// UI element metrics
	argList["outsideColor"] = (void*) new Vector4;							// color of outside of container
	argList["padding"] = (void*) new float;									// width/height of curved edge in pixels
	argList["progressBarColorsBottom"] = (void*) new std::vector<Vector4>;	// colors of bottoms of progress bars
	argList["progressBarColorsTop"] = (void*) new std::vector<Vector4>;		// colors of tops of progress bars
	argList["progressBarSize"] = (void*) new Vector2;						// size of progress bars in pixels
	argList["progressions"] = (void*) new std::vector<float>;				// progress bar completion ratios
	argList["softEdge"] = (void*) new float;								// thickness of antialiasing of border in pixels
	argList["textureNames"] = (void*) new std::vector<std::string>;			// texture names
	argList["textureSizes"] = (void*) new std::vector<Vector2>;				// manually-specified width/height of gauge textures in screen dimensions (optional)
	return argList;
}

void DrawGaugePanel::deleteArgList(DrawStackArgList argList) {
	if(argList.find("backgroundColorBottom") != argList.end()) delete (Vector4*) argList["backgroundColorBottom"];
	if(argList.find("backgroundColorTop") != argList.end()) delete (Vector4*) argList["backgroundColorTop"];
	if(argList.find("border") != argList.end()) delete (float*) argList["border"];
	if(argList.find("borderColor") != argList.end()) delete (Vector4*) argList["borderColor"];
	if(argList.find("elements") != argList.end()) delete (size_t*) argList["elements"];
	if(argList.find("fontColor") != argList.end()) delete (Vector4*) argList["fontColor"];
	if(argList.find("fontSize") != argList.end()) delete (float*) argList["fontSize"];
	if(argList.find("insideColor") != argList.end()) delete (Vector4*) argList["insideColor"];
	if(argList.find("metrics") != argList.end()) delete (UIMetrics*) argList["metrics"];
	if(argList.find("outsideColor") != argList.end()) delete (Vector4*) argList["outsideColor"];
	if(argList.find("padding") != argList.end()) delete (float*) argList["padding"];
	if(argList.find("progressBarColorsBottom") != argList.end()) delete (std::vector<Vector4>*) argList["progressBarColorsBottom"];
	if(argList.find("progressBarColorsTop") != argList.end()) delete (std::vector<Vector4>*) argList["progressBarColorsTop"];
	if(argList.find("progressBarSize") != argList.end()) delete (Vector2*) argList["progressBarSize"];
	if(argList.find("progressions") != argList.end()) delete (std::vector<float>*) argList["progressions"];
	if(argList.find("softEdge") != argList.end()) delete (float*) argList["softEdge"];
	if(argList.find("textureNames") != argList.end()) delete (std::vector<std::string>*) argList["textureNames"];
	if(argList.find("textureSizes") != argList.end()) delete (std::vector<Vector2>*) argList["textureSizes"];
}

Vector2 DrawGaugePanel::getSize(DrawStackArgList argList) {
	// determine size
	std::map<std::string, void*> testArguments;

	Vector2 maxTextureSize(0.0f, 0.0f);
	for(size_t i = 0; i < *((size_t*) argList["elements"]); ++i) {
		Vector2 textureSize;

		if(argList.find("textureSizes") != argList.end()) {
			textureSize = ((std::vector<Vector2>*) argList["textureSizes"])->at(i);
		} else {
			std::string texture = ((std::vector<std::string>*) argList["textureNames"])->at(i);
			testArguments["texture"] = (void*) &texture;

			textureSize = textureDrawer->getSize(testArguments);
		}

		if(textureSize.x > maxTextureSize.x) maxTextureSize.x = textureSize.x;
		if(textureSize.y > maxTextureSize.y) maxTextureSize.y = textureSize.y;
	}

	float maxProgressBarHeight = ((Vector2*) argList["progressBarSize"])->y / (float) gameGraphics->resolutionY * 2.0f;
	if(maxProgressBarHeight > maxTextureSize.y) maxTextureSize.y = maxProgressBarHeight;

	testArguments.clear();
	testArguments["size"] = argList["progressBarSize"];

	Vector2 progressBarSize(	// pixels in, screen dimensions out
			((Vector2*) argList["progressBarSize"])->x / (float) gameGraphics->resolutionX * 2.0f,
			((Vector2*) argList["progressBarSize"])->y / (float) gameGraphics->resolutionY * 2.0f
		);

	Vector2 padding(
		*((float*) argList["padding"]) / (float) gameGraphics->resolutionX * 2.0f,
		*((float*) argList["padding"]) / (float) gameGraphics->resolutionY * 2.0f
	);

	return Vector2(
			padding.x * 3.0f + maxTextureSize.x + progressBarSize.x,
			padding.y + ((maxTextureSize.y > progressBarSize.y ? maxTextureSize.y : progressBarSize.y) +
					padding.y) * (float) (*((size_t*) argList["elements"]))
		);
}

void DrawGaugePanel::execute(DrawStackArgList argList) {
	// calculate the size again rather than calling getSize() since we need data from in-between steps
	UIMetrics metrics = *((UIMetrics*) argList["metrics"]);
	std::map<std::string, void*> testArguments;

	Vector2 maxTextureSize(0.0f, 0.0f);
	for(size_t i = 0; i < *((size_t*) argList["elements"]); ++i) {
		Vector2 textureSize;

		if(argList.find("textureSizes") != argList.end()) {
			textureSize = ((std::vector<Vector2>*) argList["textureSizes"])->at(i);
		} else {
			std::string texture = ((std::vector<std::string>*) argList["textureNames"])->at(i);
			testArguments["texture"] = (void*) &texture;

			textureSize = textureDrawer->getSize(testArguments);
		}

		if(textureSize.x > maxTextureSize.x) maxTextureSize.x = textureSize.x;
		if(textureSize.y > maxTextureSize.y) maxTextureSize.y = textureSize.y;
	}

	float maxProgressBarHeight = ((Vector2*) argList["progressBarSize"])->y / (float) gameGraphics->resolutionY * 2.0f;
	if(maxProgressBarHeight > maxTextureSize.y) maxTextureSize.y = maxProgressBarHeight;

	testArguments.clear();
	testArguments["size"] = argList["progressBarSize"];

	Vector2 progressBarSize(
			((Vector2*) argList["progressBarSize"])->x / (float) gameGraphics->resolutionX * 2.0f,
			((Vector2*) argList["progressBarSize"])->y / (float) gameGraphics->resolutionY * 2.0f
		);

	Vector2 padding(
		*((float*) argList["padding"]) / (float) gameGraphics->resolutionX * 2.0f,
		*((float*) argList["padding"]) / (float) gameGraphics->resolutionY * 2.0f
	);

	Vector2 size(
			padding.x * 3.0f + maxTextureSize.x + progressBarSize.x,
			padding.y + ((maxTextureSize.y > progressBarSize.y ? maxTextureSize.y : progressBarSize.y) +
					padding.y) * (float) (*((size_t*) argList["elements"]))
		);
	Vector2 position = metrics.position;

	// draw the container
	std::map<std::string, void*> containerArgs = argList;
	containerArgs["size"] = new Vector2; *((Vector2*) containerArgs["size"]) = size;

	containerDrawer->execute(containerArgs);

	delete (Vector2*) containerArgs["size"];

	// draw the textures
	for(size_t i = 0; i < *((size_t*) argList["elements"]); ++i) {
		UIMetrics textureMetrics;
		std::string texture = (*((std::vector<std::string>*) argList["textureNames"]))[i];
		Vector2 textureSize;
		if(argList.find("textureSizes") != argList.end())
			textureSize = ((std::vector<Vector2>*) argList["textureSizes"])->at(i);

		std::map<std::string, void*> textureArguments;
		textureArguments["metrics"] = (void*) &textureMetrics;
		textureArguments["texture"] = (void*) &texture;
		if(argList.find("textureSizes") != argList.end())
			textureArguments["size"] = (void*) &textureSize;

		textureMetrics.position = Vector2(
				position.x - size.x / 2.0f + padding.x + maxTextureSize.x / 2.0f,
				position.y + size.y / 2.0f - padding.y - maxTextureSize.y / 2.0f - (maxTextureSize.y + padding.y) * (float) i
			);

		textureDrawer->execute(textureArguments);
	}

	// draw the progress bars
	for(size_t i = 0; i < *((size_t*) argList["elements"]); ++i) {
		UIMetrics progressBarMetrics;
		Vector4 color1Top = (*((std::vector<Vector4>*) argList["progressBarColorsTop"]))[i];
		Vector4 color1Bottom = (*((std::vector<Vector4>*) argList["progressBarColorsBottom"]))[i];
		Vector4 color2Top = *((Vector4*) argList["backgroundColorTop"]);
		Vector4 color2Bottom = *((Vector4*) argList["backgroundColorBottom"]);
		float progression = (*((std::vector<float>*) argList["progressions"]))[i];

		std::map<std::string, void*> progressBarArguments;
		progressBarArguments["metrics"] = (void*) &progressBarMetrics;
		progressBarArguments["color1Top"] = (void*) &color1Top;
		progressBarArguments["color1Bottom"] = (void*) &color1Bottom;
		progressBarArguments["color2Top"] = (void*) &color2Top;
		progressBarArguments["color2Bottom"] = (void*) &color2Bottom;
		progressBarArguments["progression"] = (void*) &progression;
		progressBarArguments["size"] = (void*) &progressBarSize;

		Vector2 actualProgressBarSize = progressBarDrawer->getSize(progressBarArguments);

		progressBarMetrics.position = Vector2(
				position.x - size.x / 2.0f + padding.x + maxTextureSize.x + padding.x + actualProgressBarSize.x / 2.0f,
				position.y + size.y / 2.0f - padding.y - maxTextureSize.y / 2.0f - (maxTextureSize.y + padding.y) * (float) i
			);

		progressBarDrawer->execute(progressBarArguments);
	}
}
