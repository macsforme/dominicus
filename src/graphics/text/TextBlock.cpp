// TextBlock.cpp
// Crucible Island

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

	// determine which lines will be printed and the dimensions of the box
	std::vector<std::string> printLines;
	width = 0;
	height = 0;

	for(int i = (int) lines.size() - 1; i >= 0; --i) {
		// if we don't have room for this line, we are done
		if(height + fontManager->lineHeights[size] > (int) wrapY)
			break;

		printLines.insert(printLines.begin(), lines[i]);

		height += fontManager->lineHeights[size];

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
	}

	// create the character entries
	float penX = -1.0f, penY = -1.0f + ((float) fontManager->descenders[size] * 2.0f / height);
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
}
