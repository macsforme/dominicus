#version 110

uniform float zMod;

attribute vec2 position;
attribute vec2 primCoord;

varying vec2 primCoordIP;

void main() {
	primCoordIP = primCoord;

	gl_Position = vec4(position, 0.0 + zMod, 1.0);
}
