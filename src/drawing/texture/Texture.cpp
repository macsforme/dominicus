/*
 * Texture.cpp
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

#include "drawing/texture/Texture.h"

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
		programLog->report(ProgramLog::LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		programLog->report(ProgramLog::LOG_FATAL, err.str().c_str());
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
		programLog->report(ProgramLog::LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		programLog->report(ProgramLog::LOG_FATAL, err.str().c_str());
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
		programLog->report(ProgramLog::LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		programLog->report(ProgramLog::LOG_FATAL, err.str().c_str());
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
		programLog->report(ProgramLog::LOG_FATAL, "Texture pixel color requested before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for retrieval out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		programLog->report(ProgramLog::LOG_FATAL, err.str().c_str());
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
		programLog->report(ProgramLog::LOG_FATAL, "Texture pixel color specified before memory allocated.");
	if(column >= width || row >= height) {
		std::stringstream err;
		err << "Texture row or column index for set out-of-bounds at " <<
				column << "," <<
				row << " (dimensions " <<
				width << "," <<
				height << ").";

		programLog->report(ProgramLog::LOG_FATAL, err.str().c_str());
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
