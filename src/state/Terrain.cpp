/*
 *  Terrain.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/29/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "Terrain.h"

Terrain::Terrain() {
	// randomly generate our input variables within the set limits
	/*unsigned int*/ density = 128;
	float rough = 0.4f + (float) (rand() % 100) / 100.0f * 0.2f;
	float gradDist = 0.45f;
	/*unsigned int*/ alphaBumps = 2 + (int) ((float) (rand() % 100) / 100.0f * 3.0f);
	float flatness = 0.75f;

	// generate the initial diamond-square heightmap
	DiamondSquare heightMap(density, rough);

	// generate one or more "alphaBump" mappings for height variance in island
	unsigned short int alphaBumpData[alphaBumps][density][density];

	for(int i = 0; i < alphaBumps; ++i) {
		// create a middle point for where to start the bump
		Vector2 midpoint;
		if(alphaBumps == 1)
			midpoint = Vector2(0.0f, 0.0f);	// if there's only one, center it
		else
			midpoint = Vector2(	// range -0.5 - 0.5
					(float) (rand() % 100) / 100.0f * 1.0f - 0.5f,
					(float) (rand() % 100) / 100.0f * 1.0f - 0.5f
				);

		// determine the bump radius based on distance to edge
		Vector2 posCoord(absolute(midpoint.x), absolute(midpoint.y));

		float radius = minimum(1.0f - posCoord.x, 1.0f - posCoord.y);

		// select one random row from the heightmap for randomness
		unsigned int mapIndex = (rand() % 100) * density / 100;

		// fill the mask with values
		for(int p = 0; p < density; ++p) {
			for(int j = 0; j < density; ++j) {
				// coordinate of these indices
				Vector2 coord(
					(float) p / (float) density * 2.0f - 1.0f,
					(float) j / (float) density * 2.0f - 1.0f
				);

				// calculate the angle formed by a right triangle with the centerpoint and this one
				Vector2 point(coord);
				point -= midpoint;
				point.norm();
				Vector2 zeroAngle(1.0f, 0.0f);
				float angle = degrees(acos(dot(zeroAngle, point)));
				if(point.y < 0.0f)
					angle = 360.0f - angle;

				// number of variation levels we allow from incoming data
				int levels = 128;

				// vary the circumference
				float dist = distance(coord, midpoint);

				dist +=
						(float)
						((int)	// precision reduction
						((heightMap.data[mapIndex][(int) (angle / 360.0f * (float) density)]	// the value from the heightmap
						+ 1.0f)	// bumped up to positive-only values
						/ 2.0f	// divided back down to 0.0 - 1.0 range
						* levels))	// precision reduction to 0-levels
						/ (float) levels	// back down to float, 0-1.0 range
						/ 2.0f	// final range 0-0.5
						;

				if(dist < radius - gradDist)
						alphaBumpData[i][p][j] = 255;
				else if(dist < radius)
						alphaBumpData[i][p][j] = (radius - dist) / gradDist * 255;
				else
					alphaBumpData[i][p][j] = 0;
			}
		}

//printf("midpoint %f, %f, radius %f\n", midpoint.x, midpoint.y, radius);
	}

	// create an alphabump map with the combined alpha images
	unsigned short int finalAlphaBumpMap[density][density];

	for(int i = 0; i < density; ++i) {
		for(int p = 0; p < density; ++p) {
			finalAlphaBumpMap[i][p] = 0;

			for(int j = 0; j < alphaBumps; ++j)
				finalAlphaBumpMap[i][p] = (float) maximum(
						(float) finalAlphaBumpMap[i][p],
						(float) alphaBumpData[j][i][p]
					) * flatness;
		}
	}

	// finally, adjust the heightmap for the alpha values
//	unsigned short int finalHeightMap[density][density];
	finalHeightMap = (unsigned short int**) malloc(density * sizeof(unsigned short int*));
	for(int i = 0; i < density; ++i)
		finalHeightMap[i] = (unsigned short int*) malloc(density * sizeof(unsigned short int));

	for(int i = 0; i < density; ++i)
		for(int p = 0; p < density; ++p)
			finalHeightMap[i][p] = (heightMap.data[i][p] + 1.0f) * (float) finalAlphaBumpMap[i][p] / 2.0f;

//(unsigned short int) ((heightMap.data[i][p] + 1.0f) * 128.0f * alphaData[i][p] / 256.0f),

	// render a texture of each alpha bump map
	for(int i = 0; i < alphaBumps; ++i) {
		Texture texture(density, density, Texture::FORMAT_RGB);

		for(int p = 0; p < density; ++p)
			for(int j = 0; j < density; ++j)
				texture.setColorAt(
						p,
						j,
						alphaBumpData[i][p][j],
						alphaBumpData[i][p][j],
						alphaBumpData[i][p][j],
						1.0f
					);

		GLuint textureID;
		glGenTextures(1, &textureID);
		alphaBumpTextureIDs.push_back(textureID);
		glBindTexture(GL_TEXTURE_2D, alphaBumpTextureIDs.back());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(
				GL_TEXTURE_2D,
				0,
				(texture.getFormat() == Texture::FORMAT_RGB ? GL_RGB : GL_RGBA),
				texture.getWidth(),
				texture.getHeight(),
				0,
				(texture.getFormat() == Texture::FORMAT_RGB ? GL_RGB : GL_RGBA),
				GL_UNSIGNED_BYTE,
				texture.getDataPointer()
			);
	}

	// one for the noise
	Texture noiseTexture(density, density, Texture::FORMAT_RGB);

	for(int p = 0; p < density; ++p)
		for(int j = 0; j < density; ++j)
			noiseTexture.setColorAt(
					p,
					j,
					(unsigned short int) ((heightMap.data[p][j] + 1.0f) * 128.0f),
					(unsigned short int) ((heightMap.data[p][j] + 1.0f) * 128.0f),
					(unsigned short int) ((heightMap.data[p][j] + 1.0f) * 128.0f),
					1.0f
				);

	glGenTextures(1, &noiseTextureID);
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(noiseTexture.getFormat() == Texture::FORMAT_RGB ? GL_RGB : GL_RGBA),
			noiseTexture.getWidth(),
			noiseTexture.getHeight(),
			0,
			(noiseTexture.getFormat() == Texture::FORMAT_RGB ? GL_RGB : GL_RGBA),
			GL_UNSIGNED_BYTE,
			noiseTexture.getDataPointer()
		);


	// and one for the final alpha image
	Texture texture(density, density, Texture::FORMAT_RGB);

	for(int p = 0; p < density; ++p)
		for(int j = 0; j < density; ++j)
			texture.setColorAt(
					p,
					j,
					finalAlphaBumpMap[p][j],
					finalAlphaBumpMap[p][j],
					finalAlphaBumpMap[p][j],
					1.0f
				);

	glGenTextures(1, &finalTextureID);
	glBindTexture(GL_TEXTURE_2D, finalTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(texture.getFormat() == Texture::FORMAT_RGB ? GL_RGB : GL_RGBA),
			texture.getWidth(),
			texture.getHeight(),
			0,
			(texture.getFormat() == Texture::FORMAT_RGB ? GL_RGB : GL_RGBA),
			GL_UNSIGNED_BYTE,
			texture.getDataPointer()
		);

	// and one for the final combined image
	Texture combinedImage(density, density, Texture::FORMAT_RGB);

	for(int p = 0; p < density; ++p)
		for(int j = 0; j < density; ++j)
			combinedImage.setColorAt(
					p,
					j,
					finalHeightMap[p][j],
					finalHeightMap[p][j],
					finalHeightMap[p][j],
					1.0f
				);

	glGenTextures(1, &finalCombinedTextureID);
	glBindTexture(GL_TEXTURE_2D, finalCombinedTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(combinedImage.getFormat() == Texture::FORMAT_RGB ? GL_RGB : GL_RGBA),
			combinedImage.getWidth(),
			combinedImage.getHeight(),
			0,
			(combinedImage.getFormat() == Texture::FORMAT_RGB ? GL_RGB : GL_RGBA),
			GL_UNSIGNED_BYTE,
			combinedImage.getDataPointer()
		);

	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/default.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/default.fragment.glsl").c_str(),
			GL_FRAGMENT_SHADER
		);

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	program = ShaderTools::makeProgram(shaders);

	// get attribute locations
	positionAttrib = glGetAttribLocation(program, "position");
	texCoordAttrib = glGetAttribLocation(program, "texCoord");

	// get uniform locations
	mvpMatrixUniform = glGetUniformLocation(program, "mvpMatrix");
	textureUniform = glGetUniformLocation(program, "texture");
}

void Terrain::render() {
	glUseProgram(program);

	Matrix4 mvpMatrix;
	mvpMatrix.identity();
	float mvpMatrixArray[] = {
			mvpMatrix.m11, mvpMatrix.m21, mvpMatrix.m31, mvpMatrix.m41,
			mvpMatrix.m12, mvpMatrix.m22, mvpMatrix.m32, mvpMatrix.m42,
			mvpMatrix.m13, mvpMatrix.m23, mvpMatrix.m33, mvpMatrix.m43,
			mvpMatrix.m14, mvpMatrix.m24, mvpMatrix.m34, mvpMatrix.m44
		};
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrixArray);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureUniform, 0);

	Vector2 boxDim(0.25, (4.0f/3.0f) * 0.25);
	Vector2 spaceDim(0.05, (4.0f/3.0f) * 0.05f);

	// render the preliminary alpha bumps
	for(int i = 0; i < alphaBumps; ++i) {
		glBindTexture(GL_TEXTURE_2D, alphaBumpTextureIDs[i]);

		glBegin(GL_QUADS);

		glVertexAttrib2f(texCoordAttrib, 0.0f, 0.0f);
		glVertexAttrib3f(
				positionAttrib,
				-1.0f + (float) i * (boxDim.x + spaceDim.x),
				1.0f - boxDim.y,
				0.0f
			);

		glVertexAttrib2f(texCoordAttrib, 0.0f, 1.0f);
		glVertexAttrib3f(
				positionAttrib,
				-1.0f + (float) i * (boxDim.x + spaceDim.x),
				1.0f,
				0.0f
			);

		glVertexAttrib2f(texCoordAttrib, 1.0f, 1.0f);
		glVertexAttrib3f(
				positionAttrib,
				-1.0f + boxDim.x + (float) i * (boxDim.x + spaceDim.x),
				1.0f,
				0.0f
			);

		glVertexAttrib2f(texCoordAttrib, 1.0f, 0.0f);
		glVertexAttrib3f(
				positionAttrib,
				-1.0f + boxDim.x + (float) i * (boxDim.x + spaceDim.x),
				1.0f - boxDim.y,
				0.0f
			);

		glEnd();
	}

	// the noise itself
	glBindTexture(GL_TEXTURE_2D, noiseTextureID);

	glBegin(GL_QUADS);

	glVertexAttrib2f(texCoordAttrib, 0.0f, 0.0f);
	glVertexAttrib3f(
			positionAttrib,
			1.0f - boxDim.x,
			1.0f - boxDim.y,
			0.0f
		);

	glVertexAttrib2f(texCoordAttrib, 0.0f, 1.0f);
	glVertexAttrib3f(
			positionAttrib,
			1.0f - boxDim.x,
			1.0f,
			0.0f
		);

	glVertexAttrib2f(texCoordAttrib, 1.0f, 1.0f);
	glVertexAttrib3f(
			positionAttrib,
			1.0f,
			1.0f,
			0.0f
		);

	glVertexAttrib2f(texCoordAttrib, 1.0f, 0.0f);
	glVertexAttrib3f(
			positionAttrib,
			1.0f,
			1.0f - boxDim.y,
			0.0f
		);

	glEnd();

	// render the final alpha bump
	glBindTexture(GL_TEXTURE_2D, finalTextureID);

	glBegin(GL_QUADS);

		glVertexAttrib2f(texCoordAttrib, 0.0f, 0.0f);
		glVertexAttrib3f(
				positionAttrib,
				-1.0f,
				1.0f - (boxDim.y * 2 + spaceDim.y),
				0.0f
			);

		glVertexAttrib2f(texCoordAttrib, 0.0f, 1.0f);
		glVertexAttrib3f(
				positionAttrib,
				-1.0f,
				1.0f - boxDim.y - spaceDim.y,
				0.0f
			);

		glVertexAttrib2f(texCoordAttrib, 1.0f, 1.0f);
		glVertexAttrib3f(
				positionAttrib,
				-1.0f + boxDim.x,
				1.0f - boxDim.y - spaceDim.y,
				0.0f
			);

		glVertexAttrib2f(texCoordAttrib, 1.0f, 0.0f);
		glVertexAttrib3f(
				positionAttrib,
				-1.0f + boxDim.x,
				1.0f - (boxDim.y * 2 + spaceDim.y),
				0.0f
			);

	glEnd();

	// render the final combined image
	glBindTexture(GL_TEXTURE_2D, finalCombinedTextureID);

	glBegin(GL_QUADS);

		glVertexAttrib2f(texCoordAttrib, 0.0f, 0.0f);
		glVertexAttrib3f(
				positionAttrib,
				1.0f - boxDim.x,
				1.0f - (boxDim.y * 2 + spaceDim.y),
				0.0f
			);

		glVertexAttrib2f(texCoordAttrib, 0.0f, 1.0f);
		glVertexAttrib3f(
				positionAttrib,
				1.0f - boxDim.x,
				1.0f - boxDim.y - spaceDim.y,
				0.0f
			);

		glVertexAttrib2f(texCoordAttrib, 1.0f, 1.0f);
		glVertexAttrib3f(
				positionAttrib,
				1.0f,
				1.0f - boxDim.y - spaceDim.y,
				0.0f
			);

		glVertexAttrib2f(texCoordAttrib, 1.0f, 0.0f);
		glVertexAttrib3f(
				positionAttrib,
				1.0f,
				1.0f - (boxDim.y * 2 + spaceDim.y),
				0.0f
			);

	glEnd();
}
/*
		}
	}

	// create the texture and load values into it
	Texture alphaTexture(size, size, Texture::FORMAT_RGBA);
	for(int i = 0; i < size; ++i)
		for(int p = 0; p < size; ++p)
			alphaTexture.setColorAt(i, p,
				alphaData[i][p],
				alphaData[i][p],
				alphaData[i][p], 1);

	// create a new combined image
	Texture combinedTexture(size, size, Texture::FORMAT_RGBA);

	for(int i = 0; i < size; ++i)
		for(int p = 0; p < size; ++p)
			combinedTexture.setColorAt(i, p,
					(unsigned short int) ((heightMap.data[i][p] + 1.0f) * 128.0f * alphaData[i][p] / 256.0f),
					(unsigned short int) ((heightMap.data[i][p] + 1.0f) * 128.0f * alphaData[i][p] / 256.0f),
					(unsigned short int) ((heightMap.data[i][p] + 1.0f) * 128.0f * alphaData[i][p] / 256.0f),
					(unsigned short int) ((heightMap.data[i][p] + 1.0f) * 128.0f * alphaData[i][p] / 256.0f));

	// create a new radar-style precision reduced image
	Texture radarTexture(size, size, Texture::FORMAT_RGBA);

	// levels of precision in radar view
	int levels = 16;

	for(int i = 0; i < size; ++i) {
		for(int p = 0; p < size; ++p) {
			float value =
					(float)
					((int)	// precision reduction
					((heightMap.data[i][p]	// the value from the combined image
					+ 1.0f)	// bumped up to positive-only values
					/ 2.0f	// divided back down to 0.0 - 1.0 range
					* levels))	// precision reduction to 0-levels
					/ (float) levels	// back down to float, 0-1.0 range
//					* 256.0f	// final range 0-256.0
					;

			radarTexture.setColorAt(i, p,
					(unsigned short int) (value * alphaData[i][p]),
					(unsigned short int) (value * alphaData[i][p]),
					(unsigned short int) (value * alphaData[i][p]),
						1);
		}
	}

	// load the texture into OpenGL
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			combinedTexture.getWidth(),
			combinedTexture.getHeight(),
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			combinedTexture.getDataPointer()
		);

	// create a mesh with that terrain data
	for(int i = 0; i < size; ++i) {
		for(int p = 0; p < size; ++p) {
			mesh.addVertex(Vector3(
					(float) i / (float) size * 2.0f - 1.0f,
					(float) combinedTexture.getRedValueAt(i, p) / 128.0f - 1.0f,
					(float) p / (float) size * 2.0f - 1.0f
				));

			if(i > 0 && p > 0) {
				mesh.addFace(
						i - 1 + (p * size),
						i - 1 + ((p - 1) * size),
						i + ((p - 1) * size)
					);
				mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);

				mesh.addFace(
						i + ((p - 1) * size),
						i + p * size,
						i - 1 + (p * size)
					);
				mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
			}
		}
	}

printf("facegroup size %i\n", mesh.faceGroups[""].size());
for(int i = 0; i < mesh.faceGroups[""].size(); ++i) {
		printf("face (%.2f %.2f %.2f) (%.2f %.2f %.2f) (%.2f %.2f %.2f)\n",
				mesh.vertices[mesh.faceGroups[""][i].vertices[0]].x,
				mesh.vertices[mesh.faceGroups[""][i].vertices[0]].y,
				mesh.vertices[mesh.faceGroups[""][i].vertices[0]].z,

				mesh.vertices[mesh.faceGroups[""][i].vertices[1]].x,
				mesh.vertices[mesh.faceGroups[""][i].vertices[1]].y,
				mesh.vertices[mesh.faceGroups[""][i].vertices[1]].z,

				mesh.vertices[mesh.faceGroups[""][i].vertices[2]].x,
				mesh.vertices[mesh.faceGroups[""][i].vertices[2]].y,
				mesh.vertices[mesh.faceGroups[""][i].vertices[2]].z
			);
	}

}

TerrainRenderer::~TerrainRenderer() {
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(program);
}
*/
