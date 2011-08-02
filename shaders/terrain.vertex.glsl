#version 110

// uniforms
uniform mat4 mvpMatrix;
uniform vec3 lightPosition;

// attributes
attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

// varyings
varying float yCoordInterpol;
varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec2 rotTexCoordInterpol;
varying vec3 lightVectorInterpol;

// main loop
void main() {
    gl_Position = mvpMatrix * vec4(position, 1.0);

	yCoordInterpol = position.y;

	mat3 rotationMatrix = mat3(vec3(mvpMatrix[0]), vec3(mvpMatrix[1]), vec3(mvpMatrix[2]));
	normalInterpol = /*rotationMatrix **/ normal;
	lightVectorInterpol = vec3(0.0, 1.0, 0.0)/*lightPosition*/ /*- position*/;

	texCoordInterpol = texCoord;
	mat2 texTransform = mat2(cos(radians(45.0)), -sin(radians(45.0)), sin(radians(45.0)), cos(radians(45.0)));
	rotTexCoordInterpol = texCoord * texTransform;
}
