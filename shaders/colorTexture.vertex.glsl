#version 110

uniform mat4 mvpMatrix;

attribute vec3 position;
attribute vec2 texCoord;
attribute vec4 color;

varying vec2 texCoordInterpol;
varying vec4 colorInterpol;

void main() {
	gl_Position = mvpMatrix * vec4(position, 1.0);

	texCoordInterpol = texCoord;
	colorInterpol = color;
}
