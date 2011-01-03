#version 110

uniform float spotSize;

varying vec2 boxCoordInterpol;

void main() {
	if(distance(vec2(0.0, 0.0), boxCoordInterpol) <= spotSize)
		gl_FragColor = vec4(vec3(0.3), 0.1);
	else if(
			abs(boxCoordInterpol.x) < 0.75 &&
			abs(boxCoordInterpol.y) < 0.75
		)
		discard;
	else
		gl_FragColor = vec4(
				vec3(0.3),
				(max(abs(boxCoordInterpol.x), abs(boxCoordInterpol.y)) * 4.0 - 3.0) *
						0.1
			);
}
