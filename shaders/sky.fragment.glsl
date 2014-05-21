#version 110

uniform vec4 topColor;
uniform vec4 horizonColor;

varying vec3 fragCoord;

void main() {
//	float value = asin(dot(vec3(0.0, 1.0, 0.0), normalize(fragCoord))) / asin(1.0);
	float value = dot(vec3(0.0, 1.0, 0.0), normalize(fragCoord));

	vec4 apexColor = vec4(0.18, 0.54, 0.90, 1.0);
	vec4 horizonColor = vec4(0.76, 0.88, 1.0, 1.0);

	if(value < 0.0)
		gl_FragColor = vec4(0.025, 0.05, 0.15, 1.0);
	else
		gl_FragColor = mix(horizonColor, apexColor, value);
}
