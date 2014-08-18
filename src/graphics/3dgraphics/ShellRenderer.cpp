// MissileRenderer
// Crucible Island

#include "graphics/3dgraphics/ShellRenderer.h"

#include <vector>

#include "core/GameSystem.h"
#include "geometry/Sphere.h"
#include "graphics/GameGraphics.h"
#include "logic/Camera.h"
#include "math/VectorMath.h"
#include "math/MatrixMath.h"
#include "platform/OpenGLHeaders.h"
#include "state/GameState.h"

extern GameGraphics* gameGraphics;
extern GameState* gameState;
extern GameSystem* gameSystem;

ShellRenderer::ShellRenderer() {
	sphere = makeSphere((size_t) gameSystem->getFloat("shellDensity"));

	// set up shader
	GLuint shaderID = 0;
	std::vector<GLuint> shaderIDs;

	shaderID = gameGraphics->getShaderID(GL_VERTEX_SHADER, "colorLighting"); shaderIDs.push_back(shaderID);
	shaderID = gameGraphics->getShaderID(GL_FRAGMENT_SHADER, "colorLighting"); shaderIDs.push_back(shaderID);
	shaderProgram = gameGraphics->makeProgram(shaderIDs);

	// set up uniforms and attributes
	uniforms["mvMatrix"] = glGetUniformLocation(shaderProgram, "mvMatrix");
	uniforms["pMatrix"] = glGetUniformLocation(shaderProgram, "pMatrix");
	uniforms["texture"] = glGetUniformLocation(shaderProgram, "texture");
	uniforms["ambientColor"] = glGetUniformLocation(shaderProgram, "ambientColor");
	uniforms["diffuseColor"] = glGetUniformLocation(shaderProgram, "diffuseColor");
	uniforms["specularColor"] = glGetUniformLocation(shaderProgram, "specularColor");
	uniforms["lightPosition"] = glGetUniformLocation(shaderProgram, "lightPosition");
	uniforms["shininess"] = glGetUniformLocation(shaderProgram, "shininess");

	attributes["position"] = glGetAttribLocation(shaderProgram, "position");
	attributes["normal"] = glGetAttribLocation(shaderProgram, "normal");
	attributes["color"] = glGetAttribLocation(shaderProgram, "color");

	// set up vertex buffers
	glGenBuffers(1, &(vertexBuffers["vertices"]));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	GLfloat* vertDataBufferArray = new GLfloat[sphere.faceGroups[""].size() * 30];	// 3 vertices + 3 normals + 4 colors * 3 faces

	// insert the vertex attribute data
	for(size_t i = 0; i < sphere.faceGroups[""].size(); ++i) {
		for(size_t p = 0; p < 3; ++p) {
			vertDataBufferArray[i * 30 + p * 10 + 0] = sphere.vertices[sphere.faceGroups[""][i].vertices[p]].x;
			vertDataBufferArray[i * 30 + p * 10 + 1] = sphere.vertices[sphere.faceGroups[""][i].vertices[p]].y;
			vertDataBufferArray[i * 30 + p * 10 + 2] = sphere.vertices[sphere.faceGroups[""][i].vertices[p]].z;

			vertDataBufferArray[i * 30 + p * 10 + 3] = sphere.vertices[sphere.faceGroups[""][i].normals[p]].x;
			vertDataBufferArray[i * 30 + p * 10 + 4] = sphere.vertices[sphere.faceGroups[""][i].normals[p]].y;
			vertDataBufferArray[i * 30 + p * 10 + 5] = sphere.vertices[sphere.faceGroups[""][i].normals[p]].z;

			vertDataBufferArray[i * 30 + p * 10 + 6] = 1.0f;
			vertDataBufferArray[i * 30 + p * 10 + 7] = 1.0f;
			vertDataBufferArray[i * 30 + p * 10 + 8] = 1.0f;
			vertDataBufferArray[i * 30 + p * 10 + 9] = 1.0f;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, sphere.faceGroups[""].size() * 30 * sizeof(GLfloat), vertDataBufferArray, GL_STATIC_DRAW);

	delete[] vertDataBufferArray;

	glGenBuffers(1, &(vertexBuffers["elements"]));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	GLuint* vertElementBufferArray = new GLuint[sphere.faceGroups[""].size() * 3];

	for(size_t i = 0; i < sphere.faceGroups[""].size() * 3; ++i) {
		vertElementBufferArray[i] = i;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.faceGroups[""].size() * 3 * sizeof(GLuint), vertElementBufferArray, GL_STATIC_DRAW);

	delete[] vertElementBufferArray;
}

ShellRenderer::~ShellRenderer() {
	// undo shader setup
	GLsizei shaderCount;
	GLuint* shaders = new GLuint[2];
	glGetAttachedShaders(shaderProgram, 2, &shaderCount, shaders);

	for(size_t i = 0; i < shaderCount; ++i) {
		glDetachShader(shaderProgram, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	delete[] shaders;

	glDeleteProgram(shaderProgram);

	glDeleteBuffers(1, &(vertexBuffers["vertices"]));
	glDeleteBuffers(1, &(vertexBuffers["elements"]));
}

void ShellRenderer::execute(std::map<std::string, void*> arguments) {
	// prepare variables
	Vector4 lightPosition(1.0f, 1.0f, -1.0f, 0.0f);
	lightPosition = lightPosition * gameGraphics->currentCamera->lightMatrix;

	// state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	if(gameGraphics->supportsMultisampling) glEnable(GL_MULTISAMPLE);

	// enable shader
	glUseProgram(shaderProgram);

	// set uniforms
	glUniformMatrix4fv(uniforms["pMatrix"], 1, GL_FALSE, (gameState->binoculars ? gameGraphics->ppBinoMatrixArray : gameGraphics->ppMatrixArray));
	glUniform3f(uniforms["ambientColor"], 0.15f, 0.15f, 0.15f);
	glUniform3f(uniforms["diffuseColor"], 0.5f, 0.5f, 0.5f);
	glUniform3f(uniforms["specularColor"], 0.5f, 0.5f, 0.5f);
	glUniform3f(uniforms["lightPosition"], lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform1f(uniforms["shininess"], 50.0f);

	// set the overall drawing state
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers["vertices"]);

	glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*) 0);
	glVertexAttribPointer(attributes["normal"], 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(attributes["color"], 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(attributes["position"]);
	glEnableVertexAttribArray(attributes["normal"]);
	glEnableVertexAttribArray(attributes["color"]);

	// draw the geometry
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers["elements"]);

	for(size_t i = 0; i < gameState->shells.size(); ++i) {
		Matrix4 mvMatrix; mvMatrix.identity();
		scaleMatrix(gameState->shellRadius, gameState->shellRadius, gameState->shellRadius, mvMatrix);
		translateMatrix(gameState->shells[i].position.x, gameState->shells[i].position.y, gameState->shells[i].position.z, mvMatrix);
		mvMatrix = mvMatrix * gameGraphics->currentCamera->mvMatrix;

		float mvMatrixArray[] = {
				mvMatrix.m11, mvMatrix.m12, mvMatrix.m13, mvMatrix.m14,
				mvMatrix.m21, mvMatrix.m22, mvMatrix.m23, mvMatrix.m24,
				mvMatrix.m31, mvMatrix.m32, mvMatrix.m33, mvMatrix.m34,
				mvMatrix.m41, mvMatrix.m42, mvMatrix.m43, mvMatrix.m44
			};

		glUniformMatrix4fv(uniforms["mvMatrix"], 1, GL_FALSE, mvMatrixArray);

		glDrawElements(GL_TRIANGLES, sphere.faceGroups[""].size() * 3, GL_UNSIGNED_INT, NULL);
	}

	glDisableVertexAttribArray(attributes["position"]);
	glDisableVertexAttribArray(attributes["normal"]);
	glDisableVertexAttribArray(attributes["color"]);

	// undo state
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	if(gameGraphics->supportsMultisampling) glDisable(GL_MULTISAMPLE);
}
