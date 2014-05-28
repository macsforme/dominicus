#version 110

// uniforms
uniform mat4 mvpMatrix;
uniform mat4 towerTransformMatrix;

// attributes
attribute vec3 position;

// varyings
varying vec3 towerTransformedPosition;
varying vec2 positionInterpol;

// main function
void main() {
	vec4 towerTransormedPositionUndivided = towerTransformMatrix * vec4(position, 1.0);
	towerTransformedPosition = towerTransormedPositionUndivided.xyz / towerTransormedPositionUndivided.w;

	positionInterpol = vec2(position.x, position.z);
	gl_Position = mvpMatrix * vec4(position, 1.0);;
}
