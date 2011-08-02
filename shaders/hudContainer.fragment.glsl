#version 110

uniform vec4 insideColor;
uniform vec4 borderColor;
uniform vec4 outsideColor;
uniform vec4 highlightColor;
uniform float pixFrac;

varying vec2 primCoordInterpol;
varying vec2 curveOriginCoordInterpol;
varying float border1DistInterpol;
varying float border2DistInterpol;
varying float highlightInterpol;
varying float concaveInterpol;

void main() {
	vec4 trueInColor = (highlightInterpol != 0.0 ? highlightColor : insideColor);
	vec4 trueOutColor = outsideColor;

	if(concaveInterpol != 0.0) {
		vec4 placeHolder = trueInColor;

		trueInColor = trueOutColor;
		trueOutColor = placeHolder;
	}

	// quad sample
	vec4 samples[4];
	vec2 factors[4];
	float halfFrac = pixFrac / 2.0;
	factors[0] = vec2(-pixFrac, -pixFrac);
	factors[1] = vec2(-pixFrac, pixFrac);
	factors[2] = vec2(pixFrac, pixFrac);
	factors[3] = vec2(pixFrac, -pixFrac);

/*
	for(int i = 0; i < 4; ++i) {
		if(distance(curveOriginCoordInterpol, primCoordInterpol + factors[i]) >= border2DistInterpol)
			samples[i] = trueOutColor;
		else if(distance(curveOriginCoordInterpol, primCoordInterpol + factors[i]) >= border1DistInterpol)
			samples[i] = borderColor;
		else
			samples[i] = trueInColor;
	}

	gl_FragColor = (samples[0] + samples[1] + samples[2] + samples[3]) / 4.0;
*/

	if(distance(curveOriginCoordInterpol, primCoordInterpol) >= border2DistInterpol)
		gl_FragColor = trueOutColor;
	else if(distance(curveOriginCoordInterpol, primCoordInterpol) >= border1DistInterpol)
		gl_FragColor = borderColor;
	else
		gl_FragColor = trueInColor;
}
