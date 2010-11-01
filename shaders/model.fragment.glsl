#version 110

uniform sampler2D texture;

varying vec2 texCoordInterpol;
varying float altitude;
//varying vec3 coord;

void main() {
//	gl_FragColor = texture2D(texture, texCoordInterpol);

	float alpha = 1.0;
	if (altitude < 0.05) alpha = (altitude / 0.05);
	gl_FragColor = vec4(vec3(altitude * 0.75 + 0.25), alpha);

//	if(coord.y <= 0.0)
//		discard;
//	else
//		gl_FragColor = vec4(coord, 1.0) / 2.0 + 0.5;
}
