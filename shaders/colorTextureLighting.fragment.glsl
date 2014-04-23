#version 110

uniform mat4 mvMatrix;
uniform sampler2D texture;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPosition;
uniform float shininess;

varying vec3 positionInterpol;
varying vec3 normalInterpol;
varying vec2 texCoordInterpol;
varying vec4 colorInterpol;

void main() {
	vec4 calculatedColor = colorInterpol;

	calculatedColor *= texture2D(texture, texCoordInterpol);

	vec3 normal = normalize(normalInterpol);
	vec3 lightDirection = normalize(lightPosition);
	vec3 position = normalize(positionInterpol);
	vec3 reflection = reflect(lightDirection, normal);
	vec3 diffuse = max(dot(normal, lightDirection), 0.0) * diffuseColor;

	calculatedColor *= vec4(diffuse + ambientColor, 1.0);

	vec3 specular = pow(max(dot(reflection, position), 0.0), shininess) * specularColor;
	calculatedColor += vec4(specular, 1.0);

	calculatedColor = min(calculatedColor, vec4(1.0));
	gl_FragColor = calculatedColor;
}
