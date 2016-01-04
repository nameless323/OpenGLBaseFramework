#pragma once

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetAmbientLight(float, float, float, float);

	void GetDiffuseColor(float*);
	void GetDirection(float*);
	void GetAmbientLight(float*);
private:
	float _diffuseColor[4];
	float _direction[3];
	float ambientLight[4];
};