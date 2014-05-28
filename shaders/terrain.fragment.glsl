#version 110

// uniforms
uniform sampler2D textures[4];
uniform vec3 lightColor;
uniform float depth;
uniform vec4 insideColorMultiplier;
uniform vec4 outsideColorMultiplier;
uniform float colorChangeRadius;

// varyings
varying vec3 towerTransformedPosition;
varying float yCoordInterpol;
varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec2 rotTexCoordInterpol;
varying vec3 lightVectorInterpol;

// main loop
void main() {
	// initial color
	vec4 calculatedColor = vec4(1.0, 1.0, 1.0, 1.0);

	// fade the underground portion
	if(yCoordInterpol < 0.0)
		calculatedColor.a *= 0.75 * (depth + yCoordInterpol) / depth;

	// texture it
	calculatedColor *= mix(
			texture2D(textures[1], texCoordInterpol),
			texture2D(textures[2], texCoordInterpol),
			texture2D(textures[3], texCoordInterpol / 10.0).x
		);

	vec3 lightVectorNorm = normalize(lightVectorInterpol);
	vec3 normalNorm = normalize(normalInterpol);

	float lightFactor = pow(max(dot(lightVectorNorm, normalNorm), 0.0), 3.0);
	lightFactor = max(0.5, lightFactor);
	calculatedColor *= vec4(min(vec3(lightFactor) * vec3(1.0, 1.0, 1.0), vec3(1.0)), 1.0);

	// color change for EMP
	if(yCoordInterpol >= 0.0) {
		if(distance(vec3(0.0), towerTransformedPosition) > colorChangeRadius)
			calculatedColor *= outsideColorMultiplier;
		else
			calculatedColor *= insideColorMultiplier;

		calculatedColor = min(vec4(1.0), calculatedColor);
	}

	// final setting
	gl_FragColor = calculatedColor;
}
