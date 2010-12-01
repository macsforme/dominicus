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

	// set attribute locations
	positionAttrib = 0;
	texCoordAttrib = 1;
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "texCoord");

	ShaderTools::linkProgram(program);

	// get uniform locations
	mvpMatrixUniform = glGetUniformLocation(program, "mvpMatrix");
	textureUniform = glGetUniformLocation(program, "texture");

	// load the texture into OpenGL
	std::string textureFile = platform.dataPath + std::string("/data/textures/terrain/dirt.bmp");
	BMPImage image(textureFile);

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	float maxAniso;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(image.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			image.getWidth(),
			image.getHeight(),
			0,
			(image.getFormat() == Texture::FORMAT_RGBA ? GL_RGBA : GL_RGB),
			GL_UNSIGNED_BYTE,
			image.getDataPointer()
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_2D);
}

TerrainRenderer::~TerrainRenderer() {
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(program);
}

void TerrainRenderer::render(Matrix4 vpMatrix) {
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
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureUniform, 0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// geometry
	float texDivisor = 10.0f;

	for(
			std::map< std::string,std::vector<Mesh::Face> >::iterator itr =
					terrain.mesh.faceGroups.begin();
			itr != terrain.mesh.faceGroups.end();
			++itr
		) {
		for(int i = 0; i < itr->second.size(); ++i) {
			// render triangles
			glBegin(GL_TRIANGLES);

			glVertexAttrib2f(
					texCoordAttrib,
					terrain.mesh.vertices[itr->second[i].vertices[0]].x / texDivisor,
					terrain.mesh.vertices[itr->second[i].vertices[0]].z / texDivisor
				);
			glVertexAttrib3f(
					positionAttrib,
					terrain.mesh.vertices[itr->second[i].vertices[0]].x,
					terrain.mesh.vertices[itr->second[i].vertices[0]].y,
					terrain.mesh.vertices[itr->second[i].vertices[0]].z
				);

			glVertexAttrib2f(
					texCoordAttrib,
					terrain.mesh.vertices[itr->second[i].vertices[1]].x / texDivisor,
					terrain.mesh.vertices[itr->second[i].vertices[1]].z / texDivisor
				);
			glVertexAttrib3f(
					positionAttrib,
					terrain.mesh.vertices[itr->second[i].vertices[1]].x,
					terrain.mesh.vertices[itr->second[i].vertices[1]].y,
					terrain.mesh.vertices[itr->second[i].vertices[1]].z
				);

			glVertexAttrib2f(
					texCoordAttrib,
					terrain.mesh.vertices[itr->second[i].vertices[2]].x / texDivisor,
					terrain.mesh.vertices[itr->second[i].vertices[2]].z / texDivisor
				);
			glVertexAttrib3f(
					positionAttrib,
					terrain.mesh.vertices[itr->second[i].vertices[2]].x,
					terrain.mesh.vertices[itr->second[i].vertices[2]].y,
					terrain.mesh.vertices[itr->second[i].vertices[2]].z
				);

			glEnd();
		}
	}

//	glDisable(GL_TEXTURE_2D);

}
