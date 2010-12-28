#version 110

uniform sampler2D texture;

varying vec2 texCoordInterpol;
varying vec3 coord;

void main() {
	float alpha = 1.0;
	if (coord.y < 0.5) alpha = (coord.y / 0.5);

	if(coord.y <= 0.0)
		discard;
	else
		gl_FragColor =
				vec4(texture2D(texture, texCoordInterpol).rgb, alpha) -
				vec4(coord.y, coord.y, coord.y, 0.0) / 25.0;
}
