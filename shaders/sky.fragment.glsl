#version 110

uniform vec4 waterColor;
uniform vec4 horizonColor;
uniform vec4 baseSkyColor;
uniform vec4 apexColor;

varying vec3 fragCoord;

void main() {
//	float value = asin(dot(vec3(0.0, 1.0, 0.0), normalize(fragCoord))) / asin(1.0);
	float value = dot(vec3(0.0, 1.0, 0.0), normalize(fragCoord));

	if(value < 0.0)
		gl_FragColor = min(vec4(1.0), waterColor);
	else if(value <= 0.01)
		gl_FragColor = min(vec4(1.0), mix(waterColor, horizonColor, value / 0.01));
	else if(value <= 0.25)
		gl_FragColor = min(vec4(1.0), mix(horizonColor, baseSkyColor, (value - 0.01) / 0.249));
	else
		gl_FragColor = min(vec4(1.0), mix(baseSkyColor, apexColor, (value - 0.25) / 0.75));
}
