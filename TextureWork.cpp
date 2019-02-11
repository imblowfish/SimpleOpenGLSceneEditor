#include "stdafx.h"
#include "TextureWork.h"
using namespace std;

TextureWork::TextureWork() {
	cout << "TextureWork constructor" << endl;
}
TextureWork::~TextureWork() {

}
void TextureWork::LoadImageFromFile(char* pathToImage) {
	image = SOIL_load_image(pathToImage, &width, &height, 0, SOIL_LOAD_RGB);
}
void TextureWork::GenTexture() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//настройка wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//настройка фильтрации
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);					
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	ГЕНЕРАЦИЯ ТЕКСТУРЫ
	/*
			1)описывает текстурную цель
			2)уровень мипмапа для которого хотим сгенерировать текстуру, здесь генерирует OPENGL, поэтому 0
			3)в каком формате хранится текстура
			4)ширина текстуры
			5)высота текстуры
			7)формат исходного изображения
			8)тип исходного изображения
			9)сами данные
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	this->Unuse();
}