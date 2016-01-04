#include "stdafx.h"

#include "../Headers/LightShaderClass.h"
#include <iostream>

LightShaderClass::LightShaderClass() {}

LightShaderClass::LightShaderClass(const LightShaderClass& other) {}

LightShaderClass::~LightShaderClass() {}

bool LightShaderClass::Initialize(OpenGLClass* openGL, HWND hwnd)
{
	bool result;

	result = InitializeShader(".//Shaders//Light.vs", ".//Shaders//Light.ps", openGL, hwnd);
	if (!result)
		return false;
	return true;
}

void LightShaderClass::Shutdown(OpenGLClass* openGL)
{
	ShutdownShader(openGL);
}

void LightShaderClass::SetShader(OpenGLClass* openGL)
{
	openGL->glUseProgram(shaderProgram);
}

bool LightShaderClass::InitializeShader(char* vsFilename, char* fsFilename, OpenGLClass* openGL, HWND hwnd)
{
	const char* vertexShaderBuffer;
	const char* fragmentShaderBuffer;
	int status;
	vertexShaderBuffer = LoadShaderSourceFile(vsFilename);
	if (!vertexShaderBuffer)
		return false;

	fragmentShaderBuffer = LoadShaderSourceFile(fsFilename);
	if (!fragmentShaderBuffer)
		return false;

	vertexShader = openGL->glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = openGL->glCreateShader(GL_FRAGMENT_SHADER);
	openGL->glShaderSource(vertexShader, 1, &vertexShaderBuffer, NULL);
	openGL->glShaderSource(fragmentShader, 1, &fragmentShaderBuffer, NULL);


	delete[] vertexShaderBuffer;
	vertexShaderBuffer = nullptr;
	delete[] fragmentShaderBuffer;
	fragmentShaderBuffer = nullptr;

	openGL->glCompileShader(vertexShader);
	openGL->glCompileShader(fragmentShader);

	openGL->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(openGL, hwnd, vertexShader, vsFilename);
		return false;
	}

	openGL->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(openGL, hwnd, vertexShader, fsFilename);
		return false;
	}

	shaderProgram = openGL->glCreateProgram();
	openGL->glAttachShader(shaderProgram, vertexShader);
	openGL->glAttachShader(shaderProgram, fragmentShader);

	openGL->glBindAttribLocation(shaderProgram, 0, "inputPosition");
	openGL->glBindAttribLocation(shaderProgram, 1, "inputTexCoord");
	openGL->glBindAttribLocation(shaderProgram, 2, "inputNormal");

	openGL->glLinkProgram(shaderProgram);

	openGL->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(openGL, hwnd, shaderProgram);
		return false;
	}
	return true;
}

char* LightShaderClass::LoadShaderSourceFile(char* filename)
{
	std::fstream fin;
	int fileSize;
	char input;
	char* buffer;

	fin.open(filename);

	if (fin.fail())
		return 0;

	fileSize = 0;
	fin.get(input);

	while (!fin.eof())
	{
		fileSize++;
		fin.get(input);
	}
	fin.close();

	buffer = new char[fileSize + 1];
	if (!buffer)
		return 0;

	fin.open(filename);
	fin.read(buffer, fileSize);

	fin.close();
	buffer[fileSize] = '\0';
	return buffer;
}

void LightShaderClass::OutputShaderErrorMessage(OpenGLClass* openGL, HWND hwnd, unsigned int shaderId, char* shaderFilename)
{
	int logSize, i;
	char* infoLog;
	std::ofstream fout;
	wchar_t newString[128];
	unsigned int error, convertedChars;

	openGL->glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
	logSize++;
	infoLog = new char[logSize];
	if (!infoLog)
		return;

	openGL->glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
	fout.open("shader-error.txt");

	for (i = 0; i < logSize; i++)
		fout << infoLog[i];

	fout.close();
	error = mbstowcs_s(&convertedChars, newString, 128, shaderFilename, 128);
	if (error != 0) return;

	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message", newString, MB_OK);
}

void LightShaderClass::OutputLinkerErrorMessage(OpenGLClass* openGL, HWND hwnd, unsigned int programId)
{
	int logSize, i;
	char* infoLog;
	std::ofstream fout;

	openGL->glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);
	logSize++;
	infoLog = new char[logSize];
	if (!infoLog)
		return;
	openGL->glGetProgramInfoLog(programId, logSize, NULL, infoLog);
	fout.open("linker-error.txt");
	for (i = 0; i < logSize; i++)
		fout << infoLog[i];
	fout.close();
	MessageBox(hwnd, L"Error compiling linker. Check linker-error.txt for message", L"Linker Error", MB_OK);
}

void LightShaderClass::ShutdownShader(OpenGLClass* openGL)
{
	openGL->glDetachShader(shaderProgram, vertexShader);
	openGL->glDetachShader(shaderProgram, fragmentShader);

	openGL->glDeleteShader(vertexShader);
	openGL->glDeleteShader(fragmentShader);

	openGL->glDeleteProgram(shaderProgram);
}

bool LightShaderClass::SetShaderParameters(OpenGLClass* openGL, float* worldMatrix, float* viewMatrix, float* projectionMatrix, int textureUnit, float* lightDirection, float* diffuseLightColor, float* ambientLight)
{
	unsigned int location;
	location = openGL->glGetUniformLocation(shaderProgram, "shaderTexture");
	if (location == -1)
		return false;
	openGL->glUniform1i(location, textureUnit);
	location = openGL->glGetUniformLocation(shaderProgram, "worldMatrix");
	if (location == -1)
		return false;
	openGL->glUniformMatrix4fv(location, 1, false, worldMatrix);

	location = openGL->glGetUniformLocation(shaderProgram, "viewMatrix");
	if (location == -1)
		return false;
	openGL->glUniformMatrix4fv(location, 1, false, viewMatrix);

	location = openGL->glGetUniformLocation(shaderProgram, "projectionMatrix");
	if (location == -1)
		return false;
	openGL->glUniformMatrix4fv(location, 1, false, projectionMatrix);

	
	location = openGL->glGetUniformLocation(shaderProgram, "diffuseLightColor");
	if (location == -1)
		return false;
	openGL->glUniform4fv(location, 1, diffuseLightColor);

	location = openGL->glGetUniformLocation(shaderProgram, "lightDirection");
	if (location == -1)
		return false;

	openGL->glUniform3fv(location, 1, lightDirection);
	
	location = openGL->glGetUniformLocation(shaderProgram, "ambientLight");
	if (location == -1)
		return false;

	openGL->glUniform3fv(location, 1, ambientLight);

	return true;
}