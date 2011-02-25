/*
 * TextBlock.cpp
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

#include "drawing/text/TextBlock.h"

TextBlock::TextBlock(const char* dataString, unsigned int wrapX, unsigned int wrapY,
		FontManager* fontManager, unsigned int size) {
	// create a list of character positions for rendering a block of text

	// break the data string up into lines by newlines or width
	std::vector<std::string> lines;
	lines.push_back("");
	unsigned int pixelsWide = 0;

	for(size_t i = 0; i < strlen(dataString); ++i) {
		pixelsWide += fontManager->fontData[dataString[i]][size].advanceX;

		// newline test
		if(pixelsWide > wrapX || dataString[i] == '\n') {
			if(dataString[i] != '\n') {
				// line doesn't fit... backtrack to the last space and go to the next line
				int p = lines.back().length() - 1;

				// make sure there is a breaking space somewhere
				while(p > 0 && *(lines.back().c_str() + p) != ' ') --p;
				if(p > 0) {
					// breaking space was found
					while(*(lines.back().c_str() + lines.back().length() - 1) != ' ') {
						lines.back().erase(lines.back().length() - 1);

						--i;
					}
				}

				--i;
			}

			lines.push_back("");

			pixelsWide = 0;
		} else {
			lines.back() += dataString[i];
		}
	}

	// determine the inverse of the largest negative adjustment of the last line
	int largestNegAdj = 0;

	if(lines.size() > 0 && lines.back().length() > 0)
		for(size_t i = 0; i < lines.back().size(); ++i)
			if(fontManager->fontData[*(lines.back().c_str() + i)][size].adjustY < largestNegAdj)
				largestNegAdj = fontManager->fontData[*(lines.back().c_str() + i)][size].adjustY;

	// determine which lines will be printed and the dimensions of the box
	std::vector<std::string> printLines;
	width = 0;
	height = -largestNegAdj;

	for(int i = (int) lines.size() - 1; i >= 0; --i) {
		// determine the largest positive adjustment for this line to see if it fits
		int largestPosAdj = 0;
		for(size_t p = 0; p < lines[i].length(); ++p) {
			FontManager::FontData* charData = &(fontManager->fontData[*(lines[i].c_str() + p)][size]);
			int posAdj = charData->adjustY + charData->height;

			if(posAdj > largestPosAdj)
				largestPosAdj = posAdj;
		}

		// if we don't have room for this line, we are done
		if(height + largestPosAdj >= wrapY) {

			// subtract difference between lineHeight and previous largestPosAdj from height
			// if this is not the very first iteration
			if(i < (int) lines.size() - 1) {
				unsigned int prevLargestPosAdj = 0;
				for(size_t p = 0; p < lines[i + 1].length(); ++p) {
					FontManager::FontData* charData = &(fontManager->fontData[*(lines[i + 1].c_str() + p)][size]);
					unsigned int posAdj = charData->adjustY + charData->height;

					if(posAdj > prevLargestPosAdj)
						prevLargestPosAdj = posAdj;
				}

				height -= (int) fontManager->lineHeights[size] - prevLargestPosAdj;
			}

			break;
		}

		// we have room, so add the line
		printLines.insert(printLines.begin(), lines[i]);

		// update the width if necessary
		unsigned int pixelsWide = 0;
		for(size_t p = 0; p < printLines[0].length(); ++p) {
			pixelsWide += fontManager->fontData[*(printLines[0].c_str() + p)][size].advanceX;
		}

		if(pixelsWide > width)
			width = pixelsWide;

		// if the next line will put us over, we are done and this line's height is final addition
		unsigned int lineHeight = fontManager->lineHeights[size];

		if(height + lineHeight >= wrapY) {
			height += largestPosAdj;

			break;
		}

		// add the lineHeight, but this might change in next iteration if it decides
		// it isn't going to be drawn
		height += lineHeight;
	}

	// create the character entries
	float penX = -1.0f, penY = -1.0f - ((float)largestNegAdj * 2.0f / height);

	for(int i = (int) printLines.size() - 1; i >= 0; --i) {
		for(size_t p = 0; p < printLines[i].length(); ++p) {
			CharEntry entry;
			entry.character = *(printLines[i].c_str() + p);

			FontManager::FontData* fontData = &(fontManager->fontData[entry.character][size]);

			entry.sX = penX + ((float) fontData->adjustX * 2.0f / width);
			entry.eX = entry.sX + ((float) fontData->width * 2.0f / width);
			entry.sY = penY + ((float) fontData->adjustY * 2.0f / height);
			entry.eY = entry.sY + ((float) fontData->height * 2.0f / height);

			entries.push_back(entry);

			penX += ((float) fontData->advanceX * 2.0f / width);
		}

		penX = -1.0f;
		penY += (float) fontManager->lineHeights[size] * 2.0f / height;
	}
}
