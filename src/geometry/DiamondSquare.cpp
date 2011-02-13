/*
 *  DiamondSquare.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/18/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "DiamondSquare.h"

DiamondSquare::DiamondSquare(unsigned int size, float roughness) :
		size(size) {
	// initialize the memory
	data = (float**) malloc(size * sizeof(float*));
	for(unsigned int i = 0; i < size; ++i)
		data[i] = (float*) malloc(size * sizeof(float));

	// zero out the memory
	for(unsigned int i = 0; i < size; ++i)
		for(unsigned int p = 0; p < size; ++p)
			data[i][p] = 0.0f;

	// do diamond-square algorithm displacement
	float displaceRange = 1.0f;

	for(int i = 0; i < (int)log2(size); ++i) {
		// diamond step
		for(
				unsigned int p = size / (int)pow(2.0, i + 1);
				p <= size - size / (int)pow(2.0, i + 1);
				p += 2 * (size / (int)pow(2.0, i + 1))
			) {
			for(
					unsigned int j = size / (int)pow(2.0, i + 1);
					j <= size - size / (int)pow(2.0, i + 1);
					j += 2 * (size / (int)pow(2.0, i + 1))
				) {
				unsigned int offset = size / (int)pow(2.0, i + 1);

				data[p][j] = (
						data[p - offset][j - offset] +
						data[p - offset][(j + offset >= size ? j + offset - size : j + offset)] +
						data[(p + offset >= size ? p + offset - size : p + offset)][j - offset] +
						data[(p + offset >= size ? p + offset - size : p + offset)]
								[(j + offset >= size ? j + offset - size : j + offset)]
					) / 4.0f + ((float) (rand() % 100) / 50.0f - 1.0f) * displaceRange;
			}
		}

		// square step
		unsigned int jump = (int)(size / pow(2.0, i));

		for(
				int m = 0;
				m < pow(2.0, i);
				++m
			) {

			for(
					int n = 0;
					n < pow(2.0, i);
					++n
				) {

				// west -x
				data[m * jump][jump / 2 + n * jump] = (
						// west neighbor
						data[
								(m * jump - jump / 2 < 0 ?
										m * jump - jump / 2 + size :
										m * jump - jump / 2)
							][jump / 2 + n * jump] +
						// east neighbor
						data[jump / 2 + m * jump][jump / 2 + n * jump] +
						// north neighbor
						data[m * jump][
								(jump + n * jump >= size ?
										jump + n * jump - size :
										jump + n * jump)
							] +
						// south neighbor
						data[m * jump][n * jump]
					) / 4.0f;
				// south -y
				data[(int) ((0.5f + m) * jump)][n * jump] = (
						// west neighbor
						data[m * jump][n * jump] +
						// east neighbor
						data[
								((m + 1) * jump >= size ?
										(m + 1) * jump - size :
										(m + 1) * jump)
							][n * jump] +
						// north neighbor
						data[(int) ((0.5f + m) * jump)][(int) ((0.5f + n) * jump)] +
						// south neighbor
						data[(int) ((0.5f + m) * jump)][
								(n * jump - jump / 2 < 0 ?
										n * jump - jump / 2 + size :
										n * jump - jump / 2)
							]
					) / 4.0f + ((float) (rand() % 100) / 50.0f - 1.0f) * displaceRange;
			}
		}

		// reduce displacement range
		displaceRange *= roughness;
	}

	// re-map all the values to +1.0f, -1.0f
	float min = 0.0f, max = 0.0f;

	for(unsigned int i = 0; i < size; ++i) {
		for(unsigned int p = 0; p < size; ++p) {
			if(data[i][p] > max)
				max = data[i][p];
			else if(data[i][p] < min)
				min = data[i][p];
		}
	}

	float range = 0.0f - min + max;

	for(unsigned int i = 0; i < size; ++i)
		for(unsigned int p = 0; p < size; ++p)
			data[i][p] = (-min + data[i][p]) * 2.0f / range - 1.0f;
}
