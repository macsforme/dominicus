// Camera.h
// Dominicus

#ifndef CAMERA_H
#define CAMERA_H

#include "math/MatrixMath.h"
#include "math/VectorMath.h"

class Camera {
public:
	Vector3 position;
	Matrix4 mvMatrix, lightMatrix;

	virtual void execute() = 0;
};

class OrbitCamera : public Camera {
public:
	void execute();
};

class PresentationCamera : public Camera {
public:
	void execute();
};

class RoamingCamera : public Camera {
public:
	Vector3 position;
	float rotationX, rotationY;

	RoamingCamera() : position(Vector3(0.0f, 5.0f, -2000.0f)), rotationX(0.0f), rotationY(0.0f) {};

	void execute();
};

class IntroCamera: public Camera {
public:
	void execute();
};

class FortressCamera: public Camera {
public:
	void execute();
};

#endif // CAMERA_H
