#version 120

uniform mat4 mvpMatrix;

attribute vec3 position;
attribute float degrees;

varying float degreesInterpol;

void main() {
	gl_Position = vec4(position, 1.0);

	degreesInterpol = degrees;
}
