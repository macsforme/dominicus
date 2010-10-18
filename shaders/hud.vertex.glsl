#version 110

attribute vec2 position;
attribute vec4 color;

varying vec4 colorInterpol;

void main() {
    gl_Position = vec4(position, -0.8, 1.0);
	colorInterpol = color;
}

