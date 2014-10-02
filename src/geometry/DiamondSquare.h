// DiamondSquare.h
// Crucible Island
//
// RIP Sergeant Sean Drenth #6894, Phoenix Police, EOW 10-18-2010.

#ifndef DIAMONDSQUARE_H
#define DIAMONDSQUARE_H

class DiamondSquare {
public:
	unsigned int size;
	float** data;

	DiamondSquare(unsigned int size, float roughness);
	~DiamondSquare();
};

#endif // DIAMONDSQUARE_H
