#version 110

uniform sampler2D texture;

varying vec2 texCoordInterpol;
varying vec4 colorInterpol;

void main() {
	vec4 calculatedColor = colorInterpol;

	calculatedColor *= texture2D(texture, texCoordInterpol);

	gl_FragColor = calculatedColor;
}
