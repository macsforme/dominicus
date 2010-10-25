#version 110

uniform float clock;
uniform mat4 MVPMatrix;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec2 texCoordInterpol;

void main() {
	vec4 fullPosition = vec4(position, 1.0);

    gl_Position = fullPosition * MVPMatrix;
	gl_Position.x += normal.x;
	gl_Position.x -= normal.x;
    texCoordInterpol = texCoord;
}
