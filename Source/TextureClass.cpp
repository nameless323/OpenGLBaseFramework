#include "stdafx.h"

#include "../Headers/TextureClass.h"

TextureClass::TextureClass()
{
	loaded = false;
}

TextureClass::TextureClass(const TextureClass& other)
{}

TextureClass::~TextureClass() {}

bool TextureClass::Initialize(OpenGLClass* openGL, char* filename, unsigned int textureUnit, bool wrap)
{
	bool result;

	result = LoadTarga(openGL, filename, textureUnit, wrap);
	if (!result)
		return false;
	return true;
}

void TextureClass::Shutdown()
{
	if (loaded)
	{
		glDeleteTextures(1, &_textureId);
		loaded = false;
	}
}

bool TextureClass::LoadTarga(OpenGLClass* openGL, char* filename, unsigned int textureUnit, bool wrap)
{
	int error, width, height, bpp, imageSize;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
		return false;

	count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
		return false;

	width = (int)targaFileHeader.width;
	height = (int)targaFileHeader.height;
	bpp = (int)targaFileHeader.bpp;

	if (bpp != 32)
		return false;

	imageSize = width*height * 4;

	targaImage = new unsigned char[imageSize];
	if (!targaImage)
		return false;

	count = fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
		return false;

	error = fclose(filePtr);
	if (error != 0)
		return false;

	openGL->glActiveTexture(GL_TEXTURE0 + textureUnit);
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, targaImage); //or gl_rgba if will work wrong

	if (wrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	openGL->glGenerateMipmap(GL_TEXTURE_2D);

	delete[] targaImage;

	loaded = true;
	return true;
}
