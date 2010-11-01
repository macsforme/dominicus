#version 110

uniform mat4 mvpMatrix;

attribute vec3 position;
attribute vec2 texCoord;

varying vec2 texCoordInterpol;

void main() {
    gl_Position = mvpMatrix * vec4(position, 1.0);
    texCoordInterpol = texCoord;
}
