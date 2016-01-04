#pragma once

#include <math.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void Render();
	void GetViewMatrix(float*);
private:
	struct VectorType
	{
		float x, y, z;
	};
	void MatrixRotationYawPitchRoll(float*, float, float, float);
	void TransformCoord(VectorType&, float*);
	void BuildViewMatrix(VectorType, VectorType, VectorType);

	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	float viewMatrix[16];
	const float degToRad = 0.0174532925f;
};