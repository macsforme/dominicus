#version 110

uniform vec4 topColor;
uniform vec4 horizonColor;

varying vec3 fragCoord;

void main() {
//	float value = asin(dot(vec3(0.0, 1.0, 0.0), normalize(fragCoord))) / asin(1.0);
	float value = dot(vec3(0.0, 1.0, 0.0), normalize(fragCoord));

	vec4 waterColor = vec4(0.025, 0.05, 0.15, 1.0);
	vec4 horizonColor = vec4(1.0, 0.88, 0.64, 1.0);
	vec4 baseSkyColor = vec4(0.76, 0.88, 1.0, 1.0);
	vec4 apexColor = vec4(0.12, 0.36, 0.60, 1.0);

	if(value < 0.0)
		gl_FragColor = waterColor;
	else if (value <= 0.01)
		gl_FragColor = mix(waterColor, horizonColor, value / 0.01);
	else if (value <= 0.25)
		gl_FragColor = mix(horizonColor, baseSkyColor, (value - 0.01) / 0.249);
	else
		gl_FragColor = mix(baseSkyColor, apexColor, (value - 0.25) / 0.75);
}
