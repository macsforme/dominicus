// DrawButton.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawGaugePanel.h"

DrawGaugePanel::DrawGaugePanel() :
		containerDrawer(new DrawContainer()), progressBarDrawer(new DrawProgressBar()), textureDrawer(new DrawTexture()) { }

DrawGaugePanel::~DrawGaugePanel() {
	// memory management
	delete containerDrawer;
	delete progressBarDrawer;
	delete textureDrawer;
}

Vector2 DrawGaugePanel::getSize(std::map<std::string, void*> arguments) {
	// determine size
	std::map<std::string, void*> testArguments;

	Vector2 maxTextureSize(0.0f, 0.0f);
	for(size_t i = 0; i < *((size_t*) arguments["elements"]); ++i) {
		std::string texture = (*((std::vector<std::string>*) arguments["textureNames"]))[i];
		testArguments["texture"] = (void*) &texture;

		Vector2 textureSize = textureDrawer->getSize(testArguments);

		if(textureSize.x > maxTextureSize.x) maxTextureSize.x = textureSize.x;
		if(textureSize.y > maxTextureSize.y) maxTextureSize.y = textureSize.y;
	}

	float maxProgressBarHeight = ((Vector2*) arguments["progressBarSize"])->y / (float) gameGraphics->resolutionY * 2.0f;
	if(maxProgressBarHeight > maxTextureSize.y) maxTextureSize.y = maxProgressBarHeight;

	testArguments.clear();
	testArguments["size"] = arguments["progressBarSize"];

	Vector2 progressBarSize = progressBarDrawer->getSize(testArguments);

	Vector2 padding = Vector2(
		*((float*) arguments["padding"]) / (float) gameGraphics->resolutionX * 2.0f,
		*((float*) arguments["padding"]) / (float) gameGraphics->resolutionY * 2.0f
	);

	return Vector2(
			padding.x * 3.0f + maxTextureSize.x + progressBarSize.x,
			padding.y + ((maxTextureSize.y > progressBarSize.y ? maxTextureSize.y : progressBarSize.y) +
					padding.y) * (float) (*((size_t*) arguments["elements"]))
		);
}

void DrawGaugePanel::execute(std::map<std::string, void*> arguments) {
	// calculate the size again rather than calling getSize() since we need data from in-between steps
	UIMetrics metrics = *((UIMetrics*) arguments["metrics"]);
	std::map<std::string, void*> testArguments;

	Vector2 maxTextureSize(0.0f, 0.0f);
	for(size_t i = 0; i < *((size_t*) arguments["elements"]); ++i) {
		std::string texture = (*((std::vector<std::string>*) arguments["textureNames"]))[i];
		testArguments["texture"] = (void*) &texture;

		Vector2 textureSize = textureDrawer->getSize(testArguments);

		if(textureSize.x > maxTextureSize.x) maxTextureSize.x = textureSize.x;
		if(textureSize.y > maxTextureSize.y) maxTextureSize.y = textureSize.y;
	}

	float maxProgressBarHeight = ((Vector2*) arguments["progressBarSize"])->y / (float) gameGraphics->resolutionY * 2.0f;
	if(maxProgressBarHeight > maxTextureSize.y) maxTextureSize.y = maxProgressBarHeight;

	testArguments.clear();
	testArguments["size"] = arguments["progressBarSize"];

	Vector2 progressBarSize = progressBarDrawer->getSize(testArguments);

	Vector2 padding = Vector2(
		*((float*) arguments["padding"]) / (float) gameGraphics->resolutionX * 2.0f,
		*((float*) arguments["padding"]) / (float) gameGraphics->resolutionY * 2.0f
	);

	Vector2 size(
			padding.x * 3.0f + maxTextureSize.x + progressBarSize.x,
			padding.y + ((maxTextureSize.y > progressBarSize.y ? maxTextureSize.y : progressBarSize.y) +
					padding.y) * (float) (*((size_t*) arguments["elements"]))
		);
	Vector2 position = metrics.position;

	// draw the container
	containerDrawer->execute(arguments);

	// draw the textures
	for(size_t i = 0; i < *((size_t*) arguments["elements"]); ++i) {
		UIMetrics textureMetrics;
		std::string texture = (*((std::vector<std::string>*) arguments["textureNames"]))[i];

		std::map<std::string, void*> textureArguments;
		textureArguments["metrics"] = (void*) &textureMetrics;
		textureArguments["texture"] = (void*) &texture;

		textureMetrics.position = Vector2(
				position.x - size.x / 2.0f + padding.x + maxTextureSize.x / 2.0f,
				position.y + size.y / 2.0f - padding.y - maxTextureSize.y / 2.0f - (maxTextureSize.y + padding.y) * (float) i
			);

		textureDrawer->execute(textureArguments);
	}

	for(size_t i = 0; i < *((size_t*) arguments["elements"]); ++i) {
		UIMetrics progressBarMetrics;
		Vector4 color1Top = (*((std::vector<Vector4>*) arguments["progressBarColorsTop"]))[i];
		Vector4 color1Bottom = (*((std::vector<Vector4>*) arguments["progressBarColorsBottom"]))[i];
		Vector4 color2Top = *((Vector4*) arguments["backgroundColorTop"]);
		Vector4 color2Bottom = *((Vector4*) arguments["backgroundColorBottom"]);
		float progression = (*((std::vector<float>*) arguments["progressions"]))[i];
		Vector2 progressBarSize = *((Vector2*) arguments["progressBarSize"]);

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
