#version 110

attribute vec2 boxCoord;
attribute vec3 position;

varying vec2 boxCoordInterpol;

void main() {
    gl_Position = vec4(position, 1.0);
    boxCoordInterpol = boxCoord;
}
