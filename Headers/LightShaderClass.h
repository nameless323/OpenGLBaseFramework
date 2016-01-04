#pragma once

#include <fstream>
#include "OpenglClass.h"

class LightShaderClass
{
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(OpenGLClass*, HWND);
	void Shutdown(OpenGLClass*);
	void SetShader(OpenGLClass*);
	bool SetShaderParameters(OpenGLClass*, float*, float*, float*, int, float*, float*, float*);
private:
	bool InitializeShader(char*, char*, OpenGLClass*, HWND);
	char* LoadShaderSourceFile(char*);
	void OutputShaderErrorMessage(OpenGLClass*, HWND, unsigned int, char*);
	void OutputLinkerErrorMessage(OpenGLClass*, HWND, unsigned int);
	void ShutdownShader(OpenGLClass*);

	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};