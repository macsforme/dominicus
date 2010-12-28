#version 110

uniform float borderSize;

varying vec2 boxCoordInterpol;

void main() {
	if(
			abs(boxCoordInterpol.x) < 1.0 - borderSize &&
			abs(boxCoordInterpol.y) < 1.0 - borderSize
		)
		discard;

	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
