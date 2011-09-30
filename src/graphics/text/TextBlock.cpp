// TextBlock.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/text/TextBlock.h"

TextBlock::TextBlock(const char* dataString, unsigned int wrapX, unsigned int wrapY,
		FontManager* fontManager, unsigned int size) {
	// create a list of character positions for rendering a block of text

	// break the data string up into lines by newlines or width
	std::vector<std::string> lines;
	lines.push_back("");
	unsigned int pixelsWide = 0;
	int skipChars = 0;

	for(size_t i = 0; i < strlen(dataString); ++i) {
		// if we're skipping chars for a color code... well, skip them
		bool addWidth = true;
		if(skipChars-- > 0)
			addWidth = false;

		// if it's the start of a color code, skip these characters
		if(dataString[i] == '\\' && i + 8 < strlen(dataString)) {
			bool isColor = true;

			for(int p = 1; p < 9; ++p) {
				if(
						(dataString[i + p] < '0' || dataString[i + p] > '9') &&
						(dataString[i + p] < 'a' || dataString[i + p] > 'f')
					) {
					isColor = false;

					break;
				}
			}

			if(isColor) {
				skipChars = 8;

				addWidth = false;
			}
		}

		if(addWidth)
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

	if(lines.size() > 0 && lines.back().length() > 0) {
		for(size_t i = 0; i < lines.back().size(); ++i) {
			// if we're skipping chars for a color code... well, skip them
			bool useChar = true;
			if(skipChars-- > 0)
				useChar = false;

			// if it's the start of a color code, skip these characters
			if(*(lines.back().c_str() + i) == '\\' && i + 8 < strlen(lines.back().c_str())) {
				bool isColor = true;

				for(int p = 1; p < 9; ++p) {
					if(
							(*(lines.back().c_str() + i + p) < '0' || *(lines.back().c_str() + i + p) > '9') &&
							(*(lines.back().c_str() + i + p) < 'a' || *(lines.back().c_str() + i + p) > 'f')
						) {
						isColor = false;

						break;
					}
				}

				if(isColor) {
					skipChars = 8;

					useChar = false;
				}
			}

			if(useChar)
				if(fontManager->fontData[*(lines.back().c_str() + i)][size].adjustY < largestNegAdj)
					largestNegAdj = fontManager->fontData[*(lines.back().c_str() + i)][size].adjustY;
		}
	}

	// determine which lines will be printed and the dimensions of the box
	std::vector<std::string> printLines;
	width = 0;
	height = -largestNegAdj;

	for(int i = (int) lines.size() - 1; i >= 0; --i) {
//printf("line \"%s\"\n", lines[i].c_str());
		// determine the largest positive adjustment for this line to see if it fits
		int largestPosAdj = 0;
		for(size_t p = 0; p < lines[i].length(); ++p) {
			// if we're skipping chars for a color code... well, skip them
			bool useChar = true;
			if(skipChars-- > 0)
				useChar = false;

			// if it's the start of a color code, skip these characters
			if(*(lines[i].c_str() + p) == '\\' && (unsigned int) p + 8 < strlen(lines[i].c_str())) {
				bool isColor = true;

				for(int q = 1; q < 9; ++q) {
					if(
							(*(lines[i].c_str() + p + q) < '0' ||
									*(lines[i].c_str() + p + q) > '9') &&
							(*(lines[i].c_str() + p + q) < 'a' ||
									*(lines[i].c_str() + p + q) > 'f')
						) {
						isColor = false;

						break;
					}
				}

				if(isColor) {
					skipChars = 8;

					useChar = false;
				}
			}

			if(useChar) {
				FontManager::FontData* charData = &(fontManager->fontData[*(lines[i].c_str() + p)][size]);
				int posAdj = charData->adjustY + charData->height;

				if(posAdj > largestPosAdj)
					largestPosAdj = posAdj;
			}
		}

		// if we don't have room for this line, we are done
		if(height + largestPosAdj >= (int) wrapY) {
			// subtract difference between lineHeight and previous largestPosAdj from height
			// if this is not the very first iteration
			if(i < (int) lines.size() - 1) {
				unsigned int prevLargestPosAdj = 0;
				for(size_t p = 0; p < lines[i + 1].length(); ++p) {
					// if we're skipping chars for a color code... well, skip them
					bool useChar = true;
					if(skipChars-- > 0)
						useChar = false;

					// if it's the start of a color code, skip these characters
					if(*(lines[i + 1].c_str() + p) == '\\' && (unsigned int) p + 8 < strlen(lines[i + 1].c_str())) {
						bool isColor = true;

						for(int q = 1; q < 9; ++q) {
							if(
									(*(lines[i + 1].c_str() + p + q) < '0' ||
											*(lines[i + 1].c_str() + p + q) > '9') &&
									(*(lines[i + 1].c_str() + p + q) < 'a' ||
											*(lines[i + 1].c_str() + p + q) > 'f')
								) {
								isColor = false;

								break;
							}
						}

						if(isColor) {
							skipChars = 8;

							useChar = false;
						}
					}

					if(useChar) {
						FontManager::FontData* charData = &(fontManager->fontData[*(lines[i + 1].c_str() + p)][size]);
						unsigned int posAdj = charData->adjustY + charData->height;

						if(posAdj > prevLargestPosAdj)
							prevLargestPosAdj = posAdj;
					}
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
			// if we're skipping chars for a color code... well, skip them
			bool useChar = true;
			if(skipChars-- > 0)
				useChar = false;

			// if it's the start of a color code, skip these characters
			if(*(printLines[0].c_str() + p) == '\\' && (unsigned int) p + 8 < strlen(printLines[0].c_str())) {
				bool isColor = true;

				for(int q = 1; q < 9; ++q) {
					if(
							(*(printLines[0].c_str() + p + q) < '0' ||
									*(printLines[0].c_str() + p + q) > '9') &&
							(*(printLines[0].c_str() + p + q) < 'a' ||
									*(printLines[0].c_str() + p + q) > 'f')
						) {
						isColor = false;

						break;
					}
				}

				if(isColor) {
					skipChars = 8;

					useChar = false;
				}
			}

			if(useChar)
				pixelsWide += fontManager->fontData[*(printLines[0].c_str() + p)][size].advanceX;
		}

		if(pixelsWide > width)
			width = pixelsWide;

		// if the next line will put us over, or if this is the last existing line,
		// we are done and this line's height is final addition
		unsigned int lineHeight = fontManager->lineHeights[size];

		if(i == 0 || height + lineHeight >= wrapY) {
			if(largestPosAdj == 0 && lines[i] == "") {
				// add the largest positive adjustment of the last non-zero line
				for(size_t p = 0; p < printLines.size(); ++p) {
					for(size_t j = 0; j < printLines[p].length(); ++j) {
						FontManager::FontData* charData = &(fontManager->fontData[*(printLines[p].c_str() + j)][size]);
						int posAdj = charData->adjustY + charData->height;

						if(posAdj > largestPosAdj)
							largestPosAdj = posAdj;
					}

					if(largestPosAdj > 0)
						break;
				}
//printf("breaking off early... height %u, largestPosAdj %u\n", height + largestPosAdj, largestPosAdj);
			}

			height += largestPosAdj;

			break;
		}
//printf("height prior to adding: %u, after %u\n", height, height + lineHeight);
		height += lineHeight;
	}

	// text origin
	originX = 0;
	originY = 0;

	size_t skipLines = 0;

	if(lines.size() > 0) {
		for(size_t i = 0; i < lines.size(); ++i)
			if(lines[i].length() == 0)
				++skipLines;

		for(size_t i = 0; skipLines != lines.size() && i < lines[skipLines].length(); ++i) {
			// if we're skipping chars for a color code... well, skip them
			bool useChar = true;
			if(skipChars-- > 0)
				useChar = false;

			// if it's the start of a color code, skip these characters
			if(*(lines[skipLines].c_str() + i) == '\\' && (unsigned int) i + 8 < strlen(lines[skipLines].c_str())) {
				bool isColor = true;

				for(int q = 1; q < 9; ++q) {
					if(
							(*(lines[skipLines].c_str() + i + q) < '0' ||
									*(lines[skipLines].c_str() + i + q) > '9') &&
							(*(lines[skipLines].c_str() + i + q) < 'a' ||
									*(lines[skipLines].c_str() + i + q) > 'f')
						) {
						isColor = false;

						break;
					}
				}

				if(isColor) {
					skipChars = 8;

					useChar = false;
				}
			}

			if(useChar) {
				FontManager::FontData* charData = &(fontManager->fontData[*(lines[skipLines].c_str() + i)][size]);
				int posAdj = charData->adjustY + charData->height;

				if(posAdj > 0 && (unsigned int) posAdj > originY)
					originY = (unsigned int) posAdj;
			}
		}
	}
//	originY += (float) skipLines * fontManager->lineHeights[size];

	// create the character entries
	float penX = -1.0f, penY = -1.0f - ((float)largestNegAdj * 2.0f / height);
	Vector4 currentColor(0.0f, 0.0f, 0.0f, 0.0f);

	for(int i = (int) printLines.size() - 1; i >= 0; --i) {
		for(size_t p = 0; p < printLines[i].length(); ++p) {
			// if we're skipping chars for a color code... well, skip them
			bool useChar = true;
			if(skipChars-- > 0)
				useChar = false;

			// if it's the start of a color code, skip these characters
			if(*(printLines[i].c_str() + p) == '\\' && p + 8 < strlen(printLines[i].c_str())) {
				bool isColor = true;

				for(int q = 1; q < 9; ++q) {
					if(
							(*(printLines[i].c_str() + p + q) < '0' ||
									*(printLines[i].c_str() + p + q) > '9') &&
							(*(printLines[i].c_str() + p + q) < 'a' ||
									*(printLines[i].c_str() + p + q) > 'f')
						) {
						isColor = false;

						break;
					}
				}

				if(isColor) {
					// store the color
					std::string colorString = printLines[i].substr(p + 1, 8);

					int colors[4];
					sscanf(colorString.c_str(), "%2x%2x%2x%2x", &colors[0], &colors[1], &colors[2], &colors[3]);


					currentColor.x = (float) colors[0] / 255.0f;
					currentColor.y = (float) colors[1] / 255.0f;
					currentColor.z = (float) colors[2] / 255.0f;
					currentColor.w = (float) colors[3] / 255.0f;

					skipChars = 8;

					useChar = false;
				}
			}

			if(useChar) {
				CharEntry entry;
				entry.character = *(printLines[i].c_str() + p);

				FontManager::FontData* fontData = &(fontManager->fontData[entry.character][size]);

				entry.sX = penX + ((float) fontData->adjustX * 2.0f / width);
				entry.eX = entry.sX + ((float) fontData->width * 2.0f / width);
				entry.sY = penY + ((float) fontData->adjustY * 2.0f / height);
				entry.eY = entry.sY + ((float) fontData->height * 2.0f / height);

				entry.color = currentColor;

				entries.push_back(entry);

				penX += ((float) fontData->advanceX * 2.0f / width);
			}
		}

		penX = -1.0f;
		penY += (float) fontManager->lineHeights[size] * 2.0f / height;
	}


// field height adjustment code
/*
	((UIMetrics*) labelArguments["metrics"])->position.x =
			((UIMetrics*) arguments["metrics"])->position.x -
			((UIMetrics*) arguments["metrics"])->size.x / 2.0f +
			spaceWidth +
			(float) textBlock->width / (float) gameGraphics->resolutionX;
	((UIMetrics*) labelArguments["metrics"])->position.y =
			((UIMetrics*) arguments["metrics"])->position.y +
			((UIMetrics*) arguments["metrics"])->size.y / 2.0f -
			((UIMetrics*) arguments["metrics"])->size.y * 2.0f / 3.0f +
			((float) textBlock->¥ / (float) textBlock->height - 0.5f) *
			(float) textBlock->height / gameGraphics->resolutionY * 2.0f;
*/
}
