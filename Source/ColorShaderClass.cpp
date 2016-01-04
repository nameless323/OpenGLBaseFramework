#include "stdafx.h"
#include "../Headers/ColorShaderClass.h"
#include <iostream>
ColorShaderClass::ColorShaderClass() {}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other) {}

ColorShaderClass::~ColorShaderClass() {}

bool ColorShaderClass::Initialize(OpenGLClass* openGL, HWND hwnd)
{
	bool result;

	result = InitializeShader(".//Shaders//color.vs", ".//Shaders//color.ps", openGL, hwnd);
	std::cout << result;
	if (!result)
	{
		return false;
	}
	std::cout << "shader done" << std::endl;
	return true;
}

void ColorShaderClass::Shutdown(OpenGLClass* openGL)
{
	ShutdownShader(openGL);
}

void ColorShaderClass::SetShader(OpenGLClass* openGL)
{
	openGL->glUseProgram(shaderProgram);
}

bool ColorShaderClass::InitializeShader(char* vsFilename, char* fsFilename, OpenGLClass* openGL, HWND hwnd)
{
	const char* vertexShaderBuffer;
	const char* fragmentShaderBuffer;

	int status;

	vertexShaderBuffer = LoadShaderSourceFile(vsFilename);

	std::cout << "Vert" << std::endl;
	if (!vertexShaderBuffer)
		return false;
	std::cout << "Vert done" << std::endl;
	fragmentShaderBuffer = LoadShaderSourceFile(fsFilename);
	if (!fragmentShaderBuffer)
		return false;
	std::cout << std::endl << "Frag done" << std::endl;
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
	std::cout << "compt" << std::endl;
	openGL->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		std::cout << "frag err" << std::endl;
		OutputShaderErrorMessage(openGL, hwnd, fragmentShader, fsFilename);
		return false;
	}

	shaderProgram = openGL->glCreateProgram();
	openGL->glAttachShader(shaderProgram, vertexShader);
	openGL->glAttachShader(shaderProgram, fragmentShader);

	openGL->glBindAttribLocation(shaderProgram, 0, "inputPosition");
	openGL->glBindAttribLocation(shaderProgram, 1, "inputColor");

	openGL->glLinkProgram(shaderProgram);

	openGL->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		std::cout << "link err" << std::endl;
		OutputLinkerErrorMessage(openGL, hwnd, shaderProgram);
		return false;
	}
	return true;
}

#include <string>
char* ColorShaderClass::LoadShaderSourceFile(char* filename)
{
	std::ifstream fin;
	int fileSize;
	char input;
	char* buffer;

	std::cout << "fname " << filename << std::endl;
	fin.open(filename, std::ios::in);
	
	std::cout << filename << std::endl;
	if (fin.fail())
	{
		return 0;
	}
	std::cout << "fname norm";
	fileSize = 0;
	fin.get(input);
	while (!fin.eof())
	{
		fileSize++;
		fin.get(input);
		std::cout << input;
 	}
	std::cout << std::endl;
	std::cout << "file ends";
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

void ColorShaderClass::OutputShaderErrorMessage(OpenGLClass* openGL, HWND hwnd, unsigned int shaderId, char* shaderFilename)
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
	{
		fout << infoLog[i];
	}
	fout.close();
	
	error = mbstowcs_s(&convertedChars, newString, 128, shaderFilename, 128);
	if (error != 0)
		return;
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", newString, MB_OK);
}

void ColorShaderClass::OutputLinkerErrorMessage(OpenGLClass* openGL, HWND hwnd, unsigned int programId)
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
	fout.close();

	MessageBox(hwnd, L"Error compiling linker. Chaeck linker-error.txt for message.", L"Linker Error", MB_OK);
}

void ColorShaderClass::ShutdownShader(OpenGLClass* openGL)
{
	openGL->glDetachShader(shaderProgram, vertexShader);
	openGL->glDetachShader(shaderProgram, fragmentShader);

	openGL->glDeleteShader(vertexShader);
	openGL->glDeleteShader(fragmentShader);

	openGL->glDeleteProgram(shaderProgram);
}

bool ColorShaderClass::SetShaderParameters(OpenGLClass* openGL, float* worldMatrix, float* viewMatrix, float* projectionMatrix)
{
	unsigned int location;

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
	{
		return false;
	}
	openGL->glUniformMatrix4fv(location, 1, false, projectionMatrix);

	return true;
}