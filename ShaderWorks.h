#pragma once
#ifndef SHADER_WORKS
#define SHADER_WORKS
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <fstream>
#include <string>
#include <glm/glm.hpp>																					     //OpenGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
class ShaderWorks {
private:										
	GLuint vertexShader;																		
	GLuint fragmentShader;																		
	string vertexFilePath;																		
	string fragmentFilePath;																	
	bool ShaderErrorLog(GLuint &shader);																	  //����� �� ������� ����������	
	void CreateShaderProgram();																				  //�������� ��������� ���������
	string ReadFile(string path);
	void OutShader();																						  //����������� � ���������� ��������
	void Error(string ErrorLog);																			  //������ ������� � ���������
public:
	GLuint shaderProgram;
	void SetUniform3f(const GLchar *unifName, float v1, float v2, float v3);								  //��������� ������� ��������
	void SetUniform3f(const GLchar *unifName, glm::vec3 val);
	void SetUniformMatrix(GLchar *uniformName, glm::mat4 matrix);											  //��������� ������� ��������
	void SetInt(const GLchar *unifName, int val);															  //��������� int ��������
	void SetFloat(const GLchar *unifName, float val);														  //��������� float ��������
	void SetAttribute(GLchar *attribName, int vecDimension, int typeOfAttrib, int numOfVertAttrib, int shift);//��������� ������ �� ��������
	bool LoadShader(string vertex_path, string fragment_path);												  //�������� �������� �� ������
	void Use() {glUseProgram(this->shaderProgram);}		
	ShaderWorks();
	~ShaderWorks();
};
#endif SHADER_WORKS