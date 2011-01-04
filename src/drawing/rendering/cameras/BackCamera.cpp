/*
 *  WorldViewCamera.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "BackCamera.h"

void BackCamera::loop() {
	// re-initialize the matrices
	shipVPMatrix.identity();
	terrainVPMatrix.identity();

	// ship camera transformations	
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), radians(180.0f), shipVPMatrix);

	shipVPMatrix *= Matrix4(
			ship.orientation.m11,
			ship.orientation.m12,
			ship.orientation.m13,
			0.0f,
			
			ship.orientation.m21,
			ship.orientation.m22,
			ship.orientation.m23,
			0.0f,

			ship.orientation.m31,
			ship.orientation.m32,
			ship.orientation.m33,
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);

	Vector2 zeroAngle(0.0f, 1.0f);
	Vector2 point(ship.orientation.m31, ship.orientation.m33);
	point.norm();
	float angle = degrees(acos(dot(zeroAngle, point)));
	if(point.x < 0.0f)
		angle = 360.0f - angle;

	// rotate back around +Y so we're always facing forward
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), -radians(angle), shipVPMatrix);

	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(BC_WATCHANGLE), shipVPMatrix);
	translateMatrix(0.0f, 0.0f, BC_BACKOFFDIST, shipVPMatrix);

	shipVPMatrix *= Matrix4(
	        1.0/tan(radians(BC_VIEWANGLE)), 0.0, 0.0, 0.0,
	        0.0, aspectRatio/tan(radians(BC_VIEWANGLE)), 0.0, 0.0,
	        0.0, 0.0, (BC_FARCLIP + BC_NEARCLIP) / (BC_FARCLIP - BC_NEARCLIP), 1.0,
	        0.0, 0.0, -2.0 * BC_FARCLIP * BC_NEARCLIP / (BC_FARCLIP - BC_NEARCLIP), 0.0
		);

	// terrain camera transformations
	translateMatrix(-ship.position.x, -ship.position.y, -ship.position.z, terrainVPMatrix);

	Vector3 zVec(ship.orientation.m31, 0.0f, ship.orientation.m33);
	zVec.norm();

	terrainVPMatrix *= Matrix4(
			zVec.z,
			0.0f,
			zVec.x,
			0.0f,
			
			0.0f,
			1.0f,
			0.0f,
			0.0f,

			-zVec.x,
			0.0f,
			zVec.z,
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);

	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians(BC_WATCHANGLE), terrainVPMatrix);
	translateMatrix(0.0f, 0.0f, BC_BACKOFFDIST, terrainVPMatrix);

	terrainVPMatrix *= Matrix4(
	        1.0/tan(radians(BC_VIEWANGLE)), 0.0, 0.0, 0.0,
	        0.0, aspectRatio/tan(radians(BC_VIEWANGLE)), 0.0, 0.0,
	        0.0, 0.0, (BC_FARCLIP + BC_NEARCLIP) / (BC_FARCLIP - BC_NEARCLIP), 1.0,
	        0.0, 0.0, -2.0 * BC_FARCLIP * BC_NEARCLIP / (BC_FARCLIP - BC_NEARCLIP), 0.0
		);

}
