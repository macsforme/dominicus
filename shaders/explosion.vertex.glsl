#version 110

uniform mat4 mvMatrix;
uniform mat4 pMatrix;

attribute vec3 position;
attribute vec3 normal;

varying vec3 positionInterpol;
varying vec3 normalInterpol;

void main() {
	vec4 eyePosition = mvMatrix * vec4(position, 1.0);
	gl_Position = pMatrix * eyePosition;

	positionInterpol = eyePosition.xyz;
	normalInterpol = (mvMatrix * vec4(normal, 0.0)).xyz;
}
