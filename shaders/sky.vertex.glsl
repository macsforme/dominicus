#version 110

uniform mat4 mvpMatrix;

attribute vec2 position;

varying vec3 fragCoord;

void main() {
	vec4 transformedPosition = mvpMatrix * vec4(position, 1.0, 1.0);
	fragCoord = transformedPosition.xyz;

	gl_Position = vec4(position, 0.0, 1.0);
}
