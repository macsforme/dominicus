/*
 *  BMPImage.h
 *  dominicus
 *
 *  Created by Joshua Bodine on 5/30/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "BMPImage.h"

BMPImage::BMPImage(const char* filename) {
	// ===== Opening and reading the file =====

	// open the file
	std::ifstream fileStream;
	fileStream.open(filename, std::ios::binary);

	// ensure the file was successfully opened
	if(! fileStream.is_open()) {
		ProgramLog::report(
				LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" could not be opened for reading.").c_str()
			);
	}

	// get length of the file
	fileStream.seekg(0, std::ios::end);
	size_t length = fileStream.tellg();

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
		ProgramLog::report(
				LOG_FATAL,
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

	// We only support the BITMAPINFOHEADER file format
	if(fileBMPHeader.dataOffset - sizeof(fileMagic) - sizeof(fileBMPHeader) != 40) {
		ProgramLog::report(
				LOG_FATAL,
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
	// Check again that we are using the BITMAPINFOHEADER file format
	if(fileDIBHeader.headerSize != 40) {
		ProgramLog::report(
				LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" reports an incorrect DIB header size.").c_str()
				);
	}

	// We don't support compressed bitmaps, only BI_RGB method
	if(fileDIBHeader.compressionMethod != 0) {
		ProgramLog::report(
				LOG_FATAL,
				std::string("The BMP image file " +
						std::string(filename) +
						" DIB header reports an unsupported image compression method.").c_str()
				);
	}

	// We will probably never use indexed bitmaps, so we don't support them
	if(fileDIBHeader.numIndexedColors > 0) {
		ProgramLog::report(
				LOG_FATAL,
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
		ProgramLog::report(
				LOG_FATAL,
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

						ProgramLog::report(LOG_FATAL, err.str().c_str());
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

				// For 24-bpp images, pad the row out until the bytes are divisable by 4
				if(fileDIBHeader.bpp == 24)
					dataMark += fileDIBHeader.imageWidth % 4;
			}

			// Make sure we exactly at the end of the pixel data
			size_t shortfall = fileContents + fileBMPHeader.fileSize - dataMark;
			if(shortfall != 0) {
				std::stringstream err;
				err << "The BMP image file "
						<< filename
						<< " has image data "
						<< (shortfall < 0 ? -shortfall : shortfall)
						<< (shortfall < 0 ? "short of" : "in excess of")
						<< " the expected data length.";

				ProgramLog::report(LOG_FATAL, err.str().c_str());
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


