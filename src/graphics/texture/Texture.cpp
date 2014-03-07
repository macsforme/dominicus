// Texture.cpp
//
// Dominicus
// Copyright 2010-2011, Joshua Bodine
//
// Released under the terms of the "Simplified BSD License." See the file
// licenses/DOMINICUS.txt for the license text.

#include "graphics/texture/Texture.h"

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

Texture::Texture(uint32_t newWidth, uint32_t newHeight, PixelFormat newFormat) {
	width = newWidth;
	height = newHeight;
	format = newFormat;

	pixelData = calloc(
 			width *
			height *
			(format == FORMAT_RGBA ? 4 : 3) +
			height * (
						(width * (format == FORMAT_RGBA ? 4 : 3) % 4) > 0 ?
						4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0
				),
			sizeof(uint8_t)
		);
}

Texture::Texture(std::string filename) {
	// ===== Opening and reading the file =====

	// open the file
	std::ifstream fileStream;
	fileStream.open(filename.c_str(), std::ios::binary);

	// ensure the file was successfully opened
	if(! fileStream.is_open()) {
		gameSystem->log(
				GameSystem::LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" could not be opened for reading.").c_str()
			);
	}

	// get length of the file
	fileStream.seekg(0, std::ios::end);
	size_t length = (size_t)fileStream.tellg();

	fileStream.seekg(0, std::ios::beg);

	// allocate memory for the data we're about to read
	uint8_t* fileContents = (uint8_t*) malloc(length);

	// read the contents
	fileStream.read((char*) fileContents, length);

	// close the file
	fileStream.close();

	// ===== Data storage structs =====

	// file type check
	struct {
			char magic[2];
		} fileMagic;

	// BMP file header
	struct {
			uint32_t fileSize;
			uint16_t reserved1;
			uint16_t reserved2;
			uint32_t dataOffset;
		} fileBMPHeader;

	// DIB file header
	struct {
			uint32_t headerSize;
			uint32_t imageWidth;
			uint32_t imageHeight;	// does not support upside-down images
			uint16_t colorPlanes;
			uint16_t bpp;
			uint32_t compressionMethod;
			uint32_t imageDataSize;
			uint32_t horizontalDPI;
			uint32_t verticalDPI;
			uint32_t numIndexedColors;
			uint32_t numImportantIndexedColors;
		} fileDIBHeader;

	// ===== Extracting headers =====

	// Extract magic header
	memcpy(
			(void*) &fileMagic,
			(void*) &fileContents,
			sizeof(fileMagic)
		);

	if(fileMagic.magic[0] == 'B' && fileMagic.magic[1] == 'M') {
		gameSystem->log(
				GameSystem::LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" does not appear to be a valid BMP file.").c_str()
				);
	}

	// Extract BMP header
	memcpy(
			(void*) &fileBMPHeader,
			(void*) (fileContents + sizeof(fileMagic)),
			sizeof(fileBMPHeader)
		);

	// We only support the BITMAPINFOHEADER file format (actually BITMAPV4HEADER seems to work too)
	if(
			fileBMPHeader.dataOffset - sizeof(fileMagic) - sizeof(fileBMPHeader) != 40 &&
			fileBMPHeader.dataOffset - sizeof(fileMagic) - sizeof(fileBMPHeader) != 108
       ) {
		gameSystem->log(
				GameSystem::LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" appears to have an incorrect DIB header size.").c_str()
				);
	}

	// Extract DIB header
	memcpy(
			(void*) &fileDIBHeader,
			(void*) (fileContents + sizeof(fileMagic) + sizeof(fileBMPHeader)),
			sizeof(fileDIBHeader)
		);

	// Check again that we are using the BITMAPINFOHEADER or BITMAPV4HEADER file format
	if(fileDIBHeader.headerSize != 40 && fileDIBHeader.headerSize != 108) {
		gameSystem->log(
				GameSystem::LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" reports an incorrect DIB header size.").c_str()
				);
	}

	// We don't support compressed bitmaps, only BI_RGB method
	if(fileDIBHeader.compressionMethod != 0) {
		gameSystem->log(
				GameSystem::LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" DIB header reports an unsupported image compression method.").c_str()
				);
	}

	// We will probably never use indexed bitmaps, so we don't support them
	if(fileDIBHeader.numIndexedColors > 0) {
		gameSystem->log(
				GameSystem::LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" DIB header reports an unsupported color-indexed format.").c_str()
				);
	}

	// Ensure a legal bits-per-pixel specification
	if(
			fileDIBHeader.bpp != 24 &&
			fileDIBHeader.bpp != 32
		) {
		gameSystem->log(
				GameSystem::LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" DIB header reports an unsupported pixel depth of " +
						std::string("" + fileDIBHeader.bpp) +
						".").c_str()
				);
	}

	// ===== Reading pixel data =====

	// Here we read the pixel data according to the bits depth specification.
	// Presently only 24 and 32-bit depths are supported, since these are the
	// only types of images commonly used as textures.

	// Note that widths are padded-out to a multiple of 4 if necessary, so
	// that the RGB(A) data is read properly to OpenGL (this could also be
	// solved by executing glPixelStorei(GL_UNPACK_ALIGNMENT, 1) for 1 byte.
	size_t pixelSize = fileDIBHeader.bpp / 8;
	uint32_t skipBytesPerRow =
			((fileDIBHeader.imageWidth * pixelSize) % 4 > 0 ?
					4 - (fileDIBHeader.imageWidth * pixelSize) % 4 : 0);

	// Allocate and initialize the necessary memory
	pixelData = calloc(
			(
					fileDIBHeader.imageHeight *
					fileDIBHeader.imageWidth *
					pixelSize
				)
			+ skipBytesPerRow * fileDIBHeader.imageHeight,
			sizeof(uint8_t)
		);

	// Different depths require different extraction methods
	switch(fileDIBHeader.bpp) {
		case 24:
		case 32:
		{
			// Create our marker pointer
			uint8_t* dataMark = (fileContents + (size_t) fileBMPHeader.dataOffset);

			// Loop through the entire stored data buffer, allocating pixel values
			// Pixel data is stored from bottom to top, left to right
			for(uint32_t y = 0;	y < fileDIBHeader.imageHeight; ++y) {
				for(uint32_t x = 0; x < fileDIBHeader.imageWidth; ++x) {
					// Make sure we aren't past the end of the pixel data
					if(dataMark - (fileContents + fileBMPHeader.dataOffset)
							> fileDIBHeader.imageDataSize) {
						std::stringstream err;
						err << "The BMP image file "
								<< filename
								<< " encountered a premature image data end at byte "
								<< (dataMark - fileContents)
								<< " (expected size "
								<< fileDIBHeader.imageDataSize
								<< ") at pixel "
								<< x << "," << y
								<< " (actual image size "
								<< fileDIBHeader.imageWidth << ","
								<< fileDIBHeader.imageHeight <<
								").";

						gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
					}

					// Pixel data starting point
					uint8_t* pixelStart =
							(uint8_t*) pixelData +
							y * (
									fileDIBHeader.imageWidth * pixelSize + skipBytesPerRow
								) +
							x * pixelSize;

					// Copy the colors into the pixel array
					*(pixelStart + 2) = *(dataMark++);	// blue
					*(pixelStart + 1) = *(dataMark++);	// green
					*(pixelStart) = *(dataMark++);	// red
					if(fileDIBHeader.bpp == 32)
						*(pixelStart + 3) = *(dataMark++);	// alpha
				}

				// For 24-bpp images, pad the row out until the bytes are divisible by 4
				if(fileDIBHeader.bpp == 24)
					dataMark += fileDIBHeader.imageWidth % 4;
			}

			// Make sure we exactly at the end of the pixel data
			int shortfall = (int)(fileContents + fileBMPHeader.fileSize - dataMark);
			if(shortfall != 0) {
				std::stringstream err;
				err << "The BMP image file "
						<< filename
						<< " has image data "
						<< (shortfall < 0 ? -shortfall : shortfall)
						<< (shortfall < 0 ? " short of" : " in excess of")
						<< " the expected data length.";

				gameSystem->log(GameSystem::LOG_FATAL, err.str().c_str());
			}

			break;
		}
	}

	// Free the file buffer
	free(fileContents);

	// Set the pixel storage format, image width, and image height
	format = (fileDIBHeader.bpp == 32 ? FORMAT_RGBA : FORMAT_RGB);
	width = fileDIBHeader.imageWidth;
	height = fileDIBHeader.imageHeight;
}

Texture::~Texture() {
	free(pixelData);
}
