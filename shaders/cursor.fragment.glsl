#version 110

uniform float thickness;

varying vec2 boxCoordInterpol;

void main() {
	gl_FragColor = (
			abs(boxCoordInterpol.x) > thickness && abs(boxCoordInterpol.y) > thickness ?
					vec4(0.0) : vec4(1.0 - distance(vec2(0.0, 0.0), boxCoordInterpol))
	);
			
}
