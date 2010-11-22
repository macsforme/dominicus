#version 110

uniform float thickness;

varying vec2 boxCoordInterpol;

void main() {
	float dist = distance(vec2(0.0, 0.0), boxCoordInterpol);
	vec3 color = vec3(1.0);
	float minCoord = min(abs(boxCoordInterpol.x), abs(boxCoordInterpol.y));

	if(minCoord < thickness)
		gl_FragColor = vec4(color, 1.0 - dist);
	else
		discard;
}
