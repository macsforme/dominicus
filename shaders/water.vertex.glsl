#version 110

// uniforms
uniform mat4 mvpMatrix;
uniform mat4 fortressTransformMatrix;

// attributes
attribute vec3 position;

// varyings
varying vec3 fortressTransformedPosition;
varying vec2 positionInterpol;

// main function
void main() {
	vec4 fortressTransormedPositionUndivided = fortressTransformMatrix * vec4(position, 1.0);
	fortressTransformedPosition = fortressTransormedPositionUndivided.xyz / fortressTransormedPositionUndivided.w;

	positionInterpol = vec2(position.x, position.z);
	gl_Position = mvpMatrix * vec4(position, 1.0);;
}
