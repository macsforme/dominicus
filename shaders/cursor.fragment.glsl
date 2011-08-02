#version 110

uniform float thickness;
uniform vec4 color;

varying vec2 boxCoordInterpol;

void main() {
	float dist = distance(vec2(0.0, 0.0), boxCoordInterpol);
	float minCoord = min(abs(boxCoordInterpol.x), abs(boxCoordInterpol.y));

	if(minCoord <= thickness)
		gl_FragColor = vec4(color.rgb, (1.0 - dist) * color.a);
	else
		discard;
}
