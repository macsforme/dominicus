/*
 *  TextureUtils.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 2/16/11.
 *  Copyright 2011 Joshua Bodine. All rights reserved.
 *
 */

#include "drawing/texture/TextureUtils.h"

void setTextureDepth(Texture* subject, unsigned int depth) {
	for(unsigned int i = 0; i < subject->getWidth(); ++i) {
		for(unsigned int p = 0; p < subject->getHeight(); ++p) {
			unsigned int step = 255 / depth;
			if(step == 0) step = 1;

			uint8_t redColor = subject->getRedValueAt((uint32_t) i, (uint32_t) p);
			uint8_t greenColor = subject->getGreenValueAt((uint32_t) i, (uint32_t) p);
			uint8_t blueColor = subject->getBlueValueAt((uint32_t) i, (uint32_t) p);
			uint8_t alphaColor = subject->getAlphaValueAt((uint32_t) i, (uint32_t) p);

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

			subject->setColorAt(
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
