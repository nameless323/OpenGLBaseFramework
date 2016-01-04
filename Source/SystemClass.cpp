#include "stdafx.h"
#include "../Headers/SystemClass.h"
#include <iostream>

SystemClass::SystemClass()
{
	openGL = nullptr;
	input = nullptr;
	graphics = nullptr;
}

SystemClass::SystemClass(const SystemClass& other) {}

SystemClass::~SystemClass() {};

bool SystemClass::Initialize()
{
	int screenWidth = 0;
	int screenHeight = 0;
	bool result = false;

	openGL = new OpenGLClass;
	if (!openGL)
		return false;

	result = InitializeWindows(openGL, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the window", L"Effor", MB_OK);
		return false;
	}
	
	input = new InputClass;
	if (!input)
		return false;
	input->Initialize();
	
	graphics = new GraphicsClass;
	if (!graphics)
		return false;

	result = graphics->Initialize(openGL, hwnd);
	if (!result)
		return false;
	return true;
}

void SystemClass::Shutdown()
{
	if (graphics)
	{
		graphics->Shutdown();
		delete graphics;
		graphics = nullptr;
	}

	if (input)
	{
		delete input;
		input = nullptr;
	}

	if (openGL)
	{
		openGL->Shutdown(hwnd);
		delete openGL;
		openGL = nullptr;
	}

	ShutdownWindows();
	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done = false;
	bool result = false;

	ZeroMemory(&msg, sizeof(MSG));

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			done = true;
		else
		{
			result = Frame();
			if (!result)
				done = true;
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool result = false;
	if (input->IsKeyDown(VK_ESCAPE))
	{

		std::cout << input->IsKeyDown(VK_ESCAPE);
		return false;
	}

	result = graphics->Frame();
	if (!result)
		return false;
	return true;
}

LRESULT CALLBACK SystemClass::MassageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
	{
		input->KeyDown((unsigned int)wparam);
		return 0;
	}
	case WM_KEYUP:
	{
		input->KeyUp((unsigned int)wparam);
		return 0;
	}
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);

	}
}

bool SystemClass::InitializeWindows(OpenGLClass* openGL, int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX(0), posY(0);

	ApplicationHandle = this;
	hinstance = GetModuleHandle(NULL);
	applicationName = L"Engine";
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, WS_POPUP, 0, 0, 640, 480, NULL, NULL, hinstance, NULL);
	if (hwnd == NULL)
		return false;
	ShowWindow(hwnd, SW_HIDE);

	int result = openGL->InitializeExtensions(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the OpenGL extensions.", L"Error", MB_OK);
	}

	DestroyWindow(hwnd);
	hwnd = NULL;

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, hinstance, NULL);
	if (hwnd == NULL)
		return false;

	result = openGL->InitializeOpenGL(hwnd, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize OpenGL, check if video card supports OpenGL 4.0", L"Error", MB_OK);
	}

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(false);
	return true;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(hwnd);
	hwnd = NULL;

	hinstance = NULL;

	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return ApplicationHandle->MassageHandler(hwnd, umessage, wparam, lparam);
	}
}