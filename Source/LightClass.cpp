#include "stdafx.h"
#include "../Headers/LightClass.h"

LightClass::LightClass() {}

LightClass::LightClass(const LightClass& other) {}

LightClass::~LightClass() {}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	_diffuseColor[0] = r;
	_diffuseColor[1] = g;
	_diffuseColor[2] = b;
	_diffuseColor[3] = a;
}

void LightClass::SetDirection(float x, float y, float z)
{
	_direction[0] = x;
	_direction[1] = y;
	_direction[2] = z;
}

void LightClass::GetDirection(float* direction)
{
	direction[0] = _direction[0];
	direction[1] = _direction[1];
	direction[2] = _direction[2];
}

void LightClass::SetAmbientLight(float r, float g, float b, float a)
{
	ambientLight[0] = r;
	ambientLight[1] = g;
	ambientLight[2] = b;
	ambientLight[3] = a;
}

void LightClass::GetDiffuseColor(float* color)
{
	color[0] = _diffuseColor[0];
	color[1] = _diffuseColor[1];
	color[2] = _diffuseColor[2];
	color[3] = _diffuseColor[3];
}

void LightClass::GetAmbientLight(float* ambient)
{
	ambient[0] = ambientLight[0];
	ambient[1] = ambientLight[1];
	ambient[2] = ambientLight[2];
	ambient[3] = ambientLight[3];
}