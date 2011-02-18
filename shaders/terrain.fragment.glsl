#version 110

uniform sampler2D textures[4];
uniform sampler2D noiseTexture;

uniform float totalWidth;
uniform float totalHeight;

varying vec2 texCoordInterpol;
varying vec3 coord;

void main() {
	float alpha = 1.0;
	if (coord.y < 2.0) alpha = (coord.y / 2.0);

	if(coord.y <= 0.0)
		discard;
	else
		gl_FragColor =
				vec4(
						mix(
								mix(
										texture2D(textures[0], texCoordInterpol).rgb,
										texture2D(textures[1], texCoordInterpol).rgb,
										texture2D(noiseTexture, coord.xz / totalWidth).rgb
									),
								mix(
										texture2D(textures[2], texCoordInterpol).rgb,
										texture2D(textures[3], texCoordInterpol).rgb,
										texture2D(noiseTexture, coord.xz / totalWidth).rgb
									),
								coord.y / totalHeight
							),
					alpha);

// -
//				vec4(coord.y, coord.y, coord.y, 0.0) / (totalHeight * 2.0);
}
