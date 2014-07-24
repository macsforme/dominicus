#version 110

uniform mat4 mvMatrix;
uniform mat4 pMatrix;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec4 color;

varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec4 colorInterpol;
varying vec3 trailVector;

void main() {
	gl_Position = pMatrix * mvMatrix * vec4(position, 1.0);

	normalInterpol = (mvMatrix * vec4(normal, 0.0)).xyz;

	texCoordInterpol = texCoord;

	colorInterpol = color;

	trailVector = (mvMatrix * vec4(-1.0, 0.0, 0.0, 0.0)).xyz;
}
