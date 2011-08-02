#version 110

// uniforms
uniform mat4 mvpMatrix;
uniform sampler2D textures[4];
uniform vec3 lightColor;
uniform float depth;
uniform float height;

// varyings
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
		calculatedColor.a *= (depth + yCoordInterpol) / depth;

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
	calculatedColor *= vec4(min(vec3(lightFactor) * /*lightColor*/ vec3(1.0, 1.0, 1.0), vec3(1.0)), 1.0);

	// final setting
	gl_FragColor = calculatedColor;
}
