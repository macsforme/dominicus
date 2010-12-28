#version 110

uniform mat4 mvpMatrix;

attribute vec3 position;

void main() {
    gl_Position = mvpMatrix * vec4(position, 1.0);
}
