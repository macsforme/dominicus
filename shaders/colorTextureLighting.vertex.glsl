#version 110

uniform mat4 mvpMatrix;
uniform vec3 lightPosition;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec4 color;

varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec4 colorInterpol;
varying vec3 lightVectorInterpol;

void main() {
	mat3 rotationMatrix = mat3(vec3(mvpMatrix[0]), vec3(mvpMatrix[1]), vec3(mvpMatrix[2]));

	gl_Position = mvpMatrix * vec4(position, 1.0);

	texCoordInterpol = texCoord;

	colorInterpol = color;

	normalInterpol = rotationMatrix * normal;
	lightVectorInterpol = lightPosition - position;

}
