#version 110

attribute vec2 position;
attribute vec2 boxCoord;

varying vec2 boxCoordInterpol;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    boxCoordInterpol = boxCoord;
}
