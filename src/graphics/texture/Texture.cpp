// Texture.cpp
// Dominicus

#include "graphics/texture/Texture.h"

#include <cstdlib>
#include <cstring>
#include <sstream>

#include "core/GameSystem.h"

extern GameSystem* gameSystem;

Texture::Texture(uint32_t newWidth, uint32_t newHeight, PixelFormat newFormat) {
	width = newWidth;
	height = newHeight;
	format = newFormat;

	size_t memSize =
			width *
			height *
			(format == FORMAT_RGBA ? 4 : 3) +
			height * (
					(width * (format == FORMAT_RGBA ? 4 : 3) % 4) > 0 ?
					4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0
				);

	pixelData = new uint8_t[memSize];

	memset(pixelData, 0, memSize);
}

Texture::Texture(std::string filename) {
	png_image pngImage;

	memset(&pngImage, 0, (sizeof pngImage));

	pngImage.version = PNG_IMAGE_VERSION;

	if(! png_image_begin_read_from_file(&pngImage, filename.c_str()))
		gameSystem->log(
				GameSystem::LOG_FATAL,
				std::string("The PNG image file " +
						std::string(filename) +
						" could not be opened for reading.").c_str()
			);

	pngImage.format = PNG_FORMAT_RGBA;

	png_bytep dataBuffer = new unsigned char[PNG_IMAGE_SIZE(pngImage)];

	if(dataBuffer == NULL || ! png_image_finish_read(&pngImage, NULL, dataBuffer, 0, NULL))
		gameSystem->log(
				GameSystem::LOG_FATAL,
				std::string("Could not read image data from PNG texture file " +
						std::string(filename) +
						".").c_str()
			);

	// flip the image vertically
	pixelData = new unsigned char[PNG_IMAGE_SIZE(pngImage)];

	for(size_t y = 0; y < pngImage.height; ++y)
		for(size_t x = 0; x < pngImage.width; ++x)
			for(size_t i = 0; i < 4; ++i)
				pixelData[(pngImage.height - 1 - y) * pngImage.width * 4 + x * 4 + i] =
						dataBuffer[y * pngImage.width * 4 + x * 4 + i];

	delete[] dataBuffer;

	format = FORMAT_RGBA;
	width = pngImage.width;
	height = pngImage.height;
}

Texture::~Texture() {
	delete[] pixelData;
}

uint8_t Texture::getRedValueAt(uint32_t column, uint32_t row) {
	if(pixelData == NULL)
		gameSystem->log(GameSystem::LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
	}

	return *((uint8_t*) pixelData +
			row *
			(width * (format == FORMAT_RGBA ? 4 : 3) +
					(width * (format == FORMAT_RGBA ? 4 : 3) % 4 > 0 ?
					4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0)) +
			column * (format == FORMAT_RGBA ? 4 : 3) +
			0);
}

uint8_t Texture::getGreenValueAt(uint32_t column, uint32_t row) {
	if(pixelData == NULL)
		gameSystem->log(GameSystem::LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
	}

	return *((uint8_t*) pixelData +
			row *
			(width * (format == FORMAT_RGBA ? 4 : 3) +
					(width * (format == FORMAT_RGBA ? 4 : 3) % 4 > 0 ?
					4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0)) +
			column * (format == FORMAT_RGBA ? 4 : 3) +
			1);
}

uint8_t Texture::getBlueValueAt(uint32_t column, uint32_t row) {
	if(pixelData == NULL)
		gameSystem->log(GameSystem::LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
	}

	return *((uint8_t*) pixelData +
			row *
			(width * (format == FORMAT_RGBA ? 4 : 3) +
					(width * (format == FORMAT_RGBA ? 4 : 3) % 4 > 0 ?
					4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0)) +
			column * (format == FORMAT_RGBA ? 4 : 3) +
			2);
}

uint8_t Texture::getAlphaValueAt(uint32_t column, uint32_t row) {
	if(pixelData == NULL)
		gameSystem->log(GameSystem::LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
	}

	if(format == FORMAT_RGBA)
		return *((uint8_t*) pixelData +
				row *
				(width * (format == FORMAT_RGBA ? 4 : 3) +
						(width * (format == FORMAT_RGBA ? 4 : 3) % 4 > 0 ?
						4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0)) +
				column * (format == FORMAT_RGBA ? 4 : 3) +
				3);
	else
		return 0xFF;
}

void Texture::setColorAt(
			uint32_t column, uint32_t row, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha
		) {
	if(pixelData == NULL)
		gameSystem->log(GameSystem::LOG_FATAL, "Texture pixel color specified before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for set out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
	}
	// create a marker pointer at the correct position in the data buffer,
	// accounting for row-padding if necessary
	uint8_t* position =
			(uint8_t*) pixelData +
			row *
			(width * (format == FORMAT_RGBA ? 4 : 3) +
					(width * (format == FORMAT_RGBA ? 4 : 3) % 4 > 0 ?
					4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0)) +
			column * (format == FORMAT_RGBA ? 4 : 3);

	// copy the colors in
	*(position++) = red;
	*(position++) = green;
	*(position++) = blue;
	if(format == FORMAT_RGBA)
		*position = alpha;
}

void Texture::setDepth(unsigned int depth) {
	for(unsigned int i = 0; i < width; ++i) {
		for(unsigned int p = 0; p < height; ++p) {
			unsigned int step = 255 / depth;
			if(step == 0) step = 1;

			uint8_t redColor = getRedValueAt((uint32_t) i, (uint32_t) p);
			uint8_t greenColor = getGreenValueAt((uint32_t) i, (uint32_t) p);
			uint8_t blueColor = getBlueValueAt((uint32_t) i, (uint32_t) p);
			uint8_t alphaColor = getAlphaValueAt((uint32_t) i, (uint32_t) p);

			uint8_t redDividend = redColor / step;
			uint8_t redModulus = redColor % step;
			if(redModulus >= step / 2) redDividend += 1;
			redColor = redDividend * step;

			uint8_t greenDividend = greenColor / step;
			uint8_t greenModulus = greenColor % step;
			if(greenModulus >= step / 2) greenDividend += 1;
			greenColor = greenDividend * step;

			uint8_t blueDividend = blueColor / step;
			uint8_t blueModulus = blueColor % step;
			if(blueModulus >= step / 2) blueDividend += 1;
			blueColor = blueDividend * step;

			setColorAt(
					(uint32_t) i,
					(uint32_t) p,
					redColor,
					greenColor,
					blueColor,
					alphaColor
				);
		}
	}
}
