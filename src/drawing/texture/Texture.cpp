/*
 *  Texture.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 6/9/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Texture.h"

Texture::Texture() {
	// for subclasses only
	width = 0;
	height = 0;
	pixelData = NULL;
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

Texture::~Texture() {
	free(pixelData);
}

uint8_t Texture::getRedValueAt(uint32_t column, uint32_t row) {
	if(pixelData == NULL)
		ProgramLog::report(LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_FATAL, err.str().c_str());
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
		ProgramLog::report(LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_FATAL, err.str().c_str());
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
		ProgramLog::report(LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_FATAL, err.str().c_str());
	}

	return *((uint8_t*) pixelData +
			row *
			(width * (format == FORMAT_RGBA ? 4 : 3) +
					(width * (format == FORMAT_RGBA ? 4 : 3) % 4 > 0 ?
					4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0)) +
			column * (format == FORMAT_RGBA ? 4 : 3) +
			0);
}

uint8_t Texture::getAlphaValueAt(uint32_t column, uint32_t row) {
	if(pixelData == NULL)
		ProgramLog::report(LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_FATAL, err.str().c_str());
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
		ProgramLog::report(LOG_FATAL, "Texture pixel color specified before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for set out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_FATAL, err.str().c_str());
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
