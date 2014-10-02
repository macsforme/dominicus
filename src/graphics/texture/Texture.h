// Texture.h
// Crucible Island

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <string>

class Texture {
private:
	void* pixelData;

public:
	enum PixelFormat {
			FORMAT_RGB,
			FORMAT_RGBA
		};

	uint32_t width, height;
	PixelFormat format;

	Texture(uint32_t newWidth, uint32_t newHeight, PixelFormat newFormat);
	Texture(std::string filename);
	~Texture();

	void* getDataPointer() { return pixelData; }

	uint8_t getRedValueAt(uint32_t column, uint32_t row);
	uint8_t getGreenValueAt(uint32_t column, uint32_t row);
	uint8_t getBlueValueAt(uint32_t column, uint32_t row);
	uint8_t getAlphaValueAt(uint32_t column, uint32_t row);

	void setColorAt(uint32_t column, uint32_t row, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

	void setDepth(unsigned int depth);
};

#endif // TEXTURE_H
