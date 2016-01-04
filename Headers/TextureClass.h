#pragma once

#include <stdio.h>
#include "OpenglClass.h"

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();
	bool Initialize(OpenGLClass*, char*, unsigned int, bool);
	void Shutdown();
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
	bool LoadTarga(OpenGLClass*, char*, unsigned int, bool);
	bool loaded;
	unsigned int _textureId;
};