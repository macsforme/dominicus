/*
 *  TerrainRenderer.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer() {
	// set up the shader program
	vertexShader = ShaderTools::makeShader(
			std::string(platform.dataPath +  "/shaders/terrain.vertex.glsl").c_str(),
			GL_VERTEX_SHADER
		);
	fragmentShader = ShaderTools::makeShader(
			std::string(platform.dataPath + "/shaders/terrain.fragment.glsl").c_str(),
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

/*
	// load the texture into OpenGL
	std::string textureFile = platform.dataPath + std::string("/data/textures/terrain/dirt.bmp");
	BMPImage image(textureFile);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(image.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGBA),
			image.getWidth(),
			image.getHeight(),
			0,
			(image.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGBA),
			GL_UNSIGNED_BYTE,
			image.getDataPointer()
	);
*/

	// OpenGL state
	glEnable(GL_TEXTURE_2D);

	// create a mesh with that terrain data
	for(int i = 0; i < terrain.density; ++i) {
		for(int p = 0; p < terrain.density; ++p) {
			mesh.addVertex(Vector3(
					(float) i / (float) terrain.density * 2.0f - 1.0f,
					((float) terrain.finalHeightMap[i][p] / 256.0f),
					(float) p / (float) terrain.density * 2.0f - 1.0f
				));

			if(i > 0 && p > 0) {
				mesh.addFace(
						i - 1 + (p * terrain.density),
						i - 1 + ((p - 1) * terrain.density),
						i + ((p - 1) * terrain.density)
					);
				mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);

				mesh.addFace(
						i + ((p - 1) * terrain.density),
						i + p * terrain.density,
						i - 1 + (p * terrain.density)
					);
				mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
			}
		}
	}

/*
mesh = Mesh();

mesh.addVertex(Vector3(-1.0f, -1.0f, -1.0f));
mesh.addVertex(Vector3(-1.0f, 1.0f, -1.0f));
mesh.addVertex(Vector3(1.0f, 1.0f, -1.0f));
mesh.addVertex(Vector3(1.0f, -1.0f, -1.0f));

mesh.addVertex(Vector3(-1.0f, -1.0f, 1.0f));
mesh.addVertex(Vector3(-1.0f, 1.0f, 1.0f));
mesh.addVertex(Vector3(1.0f, 1.0f, 1.0f));
mesh.addVertex(Vector3(1.0f, -1.0f, 1.0f));

mesh.addFace(0, 1, 2);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
mesh.addFace(2, 3, 0);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);

mesh.addFace(6, 5, 4);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
mesh.addFace(4, 7, 6);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);

mesh.addFace(4, 5, 1);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
mesh.addFace(1, 0, 4);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);

mesh.addFace(3, 2, 6);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
mesh.addFace(6, 7, 3);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);

mesh.addFace(1, 5, 6);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
mesh.addFace(6, 2, 1);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);

mesh.addFace(4, 0, 3);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
mesh.addFace(3, 7, 4);
mesh.autoTexCoord(mesh.faceGroups[""].size() - 1);
*/
}

TerrainRenderer::~TerrainRenderer() {
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(program);
}

void TerrainRenderer::render(Matrix4 vpMatrix) {
//	terrain.render();

	glUseProgram(program);

	// model view projection
	// pass in the row-major matrix since GL does the multiplication backwards
	float vpMatrixArray[] = {
			vpMatrix.m11, vpMatrix.m12, vpMatrix.m13, vpMatrix.m14,
			vpMatrix.m21, vpMatrix.m22, vpMatrix.m23, vpMatrix.m24,
			vpMatrix.m31, vpMatrix.m32, vpMatrix.m33, vpMatrix.m34,
			vpMatrix.m41, vpMatrix.m42, vpMatrix.m43, vpMatrix.m44
		};

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, vpMatrixArray);

	// textures
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureUniform, 0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// geometry
	for(
			std::map< std::string,std::vector<Mesh::Face> >::iterator itr =
					mesh.faceGroups.begin();
			itr != mesh.faceGroups.end();
			++itr
		) {
		for(int i = 0; i < itr->second.size(); ++i) {
			// render triangles
			glBegin(GL_TRIANGLES);

			glVertexAttrib2f(
					texCoordAttrib,
					mesh.texCoords[itr->second[i].texCoords[0]].x,
					mesh.texCoords[itr->second[i].texCoords[0]].y
				);
			glVertexAttrib3f(
					positionAttrib,
					mesh.vertices[itr->second[i].vertices[0]].x,
					mesh.vertices[itr->second[i].vertices[0]].y,
					mesh.vertices[itr->second[i].vertices[0]].z
				);

			glVertexAttrib2f(
					texCoordAttrib,
					mesh.texCoords[itr->second[i].texCoords[1]].x,
					mesh.texCoords[itr->second[i].texCoords[1]].y
				);
			glVertexAttrib3f(
					positionAttrib,
					mesh.vertices[itr->second[i].vertices[1]].x,
					mesh.vertices[itr->second[i].vertices[1]].y,
					mesh.vertices[itr->second[i].vertices[1]].z
				);

			glVertexAttrib2f(
					texCoordAttrib,
					mesh.texCoords[itr->second[i].texCoords[2]].x,
					mesh.texCoords[itr->second[i].texCoords[2]].y
				);
			glVertexAttrib3f(
					positionAttrib,
					mesh.vertices[itr->second[i].vertices[2]].x,
					mesh.vertices[itr->second[i].vertices[2]].y,
					mesh.vertices[itr->second[i].vertices[2]].z
				);

			glEnd();
		}
	}
}
