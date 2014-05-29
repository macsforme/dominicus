#version 110

uniform mat4 mvMatrix;
uniform mat4 pMatrix;

attribute vec3 position;
attribute vec3 normal;
attribute vec4 color;

varying vec3 positionInterpol;
varying vec3 normalInterpol;
varying vec4 colorInterpol;

void main() {
	vec4 eyePosition = mvMatrix * vec4(position, 1.0);
	gl_Position = pMatrix * eyePosition;

	positionInterpol = eyePosition.xyz;
	normalInterpol = (mvMatrix * vec4(normal, 0.0)).xyz;

	colorInterpol = color;
}
