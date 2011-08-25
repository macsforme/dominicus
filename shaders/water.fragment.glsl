#version 110

// uniforms
uniform float timer;
uniform vec3 cameraPosition;

// varyings
varying vec2 positionInterpol;

// quick hackish noise functions
float badNoise(float val) {
	return fract(sin(val * 18.327) + cos(val / 2.67 - 364.27));
}
vec2 badNoise(vec2 val) {
	return vec2(
			fract(sin(val.x * -6338.181 + val.y * 874.736)),
			fract(sin(val.y * 6338.181 + val.x * -874.736))
		);
}

// main loop
void main() {
	vec2 scaledPosition = positionInterpol / 8.0;

	float noiseFactor =
			mix(badNoise(floor(scaledPosition.x)), badNoise(ceil(scaledPosition.x)), fract(scaledPosition.x));

	float weight = (
			sin(radians(scaledPosition.x + noiseFactor + timer) * 360.0) *
			sin(radians(scaledPosition.x * 0.33 + scaledPosition.y * 0.66 + noiseFactor + timer * 2.0) * 360.0) *
			sin(radians(scaledPosition.x * -0.20 + scaledPosition.y * -0.40 + noiseFactor + timer * -3.0) * 360.0) *
			sin(radians(scaledPosition.x * -0.75 + scaledPosition.y * -0.30 + noiseFactor + timer * -1.0) * 360.0) *
			sin(radians(distance(scaledPosition, vec2(0.0, 0.0)) * 360.0 + timer))
		) * 0.5 + 0.5;

	vec4 crest = vec4(0.8, 0.8, 1.0, 1.0);
	vec4 deep = vec4(0.05, 0.1, 0.3, 1.0);
	vec4 moderate = vec4(0.0, 0.2, 0.4, 1.0);

	vec4 waveColor;

	if(weight > 0.95)
		waveColor = crest;
	else if (weight > 0.9)
		waveColor = mix(moderate, crest, vec4((weight - 0.9) * 15.0));
	else if(weight > 0.3)
		waveColor = mix(deep, moderate, vec4((weight - 0.3) / 0.6));
	else
		waveColor = deep;

	vec2 devX = abs(dFdx(positionInterpol));
	vec2 devY = abs(dFdy(positionInterpol));

	float maxDev = max(max(devX.x, devX.y), max(devY.x, devY.y));

	waveColor = mix(waveColor, deep * 0.5 + moderate * 0.5, min(maxDev / 8.0, 1.0));

//	waveColor.a = min(16.0 / maxDev - 0.2, 1.0);

	gl_FragColor = waveColor;
}
