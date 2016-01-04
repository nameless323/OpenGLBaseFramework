#include "stdafx.h"
#include "../Headers/ModelClass.h"
#include <iostream>

ModelClass::ModelClass() 
{
	_texture = 0;
	model = 0;
}

ModelClass::ModelClass(const ModelClass& other) {}

ModelClass::~ModelClass() {}

bool ModelClass::Initialize(OpenGLClass* openGL, char* modelFilename, char* textureFilename, unsigned int textureUnit, bool wrap)
{
	bool result;
	result = LoadModel(modelFilename);
	if (!result)
		return false;

	result = InitializeBuffers(openGL);
	if (!result)
		return false;
	result = LoadTexture(openGL, textureFilename, textureUnit, wrap);
	if (!result)
		return false;
	return true;
}

void ModelClass::Shutdown(OpenGLClass* openGL)
{
	ReleaseTexture();
	ShutdownBuffers(openGL);
	ReleaseModel();
}

void ModelClass::Render(OpenGLClass* opengGL)
{
	RenderBuffers(opengGL);
}

bool ModelClass::InitializeBuffers(OpenGLClass* openGL)
{
	VertexType* vertices;
	unsigned int* indices;

//	vertexCount = 3;
//	indexCount = 3;

	vertices = new VertexType[vertexCount];
	if (!vertices)
		return false;
	indices = new unsigned int[indexCount];
	if (!indices)
		return false;

	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].x = model[i].x;
		vertices[i].y = model[i].y;
		vertices[i].z = model[i].z;
		vertices[i].tu = model[i].tu;
		vertices[i].tv = model[i].tv;
		vertices[i].nx = model[i].nx;
		vertices[i].ny = model[i].ny;
		vertices[i].nz = model[i].nz;
		indices[i] = i;		
	}

	openGL->glGenVertexArrays(1, &vertexArrayId);
	openGL->glBindVertexArray(vertexArrayId);
	
	openGL->glGenBuffers(1, &vertexBufferId);
	openGL->glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	openGL->glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(VertexType), vertices, GL_STATIC_DRAW);

	openGL->glEnableVertexAttribArray(0);
	openGL->glEnableVertexAttribArray(1);
	openGL->glEnableVertexAttribArray(2);

	openGL->glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	openGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);

	openGL->glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	openGL->glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));

	openGL->glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	openGL->glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (5 * sizeof(float)));

	openGL->glGenBuffers(1, &indexBufferId);

	openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	openGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(unsigned int), indices, GL_STATIC_DRAW);

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void ModelClass::ShutdownBuffers(OpenGLClass* openGL)
{
	openGL->glDisableVertexAttribArray(0);
	openGL->glDisableVertexAttribArray(1);

	openGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
	openGL->glDeleteBuffers(1, &indexBufferId);

	openGL->glBindVertexArray(0);
	openGL->glDeleteVertexArrays(1, &vertexArrayId);
}

void ModelClass::RenderBuffers(OpenGLClass* openGL)
{
	openGL->glBindVertexArray(vertexArrayId);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

bool ModelClass::LoadTexture(OpenGLClass* openGL, char* textureFilename, unsigned int textureUnit, bool wrap)
{
	bool result;

	_texture = new TextureClass;
	if (!_texture)
		return false;
	result = _texture->Initialize(openGL, textureFilename, textureUnit, wrap);
	if (!result)
		return false;
	return true;
}

void ModelClass::ReleaseTexture()
{
	if (_texture != nullptr)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = nullptr;
	}
}

bool ModelClass::LoadModel(char* filename)
{
	std::fstream fin;
	char input;
	int i;
	fin.open(filename);

	if (fin.fail())
		return false;

	fin.get(input);
	while (input != ':')
		fin.get(input);
	fin >> vertexCount;
	indexCount = vertexCount;
	model = new ModelType[vertexCount];
	if (!model)
		return false;
	fin.get(input);
	while (input != ':')
		fin.get(input);
	fin.get(input);
	fin.get(input);
	for (i = 0; i < vertexCount; i++)
	{
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
			
	}
	fin.close();
	return true;
}

void ModelClass::ReleaseModel()
{
	if (model != nullptr)
	{
		delete[] model;
		model = nullptr;
	}

}