#pragma once
#include <Windows.h>
#include "OpenglClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(OpenGLClass*, HWND);
	void Shutdown();
	bool Frame();
private:
	bool Render(float);
	OpenGLClass* openGL;
	CameraClass* camera;
	ModelClass* model;
	LightShaderClass* _lightShader;
	LightClass* _light;
};