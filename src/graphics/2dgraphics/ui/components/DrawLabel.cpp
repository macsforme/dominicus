/*
 * DrawLabel.cpp
 * Dominicus
 *
 * Copyright 2010-2011, Joshua Bodine
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "graphics/2dgraphics/ui/components/DrawLabel.h"

DrawLabel::DrawLabel() {
	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "default"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvpMatrix"] = glGetUniformLocation(shaderProgram, "mvpMatrix");
	uniforms["useTexture"] = glGetUniformLocation(shaderProgram, "useTexture");
	uniforms["texture"] = glGetUniformLocation(shaderProgram, "texture");
	uniforms["useColor"] = glGetUniformLocation(shaderProgram, "useColor");
	uniforms["useLighting"] = glGetUniformLocation(shaderProgram, "useLighting");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["texCoord"] = glGetAttribLocation(shaderProgram, "texCoord");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glGenBuffers(1, &(vertexBuffers["elements"]));
}

DrawLabel::~DrawLabel() {
	// delete buffers
	if(glIsBuffer(vertexBuffers["vertices"]))
		glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	if(glIsBuffer(vertexBuffers["elements"]))
		glDeleteBuffers(1, &(vertexBuffers["elements"]));

	// delete shader program
	if(glIsProgram(shaderProgram))
		glDeleteProgram(shaderProgram);
}

Vector2 DrawLabel::getSize(std::map<std::string, void*> arguments) {
	// collect arguments
	float* fontSize = ((float*) arguments["fontSize"]);
	std::string* text = ((std::string*) arguments["text"]);

	// create the text block
	unsigned int widthWrap = gameGraphics->resolutionX;
	if(arguments.find("wrap") != arguments.end())
		widthWrap = (unsigned int) (*((float*) arguments["wrap"]) / 2.0f * (float) widthWrap);

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

		std::string sections[numSections];

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
//		totalHeight += maxOriginY;

		textBlock.width = totalWidth;
		textBlock.height = totalHeight;
/*
		textBlock.entries.clear();

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
*/
		for(size_t i = 0; i < numSections; ++i)
			delete(textBlocks[i]);
	}

	return(Vector2(
			(float) textBlock.width / (float) gameGraphics->resolutionX * 2.0f,
			(float) textBlock.height / (float) gameGraphics->resolutionY * 2.0f
		));
}

void DrawLabel::execute(std::map<std::string, void*> arguments) {
	// collect arguments
	UIMetrics* metrics = ((UIMetrics*) arguments["metrics"]);
	float* fontSize = ((float*) arguments["fontSize"]);
	Vector4* fontColor = ((Vector4*) arguments["fontColor"]);
	std::string* text = ((std::string*) arguments["text"]);

	// color considers timer
	float timeFactor = 1.0f;
	if((float*) arguments["textTimer"] != NULL)
		timeFactor = *((float*) arguments["textTimer"]);

	// create the text block
	unsigned int widthWrap = gameGraphics->resolutionX;
	if(arguments.find("wrap") != arguments.end())
		widthWrap = (unsigned int) (*((float*) arguments["wrap"]) / 2.0f * (float) widthWrap);

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

		std::string sections[numSections];

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

//printf("SECTIONS\n"); for(size_t i = 0; i < numSections; ++i) printf("%u \"%s\"\n", (unsigned int) i, sections[i].c_str());
		TextBlock* textBlocks[numSections];

		for(size_t i = 0; i < numSections; ++i) {
			textBlocks[i] = new TextBlock(
					sections[i].c_str(),
					(unsigned int) gameGraphics->resolutionX,
					(unsigned int) gameGraphics->resolutionY,
					gameGraphics->fontManager,
					(unsigned int) *fontSize
				);

//printf("textBlock size: %u %u\n", (unsigned int) textBlocks[i]->width, (unsigned int) textBlocks[i]->height);
		}
//printf("\n");

//textBlock = *(textBlocks[0]);

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
//		totalHeight += maxOriginY;

//printf("total dimensions: %u %u\n\n", (unsigned int) totalWidth, (unsigned int) totalHeight);


		textBlock.entries.clear();
		textBlock.width = totalWidth;
		textBlock.height = totalHeight;

		size_t penX = 0, penY = 0;

		for(size_t i = 0; i < numSections; ++i) {
			penY = totalHeight - textBlocks[i]->height/*textBlocks[i]->originY*/;
//			penY = totalHeight - (maxOriginY - textBlocks[i]->originY + textBlocks[i]->height);

/*
			size_t leadingReturns = 0;
			for(size_t p = 0; p < textBlocks[i]->entries.size(); ++p)
				if(textBlocks[i]->entries[p].character == '\n')
					++leadingReturns;
				else
					break;
*/

//printf("section %u, penY %u\n", (unsigned int) i, (unsigned int) penY);
//printf("maxOriginY %u, this originY %u\n", (unsigned int) maxOriginY, textBlocks[i]->originY);
//printf("dimensions %u, %u\n", textBlocks[i]->width, textBlocks[i]->height);
//printf("line height %u\n", gameGraphics->fontManager->lineHeights[(unsigned int) *fontSize]);
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
			delete(textBlocks[i]);
	}

	// get the actual size so possibly incorrect metrics don't skew the text
	Vector2 actualSize = Vector2(
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

	delete(elementBufferArray);

	size_t vertexBufferArraySize = quadVertices.size() * 9;
	GLfloat* vertexBufferArray = new GLfloat[vertexBufferArraySize];

	for(size_t i = 0; i < quadVertices.size(); ++i) {
		vertexBufferArray[i * 9 + 0] = quadVertices[i].position.x + metrics->position.x;
		vertexBufferArray[i * 9 + 1] = quadVertices[i].position.y + metrics->position.y;
		vertexBufferArray[i * 9 + 2] = 0.0f;
		vertexBufferArray[i * 9 + 3] = quadVertices[i].texCoord.x;
		vertexBufferArray[i * 9 + 4] = quadVertices[i].texCoord.y;

		if(quadVertices[i].color == Vector4(0.0f, 0.0f, 0.0f, 0.0f)) {
			vertexBufferArray[i * 9 + 5] = fontColor->x;
			vertexBufferArray[i * 9 + 6] = fontColor->y;
			vertexBufferArray[i * 9 + 7] = fontColor->z;
			vertexBufferArray[i * 9 + 8] = fontColor->w * timeFactor;
		} else {
			vertexBufferArray[i * 9 + 5] = quadVertices[i].color.x;
			vertexBufferArray[i * 9 + 6] = quadVertices[i].color.y;
			vertexBufferArray[i * 9 + 7] = quadVertices[i].color.z;
			vertexBufferArray[i * 9 + 8] = quadVertices[i].color.w * timeFactor;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, vertexBufferArraySize * sizeof(GL_FLOAT), vertexBufferArray,
			GL_STREAM_DRAW);

	delete(vertexBufferArray);

	// state
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["mvpMatrix"], 1, GL_FALSE, gameGraphics->idMatrixArray);
	glUniform1i(uniforms["useTexture"], 1);
	glUniform1i(uniforms["texture"], 0);
	glUniform1i(uniforms["useColor"], 1);
	glUniform1i(uniforms["useLighting"], 0);

	// activate the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameGraphics->fontManager->textureIDs[(unsigned int) *fontSize]);

	// draw the data stored in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT), (void*) 0);
	glVertexAttribPointer(attributes["texCoord"], 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GL_FLOAT),
			(GLvoid*) (5 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["texCoord"]);
	glEnableVertexAttribArray(attributes["color"]);

	glDrawElements(GL_QUADS, quadVertices.size(), GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["texCoord"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
