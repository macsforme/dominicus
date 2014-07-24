#version 110

uniform mat4 mvMatrix;
uniform sampler2D texture;
uniform float timer;

varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec4 colorInterpol;
varying vec3 trailVector;

void main() {
	vec4 calculatedColor = colorInterpol;
	vec3 normal = normalize(normalInterpol);

	calculatedColor.a = calculatedColor.a * 0.75 + texture2D(texture, abs(texCoordInterpol) + vec2(timer)).r * 0.25;

	calculatedColor.a *= 1.0 - abs(dot(normal, cross(trailVector, vec3(0.0, 0.0, -1.0))));

	calculatedColor.a = pow(calculatedColor.a, 2.0);

	gl_FragColor = calculatedColor;
}
