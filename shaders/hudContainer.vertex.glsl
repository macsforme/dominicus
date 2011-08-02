#version 110

attribute vec2 position;
attribute vec2 primCoord;
attribute vec2 curveOriginCoord;
attribute float border1Dist;
attribute float border2Dist;
attribute float highlight;
attribute float concave;

varying vec2 primCoordInterpol;
varying vec2 curveOriginCoordInterpol;
varying float border1DistInterpol;
varying float border2DistInterpol;
varying float highlightInterpol;
varying float concaveInterpol;

void main() {
	primCoordInterpol = primCoord;
	curveOriginCoordInterpol = curveOriginCoord;
	border1DistInterpol = border1Dist;
	border2DistInterpol = border2Dist;
	highlightInterpol = highlight;
	concaveInterpol = concave;

	gl_Position = vec4(position, 0.0, 1.0);
}
