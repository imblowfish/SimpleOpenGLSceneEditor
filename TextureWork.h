#pragma once
#ifndef TEXTURE_WORK
#define TEXTURE_WORK
#include <SOIL2.h>
#include <iostream>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
class TextureWork {
private:
	int width, height;
	unsigned char *image;
	GLuint texture;
	GLuint specularMap;
public:
	void GenTexture();
	void LoadImageFromFile(char*);
	void Use(){glBindTexture(GL_TEXTURE_2D, texture);}
	void Unuse(){glBindTexture(GL_TEXTURE_2D, 0);}
	TextureWork();
	~TextureWork();
};
#endif TEXTURE_WORK