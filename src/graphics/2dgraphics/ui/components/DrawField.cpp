// DrawField.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/2dgraphics/ui/components/DrawField.h"

DrawField::DrawField() : labelDrawer(new DrawLabel()) {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["useTexture"] = glGetUniformLocation(shaderProgram, "useTexture");
	uniforms["useColor"] = glGetUniformLocation(shaderProgram, "useColor");
	uniforms["useLighting"] = glGetUniformLocation(shaderProgram, "useLighting");

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
	// memory management
	delete(labelDrawer);

	// delete buffers
	if(glIsBuffer(vertexBuffers["vertices"]))
		glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	if(glIsBuffer(vertexBuffers["elements"]))
		glDeleteBuffers(1, &(vertexBuffers["elements"]));

	// delete shader program
	if(glIsProgram(shaderProgram))
		glDeleteProgram(shaderProgram);
}

Vector2 DrawField::getSize(std::map<std::string, void*> arguments) {
	// collect arguments
	float* fontSize = ((float*) arguments["fontSize"]);
	std::string* text = ((std::string*) arguments["text"]);

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

	return(Vector2(
			(float) textBlock.width / (float) gameGraphics->resolutionX * 2.0f,
			gameGraphics->fontManager->lineHeights[(unsigned int) *fontSize] /
					(float) gameGraphics->resolutionY * 2.0f
		));
}

void DrawField::execute(std::map<std::string, void*> arguments) {
	// collect arguments
	UIMetrics* metrics = ((UIMetrics*) arguments["metrics"]);
	Vector4* boxColor = ((Vector4*) arguments["boxColor"]);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLfloat vertexBufferArray[] = {
			metrics->position.x - metrics->size.x / 2.0f,
			metrics->position.y - metrics->size.y / 2.0f,
			0.0f,
			boxColor->x,
			boxColor->y,
			boxColor->z,
			boxColor->w,

			metrics->position.x - metrics->size.x / 2.0f,
			metrics->position.y + metrics->size.y / 2.0f,
			0.0f,
			boxColor->x,
			boxColor->y,
			boxColor->z,
			boxColor->w,

			metrics->position.x + metrics->size.x / 2.0f,
			metrics->position.y + metrics->size.y / 2.0f,
			0.0f,
			boxColor->x,
			boxColor->y,
			boxColor->z,
			boxColor->w,

			metrics->position.x + metrics->size.x / 2.0f,
			metrics->position.y - metrics->size.y / 2.0f,
			0.0f,
			boxColor->x,
			boxColor->y,
			boxColor->z,
			boxColor->w
		};

	glBufferData(GL_ARRAY_BUFFER, 28 * sizeof(GL_FLOAT), vertexBufferArray, GL_STREAM_DRAW);

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, gameGraphics->idMatrixArray);
	glUniform1i(uniforms["useTexture"], 0);
	glUniform1i(uniforms["useColor"], 1);
	glUniform1i(uniforms["useLighting"], 0);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT),
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
			(unsigned int) *((float*) arguments["fontSize"])
		);
	float spaceWidth = (float) textBlock->width / (float) gameGraphics->resolutionX * 2.0f;

	std::string labelString = *((std::string*) arguments["text"]);

	delete(textBlock);
	textBlock = new TextBlock(
			labelString.c_str(),
			gameGraphics->resolutionX,
			gameGraphics->resolutionY,
			gameGraphics->fontManager,
			(unsigned int) *((float*) arguments["fontSize"])
		);

	while((float) textBlock->width / (float) gameGraphics->resolutionX * 2.0f +
			spaceWidth * 2.0f > metrics->size.x) {
		labelString = labelString.substr(1);

		delete(textBlock);
		textBlock = new TextBlock(
				labelString.c_str(),
				gameGraphics->resolutionX,
				gameGraphics->resolutionY,
				gameGraphics->fontManager,
				(unsigned int) *((float*) arguments["fontSize"])
			);
	}

	// clipped text
	std::map<std::string, void*> labelArguments = arguments;

	labelArguments["text"] = (void*) new std::string;
	labelArguments["metrics"] = (void*) new UIMetrics;

	*((std::string*) labelArguments["text"]) = labelString;

	*((UIMetrics*) labelArguments["metrics"]) = *((UIMetrics*) arguments["metrics"]);
	((UIMetrics*) labelArguments["metrics"])->size = labelDrawer->getSize(labelArguments);
	((UIMetrics*) labelArguments["metrics"])->position.x =
			((UIMetrics*) arguments["metrics"])->position.x -
			((UIMetrics*) arguments["metrics"])->size.x / 2.0f +
			spaceWidth +
			(float) textBlock->width / (float) gameGraphics->resolutionX;
	((UIMetrics*) labelArguments["metrics"])->position.y =
			((UIMetrics*) arguments["metrics"])->position.y +
			((UIMetrics*) arguments["metrics"])->size.y / 2.0f -
			((UIMetrics*) arguments["metrics"])->size.y * 2.0f / 3.0f +
			((float) textBlock->originY / (float) textBlock->height - 0.5f) *
			(float) textBlock->height / gameGraphics->resolutionY * 2.0f;

	labelDrawer->execute(labelArguments);

	delete((std::string*) labelArguments["text"]);
	delete((UIMetrics*) labelArguments["metrics"]);

}
