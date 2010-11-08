#version 110

uniform vec4 insideColor;
uniform vec4 borderColor;
uniform vec4 outsideColor;

uniform vec2 curveOriginCoord;
uniform float border1Dist;
uniform float border2Dist;

varying vec2 primCoordIP;

void main() {
	if(distance(curveOriginCoord, primCoordIP) > border2Dist)
		gl_FragColor = outsideColor;
	else if(distance(curveOriginCoord, primCoordIP) > border1Dist)
		gl_FragColor = borderColor;
	else
		gl_FragColor = insideColor;
}
