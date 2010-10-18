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

Texture::Texture(unsigned int newWidth, unsigned int newHeight, PixelFormat newFormat) {
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
			sizeof(unsigned short int)
		);
}

Texture::~Texture() {
	free(pixelData);
}

unsigned short int Texture::getRedValueAt(unsigned int column, unsigned int row) {
	if(pixelData == NULL)
		ProgramLog::report(
				LOG_INTERNALERROR,
				"Texture pixel color requested before memory allocated."
			);
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_INTERNALERROR, err.str().c_str());
	}

	return *((unsigned short int*) pixelData +
			row *
			(width * (format == FORMAT_RGBA ? 4 : 3) +
					(width * (format == FORMAT_RGBA ? 4 : 3) % 4 > 0 ?
					4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0)) +
			column * (format == FORMAT_RGBA ? 4 : 3) +
			0);
}

unsigned short int Texture::getGreenValueAt(unsigned int column, unsigned int row) {
	if(pixelData == NULL)
		ProgramLog::report(
				LOG_INTERNALERROR,
				"Texture pixel color requested before memory allocated."
			);
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_INTERNALERROR, err.str().c_str());
	}

	return *((unsigned short int*) pixelData +
			row *
			(width * (format == FORMAT_RGBA ? 4 : 3) +
					(width * (format == FORMAT_RGBA ? 4 : 3) % 4 > 0 ?
					4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0)) +
			column * (format == FORMAT_RGBA ? 4 : 3) +
			1);
}

unsigned short int Texture::getBlueValueAt(unsigned int column, unsigned int row) {
	if(pixelData == NULL)
		ProgramLog::report(
				LOG_INTERNALERROR,
				"Texture pixel color requested before memory allocated."
			);
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_INTERNALERROR, err.str().c_str());
	}

	return *((unsigned short int*) pixelData +
			row *
			(width * (format == FORMAT_RGBA ? 4 : 3) +
					(width * (format == FORMAT_RGBA ? 4 : 3) % 4 > 0 ?
					4 - (width * (format == FORMAT_RGBA ? 4 : 3) % 4) : 0)) +
			column * (format == FORMAT_RGBA ? 4 : 3) +
			0);
}

unsigned short int Texture::getAlphaValueAt(unsigned int column, unsigned int row) {
	if(pixelData == NULL)
		ProgramLog::report(
				LOG_INTERNALERROR,
				"Texture pixel color requested before memory allocated."
			);
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_INTERNALERROR, err.str().c_str());
	}

	if(format == FORMAT_RGBA)
		return *((unsigned short int*) pixelData +
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
			unsigned int column, unsigned int row, unsigned short int red, unsigned short int green, unsigned short int blue, unsigned short int alpha
		) {
	if(pixelData == NULL)
		ProgramLog::report(
				LOG_INTERNALERROR,
				"Texture pixel color specified before memory allocated."
			);
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for set out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		ProgramLog::report(LOG_INTERNALERROR, err.str().c_str());
	}
	// create a marker pointer at the correct position in the data buffer,
	// accounting for row-padding if necessary
	unsigned short int* position =
			(unsigned short int*) pixelData +
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


