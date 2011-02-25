/*
 * ShaderTools.cpp
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

#include "drawing/ShaderTools.h"

GLuint ShaderTools::makeShader(const char* filename, GLenum shaderType) {
	std::ifstream fileStream;
	std::vector<std::string> fileLines;
	fileStream.open(filename);
	while(fileStream.good()) {
		std::string thisChar = "";
		thisChar += fileStream.get();

		if(fileLines.size() == 0)
			fileLines.push_back("");

		fileLines.back() += thisChar;

		if(thisChar == "\n") {
			fileLines.push_back("");
		}
	}
	fileStream.close();
	if(fileLines.size() > 0)
		fileLines.pop_back();
	else
		programLog->report(ProgramLog::LOG_FATAL,
				std::string("The GLSL shader " +
						std::string(filename) +
						" could not be opened for reading.").c_str()
			);

	GLsizei count = fileLines.size();
	uint8_t bytes = 0;
	GLchar** shaderSource;

	for(size_t i = 0; i < fileLines.size(); ++i)
		bytes += fileLines[i].size() + 1;

	shaderSource = (GLchar**) malloc(fileLines.size() * sizeof(size_t));

	for(size_t i = 0; i < fileLines.size(); ++i) {
		shaderSource[i] = (GLchar*) malloc(fileLines[i].size() + 1);
		memcpy(
				shaderSource[i],
				fileLines[i].c_str(),
				fileLines[i].size() + 1
			);
		shaderSource[i][fileLines[i].size()] = '\0';
	}

	GLuint shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, count, (const GLchar**) shaderSource, NULL);

	for(int i = 0; i < count; ++i)
		free(shaderSource[i]);
	free(shaderSource);

	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if(result == GL_FALSE) {
		std::stringstream err;
		err << "The GLSL shader "
				<< filename
				<< " did not compile successfully."
				<< std::endl << std::endl;

		GLint sourceLength;
		glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &sourceLength);
		GLchar* sourceLines = (GLchar*) malloc(sourceLength);
		glGetShaderSource(shader, sourceLength, NULL, sourceLines);

		err << "SHADER SOURCE ON GPU" << std::endl
				<< "--------------------" << std::endl
				<< sourceLines
				<< "--------------------" << std::endl << std::endl;
		free(sourceLines);

		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* logLines = (GLchar*) malloc(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, logLines);

		err << "ERROR LOG" << std::endl
				<< "---------" << std::endl
				<< logLines
				<< "---------";

		free(logLines);

		programLog->report(ProgramLog::LOG_FATAL, err.str().c_str());
	}

	return shader;
}

void ShaderTools::linkProgram(GLuint program) {
	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if(result == GL_FALSE) {
		std::stringstream err;
		err << "The GLSL shader program did not link successfully." << std::endl << std::endl;

		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* logLines = (GLchar*) malloc(logLength);
		glGetProgramInfoLog(program, logLength, NULL, logLines);

		err << "ERROR LOG" << std::endl
				<< "---------" << std::endl
				<< logLines
				<< "---------";

		free(logLines);

		programLog->report(ProgramLog::LOG_FATAL, err.str().c_str());
	}
}

GLuint ShaderTools::makeProgram(std::vector<GLuint> shaders) {
	GLuint program;

	program = glCreateProgram();

	for(size_t i = 0; i < shaders.size(); ++i)
		glAttachShader(program, shaders[i]);

	linkProgram(program);

	return program;
}
