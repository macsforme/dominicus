#version 110

uniform float spotSize;
uniform vec4 color;

varying vec2 boxCoordInterpol;

void main() {
	vec2 absCoord = abs(boxCoordInterpol);
	float cornerBegin = 0.65;
	float squareTriHypotenuse = 1.414213562;

	if(distance(vec2(0.0, 0.0), absCoord) <= spotSize) {
		gl_FragColor = color;
	} else if(
			(absCoord.x > cornerBegin && absCoord.y <= cornerBegin) ||
			(absCoord.y > cornerBegin && absCoord.x <= cornerBegin) ||
			(absCoord.x < cornerBegin && absCoord.y <= cornerBegin)
		) {
		discard;
	} else {
		float distOver = (
				cornerBegin != 1.0 ?
				distance(vec2(cornerBegin), absCoord) / (1.0 - cornerBegin) :
				0.0);

		if(distOver < 1.0)
			discard;
		else
			gl_FragColor = color;
	}
}
