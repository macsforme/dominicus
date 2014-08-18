// DrawField.cpp
// Crucible Island

#include "graphics/2dgraphics/DrawField.h"

#include <vector>

#include "graphics/GameGraphics.h"
#include "graphics/text/TextBlock.h"
#include "graphics/UILayoutAuthority.h"
#include "platform/OpenGLHeaders.h"

extern GameGraphics* gameGraphics;

DrawField::DrawField(DrawLabel* newLabelDrawer) : labelDrawer(newLabelDrawer) {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "color"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "color"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLushort elementBufferArray[] = { 0, 1, 2, 3 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferArray), elementBufferArray,
			GL_STATIC_DRAW);
}

DrawField::~DrawField() {
	// undo shader setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));

	if(! glIsShader(shaderProgram)) // sometimes duplicate shaders get optimized out so check for validity
		return;

	GLsizei shaderCount;
	GLuint* shaders = new GLuint[2];
	glGetAttachedShaders(shaderProgram, 2, &shaderCount, shaders);

	for(size_t i = 0; i < shaderCount; ++i) {
		glDetachShader(shaderProgram, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	delete[] shaders;

	glDeleteProgram(shaderProgram);
}

DrawStackArgList DrawField::instantiateArgList() {
	DrawStackArgList argList;

	argList["boxColor"] = (void*) new Vector4;		// background color of field
	argList["fontColor"] = (void*) new Vector4;		// font base color
	argList["fontSize"] = (void*) new float;		// font point size for this label
	argList["metrics"] = (void*) new UIMetrics;		// UI element metrics
	argList["size"] = (void*) new Vector2;			// width/height of field in screen dimensions
	argList["text"] = (void*) new std::string;		// text to draw

	return argList;
}

void DrawField::deleteArgList(DrawStackArgList argList) {
	if(argList.find("boxColor") != argList.end()) delete (Vector4*) argList["boxColor"];
	if(argList.find("fontColor") != argList.end()) delete (Vector4*) argList["fontColor"];
	if(argList.find("fontSize") != argList.end()) delete (float*) argList["fontSize"];
	if(argList.find("metrics") != argList.end()) delete (UIMetrics*) argList["metrics"];
	if(argList.find("size") != argList.end()) delete (Vector2*) argList["size"];
	if(argList.find("text") != argList.end()) delete (std::string*) argList["text"];
}

Vector2 DrawField::getSize(DrawStackArgList argList) {
	// collect arguments
	float* fontSize = ((float*) argList["fontSize"]);
	std::string* text = ((std::string*) argList["text"]);

	// create the text block
	std::string paddedText = " " + *text + " ";
	unsigned int widthWrap = gameGraphics->resolutionX;

	TextBlock textBlock(
			paddedText.c_str(),
			widthWrap,
			gameGraphics->resolutionY,
			gameGraphics->fontManager,
			(unsigned int) *fontSize
		);

	return Vector2(
			(float) textBlock.width / (float) gameGraphics->resolutionX * 2.0f,
			gameGraphics->fontManager->lineHeights[(unsigned int) *fontSize] /
					(float) gameGraphics->resolutionY * 2.0f
		);
}

void DrawField::execute(DrawStackArgList argList) {
	// collect arguments
	UIMetrics* metrics = ((UIMetrics*) argList["metrics"]);
	Vector2 size = *((Vector2*) argList["size"]);
	Vector4* boxColor = ((Vector4*) argList["boxColor"]);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertexBufferArray[] = {
			metrics->position.x - size.x / 2.0f,
			metrics->position.y - size.y / 2.0f,
			0.0f,
			boxColor->x,
			boxColor->y,
			boxColor->z,
			boxColor->w,

			metrics->position.x - size.x / 2.0f,
			metrics->position.y + size.y / 2.0f,
			0.0f,
			boxColor->x,
			boxColor->y,
			boxColor->z,
			boxColor->w,

			metrics->position.x + size.x / 2.0f,
			metrics->position.y + size.y / 2.0f,
			0.0f,
			boxColor->x,
			boxColor->y,
			boxColor->z,
			boxColor->w,

			metrics->position.x + size.x / 2.0f,
			metrics->position.y - size.y / 2.0f,
			0.0f,
			boxColor->x,
			boxColor->y,
			boxColor->z,
			boxColor->w
		};

	glBufferData(GL_ARRAY_BUFFER, 28 * sizeof(GLfloat), vertexBufferArray, GL_STREAM_DRAW);

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, gameGraphics->idMatrixArray);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) 0);
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
			(GLvoid*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["color"]);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_BLEND);

	// cap the line length so it doesn't go outside the field
	TextBlock* textBlock = new TextBlock(
			" ",
			gameGraphics->resolutionX,
			gameGraphics->resolutionY,
			gameGraphics->fontManager,
			(unsigned int) *((float*) argList["fontSize"])
		);
	float spaceWidth = (float) textBlock->width / (float) gameGraphics->resolutionX * 2.0f;

	std::string labelString = *((std::string*) argList["text"]);

	delete textBlock;
	textBlock = new TextBlock(
			labelString.c_str(),
			gameGraphics->resolutionX,
			gameGraphics->resolutionY,
			gameGraphics->fontManager,
			(unsigned int) *((float*) argList["fontSize"])
		);

	while((float) textBlock->width / (float) gameGraphics->resolutionX * 2.0f +
			spaceWidth * 2.0f > size.x) {
		labelString = labelString.substr(1);

		delete textBlock;
		textBlock = new TextBlock(
				labelString.c_str(),
				gameGraphics->resolutionX,
				gameGraphics->resolutionY,
				gameGraphics->fontManager,
				(unsigned int) *((float*) argList["fontSize"])
			);
	}

	// clipped text
	std::map<std::string, void*> labelArguments = argList;

	labelArguments["text"] = (void*) new std::string;
	labelArguments["metrics"] = (void*) new UIMetrics;

	*((std::string*) labelArguments["text"]) = labelString;

	*((UIMetrics*) labelArguments["metrics"]) = *((UIMetrics*) argList["metrics"]);
	((UIMetrics*) labelArguments["metrics"])->size = labelDrawer->getSize(labelArguments);
	((UIMetrics*) labelArguments["metrics"])->position.x =
			((UIMetrics*) argList["metrics"])->position.x -
			((UIMetrics*) argList["metrics"])->size.x / 2.0f +
			spaceWidth +
			(float) textBlock->width / (float) gameGraphics->resolutionX;
	((UIMetrics*) labelArguments["metrics"])->position.y =
			((UIMetrics*) argList["metrics"])->position.y;

	labelDrawer->execute(labelArguments);

	delete (std::string*) labelArguments["text"];
	delete (UIMetrics*) labelArguments["metrics"];

}
