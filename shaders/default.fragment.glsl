#version 110

uniform sampler2D texture;

varying vec2 texCoordInterpol;

void main() {
	gl_FragColor = texture2D(texture, texCoordInterpol);
}
