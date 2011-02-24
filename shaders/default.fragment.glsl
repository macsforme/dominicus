#version 110

// uniforms
uniform mat4 mvpMatrix;

uniform bool useTexture;
uniform sampler2D texture;

uniform bool useLighting;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform float shininess;
uniform vec3 specularColor;

// varyings
varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec4 colorInterpol;

varying vec3 lightVectorInterpol;
varying vec3 viewVec;

// main loop
void main() {
	vec4 calculatedColor = colorInterpol;
	vec3 lightVectorNorm = normalize(lightVectorInterpol);
	vec3 normalNorm = normalize(normalInterpol);

	float lightFactor = max(dot(lightVectorNorm, normalNorm), 0.0);

	if(useTexture)
		calculatedColor *= texture2D(texture, texCoordInterpol);

	if(useLighting) {
		calculatedColor *= vec4(min(vec3(lightFactor) * lightColor + ambientColor, vec3(1.0)), 1.0);

		vec3 refLight = reflect(lightVectorNorm, normalNorm);
		vec3 specularFactor = pow(max(dot(refLight, vec3(0.0, 0.0, 1.0)), 0.0), shininess) * specularColor;

		calculatedColor += vec4(specularFactor, 0.0);
		calculatedColor = min(calculatedColor, vec4(1.0));
	}

	gl_FragColor = calculatedColor;
}
