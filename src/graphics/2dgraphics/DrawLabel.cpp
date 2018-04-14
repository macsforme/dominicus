// DrawLabel.cpp
// Dominicus

#include "graphics/2dgraphics/DrawLabel.h"

#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

#include "core/GameSystem.h"
#include "graphics/GameGraphics.h"
#include "graphics/text/TextBlock.h"
#include "graphics/UILayoutAuthority.h"
#include "platform/OpenGLHeaders.h"

extern GameGraphics* gameGraphics;
extern GameSystem* gameSystem;

DrawLabel::DrawLabel() {
	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));
}

DrawLabel::~DrawLabel() {
	// undo vertex buffer setup
	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

DrawStackArgList DrawLabel::instantiateArgList() {
	DrawStackArgList argList;

	argList["fontColor"] = (void*) new Vector4;		// font base color
	argList["fontSize"] = (void*) new float;		// font point size for this label
	argList["metrics"] = (void*) new UIMetrics;		// UI element metrics
	argList["text"] = (void*) new std::string;		// text to draw
	argList["wrap"] = (void*) new float;			// wrap width for text in pixels (optional)

	return argList;
}

void DrawLabel::deleteArgList(DrawStackArgList argList) {
	if(argList.find("fontColor") != argList.end()) delete (Vector4*) argList["fontColor"];
	if(argList.find("fontSize") != argList.end()) delete (float*) argList["fontSize"];
	if(argList.find("metrics") != argList.end()) delete (UIMetrics*) argList["metrics"];
	if(argList.find("text") != argList.end()) delete (std::string*) argList["text"];
	if(argList.find("wrap") != argList.end()) delete (float*) argList["wrap"];
}

Vector2 DrawLabel::getSize(DrawStackArgList argList) {
	// collect arguments
	float* fontSize = ((float*) argList["fontSize"]);
	std::string* text = ((std::string*) argList["text"]);

	// create the text block
	unsigned int widthWrap = gameGraphics->resolutionX;
	if(argList.find("wrap") != argList.end())
		widthWrap = (unsigned int) (*((float*) argList["wrap"]) / 2.0f * (float) widthWrap);
	if(widthWrap == 0)
		widthWrap = gameGraphics->resolutionX;
	TextBlock textBlock(
			text->c_str(),
			widthWrap,
			gameGraphics->resolutionY,
			gameGraphics->fontManager,
			(unsigned int) *fontSize
		);

	// determine if there will be multiple blocks due to tabs
	bool areTabs = false;
	for(size_t i = 0; i < strlen(text->c_str()); ++i) {
		if(*(text->c_str() + i) == '\t') {
			areTabs = true;

			break;
		}
	}

	if(areTabs) {
		// amend the textBlock to have separate block data for each tab section
		// (this will also render wrapping pretty useless)
		size_t numSections = 1;

		size_t thisLineTabs = 0;
		for(size_t i = 0; i < strlen(text->c_str()); ++i) {
			if(*(text->c_str() + i) == '\n') {
				thisLineTabs = 0;
			} else if(*(text->c_str() + i) == '\t') {
				++thisLineTabs;

				if(thisLineTabs + 1 > numSections)
					numSections = thisLineTabs + 1;
			}
		}

		std::string* sections = new std::string[numSections];

		size_t currentSection = 0;

		for(size_t i = 0; i < strlen(text->c_str()); ++i) {
			if(*(text->c_str() + i) == '\n') {
				for(size_t p = 0; p < numSections - currentSection; ++p)
					sections[currentSection + p] += "\n";

				currentSection = 0;
			} else if(*(text->c_str() + i) == '\t') {
				sections[currentSection] += "\n";

				++currentSection;
			} else {
				sections[currentSection] += *(text->c_str() + i);
			}
		}

		for(size_t i = 0; i < numSections; ++i)
			while(sections[i].substr(sections[i].size() - 1, std::string::npos) == "\n")
				sections[i] = sections[i].substr(0, sections[i].size() - 1);

		TextBlock* textBlocks[numSections];

		for(size_t i = 0; i < numSections; ++i)
			textBlocks[i] = new TextBlock(
					sections[i].c_str(),
					(unsigned int) gameGraphics->resolutionX,
					(unsigned int) gameGraphics->resolutionY,
					gameGraphics->fontManager,
					(unsigned int) *fontSize
				);

		delete[] sections;

		size_t totalWidth = 0, totalHeight = 0, maxOriginY = 0;

		for(size_t i = 0; i < numSections; ++i) {
			if(i != 0)
				totalWidth += gameGraphics->fontManager->fontData[' '][(unsigned int) *fontSize].advanceX * 8;

			totalWidth += textBlocks[i]->width;

			if(textBlocks[i]->height > (int) totalHeight)
				totalHeight = textBlocks[i]->height;

			if(textBlocks[i]->originY > maxOriginY)
				maxOriginY = textBlocks[i]->originY;
		}

		textBlock.width = totalWidth;
		textBlock.height = totalHeight;

		// skip part that actually sets text positions

		for(size_t i = 0; i < numSections; ++i)
			delete textBlocks[i];
	}

	return Vector2(
			(float) textBlock.width / (float) gameGraphics->resolutionX * 2.0f,
			(float) textBlock.height / (float) gameGraphics->resolutionY * 2.0f
		);
}

void DrawLabel::execute(DrawStackArgList argList) {
	// collect arguments
	UIMetrics* metrics = ((UIMetrics*) argList["metrics"]);
	float* fontSize = ((float*) argList["fontSize"]);
	Vector4* fontColor = ((Vector4*) argList["fontColor"]);
	std::string* text = ((std::string*) argList["text"]);

	// verify font size is valid
	if(gameGraphics->fontManager->textureIDs.find((unsigned int) *fontSize) == gameGraphics->fontManager->textureIDs.end()) {
		std::stringstream sstream;
		sstream <<
				"Font size " <<
				(unsigned int) *fontSize <<
				" requested for drawing but is not in memory.";
		gameSystem->log(GameSystem::LOG_FATAL, sstream.str().c_str());
	}

	// create the text block
	unsigned int widthWrap = gameGraphics->resolutionX;
	if(argList.find("wrap") != argList.end())
		widthWrap = (unsigned int) (*((float*) argList["wrap"]) / 2.0f * (float) widthWrap);
	if(widthWrap == 0)
		widthWrap = gameGraphics->resolutionX;

	TextBlock textBlock(
			text->c_str(),
			widthWrap,
			gameGraphics->resolutionY,
			gameGraphics->fontManager,
			(unsigned int) *fontSize
		);

	// determine if there will be multiple blocks due to tabs
	bool areTabs = false;
	for(size_t i = 0; i < strlen(text->c_str()); ++i) {
		if(*(text->c_str() + i) == '\t') {
			areTabs = true;

			break;
		}
	}

	if(areTabs) {
		// amend the textBlock to have separate block data for each tab section
		// (this will also render wrapping pretty useless)
		size_t numSections = 1;

		size_t thisLineTabs = 0;
		for(size_t i = 0; i < strlen(text->c_str()); ++i) {
			if(*(text->c_str() + i) == '\n') {
				thisLineTabs = 0;
			} else if(*(text->c_str() + i) == '\t') {
				++thisLineTabs;

				if(thisLineTabs + 1 > numSections)
					numSections = thisLineTabs + 1;
			}
		}

		std::string* sections = new std::string[numSections];

		size_t currentSection = 0;

		for(size_t i = 0; i < strlen(text->c_str()); ++i) {
			if(*(text->c_str() + i) == '\n') {
				for(size_t p = 0; p < numSections - currentSection; ++p)
					sections[currentSection + p] += "\n";

				currentSection = 0;
			} else if(*(text->c_str() + i) == '\t') {
				sections[currentSection] += "\n";

				++currentSection;
			} else {
				sections[currentSection] += *(text->c_str() + i);
			}
		}

		for(size_t i = 0; i < numSections; ++i)
			while(sections[i].substr(sections[i].size() - 1, std::string::npos) == "\n")
				sections[i] = sections[i].substr(0, sections[i].size() - 1);

		TextBlock* textBlocks[numSections];

		for(size_t i = 0; i < numSections; ++i) {
			textBlocks[i] = new TextBlock(
					sections[i].c_str(),
					(unsigned int) gameGraphics->resolutionX,
					(unsigned int) gameGraphics->resolutionY,
					gameGraphics->fontManager,
					(unsigned int) *fontSize
				);
		}

		delete[] sections;

		size_t totalWidth = 0, totalHeight = 0, maxOriginY = 0;

		for(size_t i = 0; i < numSections; ++i) {
			if(i != 0)
				totalWidth += gameGraphics->fontManager->fontData[' '][(unsigned int) *fontSize].advanceX * 8;

			totalWidth += textBlocks[i]->width;

			if(textBlocks[i]->height /*- textBlocks[i]->originY*/ > (int) totalHeight)
				totalHeight = textBlocks[i]->height /*- textBlocks[i]->originY*/;

			if(textBlocks[i]->originY > maxOriginY)
				maxOriginY = textBlocks[i]->originY;
		}

		textBlock.entries.clear();
		textBlock.width = totalWidth;
		textBlock.height = totalHeight;

		size_t penX = 0, penY = 0;

		for(size_t i = 0; i < numSections; ++i) {
			penY = totalHeight - textBlocks[i]->height;

			for(size_t p = 0; p < textBlocks[i]->entries.size(); ++p) {
				TextBlock::CharEntry thisEntry = textBlocks[i]->entries[p];

				thisEntry.sX = (thisEntry.sX * 0.5f + 0.5f) * (float) textBlocks[i]->width;
				thisEntry.sX += (float) penX;
				thisEntry.sX = thisEntry.sX / (float) totalWidth * 2.0f - 1.0f;

				thisEntry.eX = (thisEntry.eX * 0.5f + 0.5f) * (float) textBlocks[i]->width;
				thisEntry.eX += (float) penX;
				thisEntry.eX = thisEntry.eX / (float) totalWidth * 2.0f - 1.0f;

				thisEntry.sY = (thisEntry.sY * 0.5f + 0.5f) * (float) textBlocks[i]->height;
				thisEntry.sY += (float) penY;
				thisEntry.sY = thisEntry.sY / (float) totalHeight * 2.0f - 1.0f;

				thisEntry.eY = (thisEntry.eY * 0.5f + 0.5f) * (float) textBlocks[i]->height;
				thisEntry.eY += (float) penY;
				thisEntry.eY = thisEntry.eY / (float) totalHeight * 2.0f - 1.0f;

				textBlock.entries.push_back(thisEntry);
			}

			penX +=
					textBlocks[i]->width +
					gameGraphics->fontManager->fontData[' '][(unsigned int) *fontSize].advanceX * 8;
		}

		for(size_t i = 0; i < numSections; ++i)
			delete textBlocks[i];
	}

	// get the actual size so possibly incorrect metrics don't skew the text
	Vector2 actualSize(
			(float) textBlock.width / (float) gameGraphics->resolutionX * 2.0f,
			(float) textBlock.height / (float) gameGraphics->resolutionY * 2.0f
		);

	// update vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	std::vector<VertexEntry> quadVertices;

	for(
			std::vector<TextBlock::CharEntry>::iterator itr = textBlock.entries.begin();
			itr != textBlock.entries.end();
			++itr
		) {
		VertexEntry entry;

		entry.position.x = itr->sX * actualSize.x / 2.0f;
		entry.position.y = itr->sY * actualSize.y / 2.0f;
		entry.texCoord.x = gameGraphics->fontManager->fontData[itr->character][*fontSize].sX;
		entry.texCoord.y = gameGraphics->fontManager->fontData[itr->character][*fontSize].sY;
		entry.color = itr->color;
		quadVertices.push_back(entry);

		entry.position.x = itr->sX * actualSize.x / 2.0f;
		entry.position.y = itr->eY * actualSize.y / 2.0f;
		entry.texCoord.x = gameGraphics->fontManager->fontData[itr->character][*fontSize].sX;
		entry.texCoord.y = gameGraphics->fontManager->fontData[itr->character][*fontSize].eY;
		entry.color = itr->color;
		quadVertices.push_back(entry);

		entry.position.x = itr->eX * actualSize.x / 2.0f;
		entry.position.y = itr->eY * actualSize.y / 2.0f;
		entry.texCoord.x = gameGraphics->fontManager->fontData[itr->character][*fontSize].eX;
		entry.texCoord.y = gameGraphics->fontManager->fontData[itr->character][*fontSize].eY;
		entry.color = itr->color;
		quadVertices.push_back(entry);

		entry.position.x = itr->eX * actualSize.x / 2.0f;
		entry.position.y = itr->sY * actualSize.y / 2.0f;
		entry.texCoord.x = gameGraphics->fontManager->fontData[itr->character][*fontSize].eX;
		entry.texCoord.y = gameGraphics->fontManager->fontData[itr->character][*fontSize].sY;
		entry.color = itr->color;
		quadVertices.push_back(entry);
	}

	GLuint* elementBufferArray = new GLuint[quadVertices.size()];
	for(size_t i = 0; i < quadVertices.size(); ++i)
		elementBufferArray[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadVertices.size() * sizeof(GLuint), elementBufferArray,
			GL_STREAM_DRAW);

	delete[] elementBufferArray;

	size_t vertexBufferArraySize = quadVertices.size() * 9;
	GLfloat* vertexBufferArray = new GLfloat[vertexBufferArraySize];

	for(size_t i = 0; i < quadVertices.size(); ++i) {
		vertexBufferArray[i * 9 + 0] = quadVertices[i].position.x + metrics->position.x;
		vertexBufferArray[i * 9 + 1] = quadVertices[i].position.y + metrics->position.y;
		vertexBufferArray[i * 9 + 2] = 0.0f;
		vertexBufferArray[i * 9 + 3] = quadVertices[i].texCoord.x;
		vertexBufferArray[i * 9 + 4] = quadVertices[i].texCoord.y;

		if(quadVertices[i].color == Vector4(0.0f, 0.0f, 0.0f, 0.0f)) { // this could be a legitimate color, so... uh... oh well
			vertexBufferArray[i * 9 + 5] = fontColor->x;
			vertexBufferArray[i * 9 + 6] = fontColor->y;
			vertexBufferArray[i * 9 + 7] = fontColor->z;
			vertexBufferArray[i * 9 + 8] = fontColor->w /* * timeFactor*/;
		} else {
			vertexBufferArray[i * 9 + 5] = quadVertices[i].color.x;
			vertexBufferArray[i * 9 + 6] = quadVertices[i].color.y;
			vertexBufferArray[i * 9 + 7] = quadVertices[i].color.z;
			vertexBufferArray[i * 9 + 8] = quadVertices[i].color.w /* * timeFactor*/;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, vertexBufferArraySize * sizeof(GLfloat), vertexBufferArray,
			GL_STREAM_DRAW);

	delete[] vertexBufferArray;

	// state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	if(gameGraphics->supportsMultisampling) glDisable(GL_MULTISAMPLE);
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);

	// enable shader
	glUseProgram(gameGraphics->getProgramID("colorTexture"));

	// set uniforms
	glUniformMatrix4fv(glGetUniformLocation(gameGraphics->getProgramID("colorTexture"), "mvpMatrix"), 1, GL_FALSE, gameGraphics->idMatrixArray);
	glUniform1i(glGetUniformLocation(gameGraphics->getProgramID("colorTexture"), "texture"), 0);

	// activate the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameGraphics->fontManager->textureIDs[(unsigned int) *fontSize]);

	// texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "position"), 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "texCoord"), 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "color"), 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*) (5 * sizeof(GLfloat)));

	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "position"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "texCoord"));
	glEnableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "color"));

	glDrawElements(GL_QUADS, quadVertices.size(), GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "position"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "texCoord"));
	glDisableVertexAttribArray(glGetAttribLocation(gameGraphics->getProgramID("colorTexture"), "color"));
}
