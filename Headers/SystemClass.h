#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "OpenglClass.h"
#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	virtual ~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MassageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	bool InitializeWindows(OpenGLClass*, int&, int&);
	void ShutdownWindows();
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

	OpenGLClass* openGL;
	InputClass* input;
	GraphicsClass* graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;