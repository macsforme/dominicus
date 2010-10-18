#version 110

uniform sampler2D fontTexture;

varying vec2 texCoordInterpol;

void main() {
	gl_FragColor = vec4(1.0, 1.0, 1.0, texture2D(fontTexture, texCoordInterpol).w);
}
