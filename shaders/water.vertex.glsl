#version 110

// uniforms
uniform mat4 mvpMatrix;

// attributes
attribute vec3 position;

// varyings
varying vec2 positionInterpol;

// main function
void main() {
	positionInterpol = vec2(position.x, position.z);

	gl_Position = mvpMatrix * vec4(position, 1.0);
}
