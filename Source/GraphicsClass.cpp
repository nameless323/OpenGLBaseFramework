#include "stdafx.h"

#include "..\Headers\GraphicsClass.h"
#include <iostream>

GraphicsClass::GraphicsClass() 
{
	openGL = nullptr;
	camera = nullptr;
	model = nullptr;
	_lightShader = nullptr;
	_light = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other) {}

GraphicsClass::~GraphicsClass() {}

bool GraphicsClass::Initialize(OpenGLClass* openGL, HWND hwnd)
{
	bool result;
	this->openGL = openGL;

	camera = new CameraClass;
	if (!camera)
		return false;
	camera->SetPosition(0, 0, -10);

	model = new ModelClass;
	if (!model)
		return false;

	result = model->Initialize(openGL, ".//Models//Cube.txt",".//Textures//stone.tga", 0, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	_lightShader = new LightShaderClass;
	if (!_lightShader)
		return false;
	result = _lightShader->Initialize(openGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;
	}

	_light = new LightClass;
	if (!_light)
		return false;
	_light->SetDiffuseColor(1, 1, 1, 1);
	_light->SetDirection(1, 0, 0);
	_light->SetAmbientLight(0.15f, 0.15f, 0.15f, 1.0f);

	std::cout << "graphics initialized " << std::endl;
	return true;
}

void GraphicsClass::Shutdown()
{
	if (_light != nullptr)
		_light = nullptr;
	if (_lightShader != nullptr)
	{
		_lightShader->Shutdown(openGL);
		delete _lightShader;
		_lightShader = nullptr;
	}
	if (model)
	{
		model->Shutdown(openGL);
		delete model;
		model = nullptr;
	}
	if (camera)
	{
		delete camera;
		camera = nullptr;
	}

	openGL = nullptr;
	return;
}

bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0;
	rotation += 0.0174532925f * 1.0f;
	if (rotation > 360)
		rotation -= 360;
	result = Render(rotation);
	if (!result)
		return false;
	return true;
}

bool GraphicsClass::Render(float rotation)
{
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];
	float lightDirection[3];
	float diffuseLightColor[4];
	float ambientLight[4];

	openGL->BeginScene(0, 0, 0, 1);
	camera->Render();

	openGL->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	openGL->GetProjectionMatrix(projectionMatrix);

	_light->GetDirection(lightDirection);
	_light->GetDiffuseColor(diffuseLightColor);
	_light->GetAmbientLight(ambientLight);
	openGL->MatrixRotationY(worldMatrix, rotation);

	_lightShader->SetShader(openGL);
	_lightShader->SetShaderParameters(openGL, worldMatrix, viewMatrix, projectionMatrix, 0, lightDirection, diffuseLightColor, ambientLight);

	model->Render(openGL);
	

	openGL->EndScene();
	return true;
}