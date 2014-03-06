#version 110

uniform mat4 mvpMatrix;
uniform sampler2D texture;
uniform vec3 lightColor;
uniform vec3 specularColor;
uniform float shininess;

varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec4 colorInterpol;
varying vec3 lightVectorInterpol;

void main() {
	vec4 calculatedColor = colorInterpol;

	calculatedColor *= texture2D(texture, texCoordInterpol);

	vec3 lightVectorNorm = normalize(lightVectorInterpol);
	vec3 normalNorm = normalize(normalInterpol);

	float lightFactor = max(dot(lightVectorNorm, normalNorm), 0.0);

	calculatedColor *= vec4(min(vec3(lightFactor) * lightColor, vec3(1.0)), 1.0);

	vec3 refLight = reflect(lightVectorNorm, normalNorm);
	vec3 specularFactor = pow(max(dot(refLight, vec3(0.0, 0.0, 1.0)), 0.0), shininess) * specularColor;

	calculatedColor += vec4(specularFactor, 0.0);
	calculatedColor = min(calculatedColor, vec4(1.0));

	gl_FragColor = calculatedColor;
}
