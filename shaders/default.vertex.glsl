#version 110

// uniforms
uniform mat4 mvpMatrix;

uniform bool useTexture;

uniform bool useColor;

uniform bool useLighting;
uniform vec3 lightPosition;

// attributes
attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec4 color;

// varyings
varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec4 colorInterpol;

varying vec3 lightVectorInterpol;

// main loop
void main() {
	mat3 rotationMatrix = mat3(vec3(mvpMatrix[0]), vec3(mvpMatrix[1]), vec3(mvpMatrix[2]));

	// set the vertex position
    gl_Position = mvpMatrix * vec4(position, 1.0);

	// set varyings
	if(useLighting) {
		normalInterpol = rotationMatrix * normal;
		lightVectorInterpol = lightPosition - position;
	}

	if(useTexture)
		texCoordInterpol = texCoord;

	if(useColor)
		colorInterpol = color;
}
