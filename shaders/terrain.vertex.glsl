#version 110

// uniforms
uniform mat4 mvMatrix;
uniform mat4 pMatrix;
uniform mat4 fortressTransformMatrix;

// attributes
attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

// varyings
varying vec3 fortressTransformedPosition;
varying float yCoordInterpol;
varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec2 rotTexCoordInterpol;
varying vec3 lightVectorInterpol;

// main loop
void main() {
	mat4 mvpMatrix = pMatrix * mvMatrix;
	gl_Position = mvpMatrix * vec4(position, 1.0);

	vec4 fortressTransormedPositionUndivided = fortressTransformMatrix * vec4(position, 1.0);
	fortressTransformedPosition = fortressTransormedPositionUndivided.xyz / fortressTransormedPositionUndivided.w;
	yCoordInterpol = position.y;

	mat3 rotationMatrix = mat3(vec3(mvpMatrix[0]), vec3(mvpMatrix[1]), vec3(mvpMatrix[2]));
	normalInterpol = normal;
	lightVectorInterpol = vec3(0.0, 1.0, 0.0);

	texCoordInterpol = texCoord;
	mat2 texTransform = mat2(cos(radians(45.0)), -sin(radians(45.0)), sin(radians(45.0)), cos(radians(45.0)));
	rotTexCoordInterpol = texCoord * texTransform;
}
