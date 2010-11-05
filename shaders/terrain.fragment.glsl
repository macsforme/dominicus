#version 110

uniform sampler2D texture;

varying vec2 texCoordInterpol;
varying vec3 coord;

void main() {
//	gl_FragColor = texture2D(texture, texCoordInterpol);

//	float alpha = 1.0;
//	if (coord.y < 0.05) alpha = (coord.y / 0.05);
//	gl_FragColor = vec4(vec3(coord.y * 0.75 + 0.25), alpha);

//	if(coord.y <= 0.0)
//		discard;
//	else
//		gl_FragColor = vec4(coord, 1.0) / 2.0 + 0.5;

	gl_FragColor = vec4(coord, 1.0);
}
