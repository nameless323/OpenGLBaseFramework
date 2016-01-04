// OpenGlTutorial.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "../Headers/SystemClass.h"

int main()
{
	SystemClass* system;
	bool result = false;

	system = new SystemClass;
	if (!system)
		return -1;
	result = system->Initialize();
	if (result)
		system->Run();

	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}