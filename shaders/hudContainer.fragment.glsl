#version 110

uniform vec4 insideColor;
uniform vec4 borderColor;
uniform vec4 outsideColor;
uniform float softEdge;

varying vec2 primCoordInterpol;
varying vec2 curveOriginCoordInterpol;
varying float border1DistInterpol;
varying float border2DistInterpol;

void main() {
	float fragDistance = distance(curveOriginCoordInterpol, primCoordInterpol);
	if(fragDistance > border1DistInterpol)
		gl_FragColor = mix(
				borderColor,
				outsideColor,
				smoothstep(
						border2DistInterpol - softEdge,
						border2DistInterpol,
						fragDistance
					)
			);
	else
		gl_FragColor = mix(
				insideColor,
				borderColor,
				smoothstep(
						border1DistInterpol - softEdge,
						border1DistInterpol,
						fragDistance
					)
			);
}
