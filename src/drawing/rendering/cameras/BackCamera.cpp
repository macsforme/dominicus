/*
 *  WorldViewCamera.cpp
 *  dominicus
 *
 *  Created by Joshua Bodine on 10/28/10.
 *  Copyright 2010 Joshua Bodine. All rights reserved.
 *
 */

#include "BackCamera.h"

BackCamera::BackCamera() : rotation(0.0f), aspectRatio(gameWindow->aspectRatio) {
	cameraOrientation = ship.orientation;
}

void BackCamera::loop() {
	// the back ship view should trail behind the actual ship orientation and
	// catch up to it a little bit each frame
	const float cFactor = 
			(gamePrefs.getFloat("shipFollowLagFactor") != 0.0f ?
					1.0f / ((float) gamePrefs.getInt("renderingFPS") *
							gamePrefs.getFloat("shipFollowLagFactor"))
					: 1.0f);
	
	cameraOrientation.m11 = cameraOrientation.m11 * (1.0f - cFactor) + ship.orientation.m11 * cFactor;
	cameraOrientation.m12 = cameraOrientation.m12 * (1.0f - cFactor) + ship.orientation.m12 * cFactor;
	cameraOrientation.m13 = cameraOrientation.m13 * (1.0f - cFactor) + ship.orientation.m13 * cFactor;

	cameraOrientation.m21 = cameraOrientation.m21 * (1.0f - cFactor) + ship.orientation.m21 * cFactor;
	cameraOrientation.m22 = cameraOrientation.m22 * (1.0f - cFactor) + ship.orientation.m22 * cFactor;
	cameraOrientation.m23 = cameraOrientation.m23 * (1.0f - cFactor) + ship.orientation.m23 * cFactor;

	cameraOrientation.m31 = cameraOrientation.m31 * (1.0f - cFactor) + ship.orientation.m31 * cFactor;
	cameraOrientation.m32 = cameraOrientation.m32 * (1.0f - cFactor) + ship.orientation.m32 * cFactor;
	cameraOrientation.m33 = cameraOrientation.m33 * (1.0f - cFactor) + ship.orientation.m33 * cFactor;
	
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
	Vector2 point(cameraOrientation.m31, cameraOrientation.m33);
	point.norm();
	float angle = degrees(acos(dot(zeroAngle, point)));
	if(point.x < 0.0f)
		angle = 360.0f - angle;

	// rotate back around +Y so we're always facing forward
	rotateMatrix(Vector3(0.0f, 1.0f, 0.0f), -radians(angle), shipVPMatrix);

	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f),
			radians((float) -gamePrefs.getInt("shipFollowAngle")), shipVPMatrix);
	translateMatrix(0.0f, 0.0f, (float) gamePrefs.getInt("shipFollowDistance"), shipVPMatrix);

	const float fov = gamePrefs.getFloat("perspectiveFOV");
	const float nClip = gamePrefs.getFloat("perspectiveNearClip");
	const float fClip = gamePrefs.getFloat("perspectiveFarClip");

	shipVPMatrix *= Matrix4(
	        1.0/tan(radians(fov)), 0.0, 0.0, 0.0,
	        0.0, aspectRatio/tan(radians(fov)), 0.0, 0.0,
	        0.0, 0.0, (fClip + nClip) / (fClip - nClip), 1.0,
	        0.0, 0.0, -2.0 * fClip * nClip / (fClip - nClip), 0.0
		);

	// terrain camera transformations
	translateMatrix(-ship.position.x, -ship.position.y, -ship.position.z, terrainVPMatrix);

	Vector3 zVec(cameraOrientation.m31, 0.0f, cameraOrientation.m33);
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

	rotateMatrix(Vector3(1.0f, 0.0f, 0.0f), radians((float) -gamePrefs.getInt("shipFollowAngle")), terrainVPMatrix);
	translateMatrix(0.0f, 0.0f, (float) gamePrefs.getInt("shipFollowDistance"), terrainVPMatrix);

	terrainVPMatrix *= Matrix4(
	        1.0/tan(radians(fov)), 0.0, 0.0, 0.0,
	        0.0, aspectRatio/tan(radians(fov)), 0.0, 0.0,
	        0.0, 0.0, (fClip + nClip) / (fClip - nClip), 1.0,
	        0.0, 0.0, -2.0 * fClip * nClip / (fClip - nClip), 0.0
		);
}
