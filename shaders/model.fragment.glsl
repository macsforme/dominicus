#version 110

uniform float clock;
uniform sampler2D texture;

varying vec2 texCoordInterpol;

void main() {
//	gl_FragColor = vec4(texture2D(texture, texCoordInterpol).rgb, 0.3);
	gl_FragColor = texture2D(texture, texCoordInterpol);

}
