#pragma once

#include "OpenglClass.h"
#include "TextureClass.h"
#include <fstream>

class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(OpenGLClass*, char*, char*, unsigned int, bool);
	void Shutdown(OpenGLClass*);
	void Render(OpenGLClass*);
private:
	struct VertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	bool LoadTexture(OpenGLClass*, char*, unsigned int, bool);
	void ReleaseTexture();
	bool InitializeBuffers(OpenGLClass*);
	void ShutdownBuffers(OpenGLClass*);
	void RenderBuffers(OpenGLClass*);
	int vertexCount, indexCount;
	unsigned int vertexArrayId, vertexBufferId, indexBufferId;
	bool LoadModel(char*);
	void ReleaseModel();

	TextureClass* _texture;
	ModelType* model;
};