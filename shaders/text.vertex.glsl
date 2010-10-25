#version 110

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 texCoordInterpol;

void main() {
    gl_Position = vec4(position, -1.0, 1.0);
	texCoordInterpol = texCoord;
}
