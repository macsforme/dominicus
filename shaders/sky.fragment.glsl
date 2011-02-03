#version 120

uniform vec3 horizonColor;
uniform vec3 skyColor;

varying float degreesInterpol;

void main() {
	if(degreesInterpol < 0.0)
		discard;
	else
		gl_FragColor = vec4(mix(horizonColor, skyColor, vec3(abs(degreesInterpol))), 1.0);
}
