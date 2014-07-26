#version 110

uniform vec3 fortressVector;
uniform float progression;

varying vec3 positionInterpol;
varying vec3 normalInterpol;

void main() {
	vec3 white = vec3(0.9, 0.9, 0.9);
	vec3 gray = vec3(0.85, 0.85, 0.85);

	float colorProgression = min(progression + (abs(dot(normalize(fortressVector), normalize(normalInterpol))) - 0.5) * 0.5, 1.0);

	vec3 color = mix(white, gray, smoothstep(0.75, 1.0, colorProgression));

	float alpha =
			abs(dot(normalize(fortressVector), normalize(normalInterpol))) *
			(1.0 - smoothstep(0.5, 1.0, progression));

	gl_FragColor = vec4(color, alpha);
}
