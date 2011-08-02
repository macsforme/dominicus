#version 110

uniform vec2 boxSize;

attribute vec3 position;
attribute vec2 boxCoord;

varying vec2 boxCoordInterpol;

void main() {
    gl_Position = vec4(position.xy * boxSize, position.z, 1.0);
    boxCoordInterpol = boxCoord;
}
