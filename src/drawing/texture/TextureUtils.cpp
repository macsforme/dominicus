/*
 * TextureUtils.cpp
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
