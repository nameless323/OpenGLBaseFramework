#include "stdafx.h"
#include "../Headers/CameraClass.h"

CameraClass::CameraClass()
{
	positionX = 0;
	positionY = 0;
	positionZ = 0;

	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;
}

CameraClass::CameraClass(const CameraClass& other) {}

CameraClass::~CameraClass() {}

void CameraClass::SetPosition(float x, float y, float z)
{
	positionX = x;
	positionY = y;
	positionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	rotationX = x;
	rotationY = y;
	rotationZ = z;
}

void CameraClass::Render()
{
	VectorType up, position, lookAt;
	float yaw, pitch, roll;
	float rotationMatrix[9];

	up.x = 0;
	up.y = 1;
	up.z = 0;

	position.x = positionX;
	position.y = positionY;
	position.z = positionZ;

	lookAt.x = 0;
	lookAt.y = 0;
	lookAt.z = 1;

	pitch = rotationX * degToRad;
	yaw = rotationY * degToRad;
	roll = rotationZ * degToRad;

	MatrixRotationYawPitchRoll(rotationMatrix, yaw, pitch, roll);

	TransformCoord(lookAt, rotationMatrix);
	TransformCoord(up, rotationMatrix);

	lookAt.x = position.x + lookAt.x;
	lookAt.y = position.y + lookAt.y;
	lookAt.z = position.z + lookAt.z;

	BuildViewMatrix(position, lookAt, up);
}

void CameraClass::MatrixRotationYawPitchRoll(float* matrix, float yaw, float pitch, float roll)
{
	float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;

	cYaw = cosf(yaw);
	cPitch = cosf(pitch);
	cRoll = cosf(roll);

	sYaw = sinf(yaw);
	sPitch = sinf(pitch);
	sRoll = sinf(roll);

	matrix[0] = (cRoll * cYaw) + (sRoll * sPitch * sYaw);
	matrix[1] = (sRoll * cPitch);
	matrix[2] = (cRoll * -sYaw) + (sRoll * sPitch * cYaw);

	matrix[3] = (-sRoll * cYaw) + (cRoll * sPitch * sYaw);
	matrix[4] = (cRoll * cPitch);
	matrix[5] = (sRoll * sYaw) + (cRoll * sPitch * cYaw);

	matrix[6] = (cPitch * sYaw);
	matrix[7] = -sPitch;
	matrix[8] = (cPitch * cYaw);
}

void CameraClass::TransformCoord(VectorType& vector, float* matrix)
{
	float x, y, z;
	x = (vector.x * matrix[0]) + (vector.y * matrix[3]) + (vector.z * matrix[6]);
	y = (vector.x * matrix[1]) + (vector.y * matrix[4]) + (vector.z * matrix[7]);
	z = (vector.x * matrix[2]) + (vector.y * matrix[5]) + (vector.z * matrix[8]);
	
	vector.x = x;
	vector.y = y;
	vector.z = z;
}

void CameraClass::BuildViewMatrix(VectorType position, VectorType lookAt, VectorType up)
{
	VectorType zAxis, xAxis, yAxis;
	float length, result1, result2, result3;

	zAxis.x = lookAt.x - position.x;
	zAxis.y = lookAt.y - position.y;
	zAxis.z = lookAt.z - position.z;

	length = sqrt((zAxis.x * zAxis.x) + (zAxis.y * zAxis.y) + (zAxis.z * zAxis.z));
	zAxis.x = zAxis.x / length;
	zAxis.y = zAxis.y / length;
	zAxis.z = zAxis.z / length;

	xAxis.x = (up.y * zAxis.z) - (up.z * zAxis.y);
	xAxis.y = (up.z * zAxis.x) - (up.x * zAxis.z);
	xAxis.z = (up.x * zAxis.y) - (up.y * zAxis.x);
	length = sqrt((xAxis.x * xAxis.x) + (xAxis.y * xAxis.y) + (zAxis.z * zAxis.z));
	xAxis.x = xAxis.x / length;
	xAxis.y = xAxis.y / length;
	xAxis.z = xAxis.z / length;


	yAxis.x = (zAxis.y * xAxis.z) - (zAxis.z * xAxis.y);
	yAxis.y = (zAxis.z * xAxis.x) - (zAxis.x * xAxis.z);
	yAxis.z = (zAxis.x * xAxis.y) - (zAxis.y * xAxis.x);

	result1 = ((xAxis.x * position.x) + (xAxis.y * position.y) + (xAxis.z * position.z)) * -1;
	result2 = ((yAxis.x * position.x) + (yAxis.y * position.y) + (yAxis.z * position.z)) * -1;
	result3 = ((zAxis.x * position.x) + (zAxis.y * position.y) + (zAxis.z * position.z)) * -1;

	viewMatrix[0] = xAxis.x;
	viewMatrix[1] = yAxis.x;
	viewMatrix[2] = zAxis.x;
	viewMatrix[3] = 0;

	viewMatrix[4] = xAxis.y;
	viewMatrix[5] = yAxis.y;
	viewMatrix[6] = zAxis.y;
	viewMatrix[7] = 0;

	viewMatrix[8] = xAxis.z;
	viewMatrix[9] = yAxis.z;
	viewMatrix[10] = zAxis.z;
	viewMatrix[11] = 0;

	viewMatrix[12] = result1;
	viewMatrix[13] = result2;
	viewMatrix[14] = result3;
	viewMatrix[15] = 1;
}

void CameraClass::GetViewMatrix(float* matrix)
{
	matrix[0] = viewMatrix[0];
	matrix[1] = viewMatrix[1];
	matrix[2] = viewMatrix[2];
	matrix[3] = viewMatrix[3];
	matrix[4] = viewMatrix[4];
	matrix[5] = viewMatrix[5];
	matrix[6] = viewMatrix[6];
	matrix[7] = viewMatrix[7];
	matrix[8] = viewMatrix[8];
	matrix[9] = viewMatrix[9];
	matrix[10] = viewMatrix[10];
	matrix[11] = viewMatrix[11];
	matrix[12] = viewMatrix[12];
	matrix[13] = viewMatrix[13];
	matrix[14] = viewMatrix[14];
	matrix[15] = viewMatrix[15];
}