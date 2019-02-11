#include "stdafx.h"
#include "ShaderWorks.h"

ShaderWorks::ShaderWorks() {
	cout << "ShaderWorks constructor" << endl;
}
ShaderWorks::~ShaderWorks() {
	this->OutShader();
}
bool ShaderWorks::LoadShader(string vertex_path, string fragment_path) {
	cout << vertex_path << " and " << fragment_path << " compile" << endl;
	string vertexStr = ReadFile(vertex_path);
	string fragmentStr = ReadFile(fragment_path);
	const GLchar *vertShaderSrc = vertexStr.c_str();
	const GLchar *fragShaderSrc = fragmentStr.c_str();
	//вывод содержимого шейдеров
	/*cout << "Vertex Shader" << endl;							
	cout << vertShaderSrc << endl;
	cout << "----------------------------------------" << endl;
	cout << "FragSrc" << endl;
	cout << fragShaderSrc << endl;
	cout << "----------------------------------------" << endl;*/
	//компиляция вершинного шейдера
	vertexShader = glCreateShader(GL_VERTEX_SHADER);			
	glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertexShader);
	//проверка компиляции шейдера
	if (!ShaderErrorLog(vertexShader)) {						
		glDeleteShader(vertexShader);
		return false;
	} else cout << "vertexShader success compile" << endl;
	//компиляция фрагментного шейдера
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);		
	glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragmentShader);
	if (!ShaderErrorLog(fragmentShader)) {
		glDeleteShader(fragmentShader);
		return false;
	} else cout << "fragmentShader success compile" << endl;
	CreateShaderProgram();
	return true;
}
string ShaderWorks::ReadFile(string path) {
	string content;
	ifstream file(path);
	if (!file.is_open()) return "";
	while (!file.eof()) content += file.get();
	//cout << content << endl;
	file.close();
	return content;
}
bool ShaderWorks::ShaderErrorLog(GLuint &shader) {
	GLint status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar *errorLog = new GLchar(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		cout << errorLog << endl;
		getchar();
		return false;
	}
	return true;
}
void ShaderWorks::CreateShaderProgram() {
	shaderProgram = glCreateProgram();				//создание программы шейдера
	glAttachShader(shaderProgram, vertexShader);	//прикрепление шейдеров к программе
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);					//линковка
}
void ShaderWorks::OutShader() {
	glDetachShader(shaderProgram, vertexShader);	//откреление шейдеров
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);					//удаление шейдеров
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);					//удаление программы
}
void ShaderWorks::Error(string ErrorLog) {
	cout << "Error:" << ErrorLog << endl;
	system("pause");
}
void ShaderWorks::SetUniform3f(const GLchar *unifName, float v1, float v2, float v3) {
	GLint uniform = glGetUniformLocation(this->shaderProgram, unifName);	//получение сссылки в shaderProgram по названию unifName
	if(uniform == -1) Error("Can't Find uniform with name:" + (string)unifName);
	glUniform3f(uniform, v1, v2, v3);										//передача трехмерного вектора
}
void ShaderWorks::SetUniform3f(const GLchar *unifName, glm::vec3 val) {
	GLint uniform = glGetUniformLocation(this->shaderProgram, unifName);	//получение сссылки в shaderProgram по названию unifName
	if (uniform == -1) Error("Can't Find uniform with name:" + (string)unifName);
	glUniform3f(uniform, val.x, val.y, val.z);								//передача трехмерного вектора
}
void ShaderWorks::SetInt(const GLchar *unifName, int val) {
	GLint uniform = glGetUniformLocation(this->shaderProgram, unifName);
	if (uniform == -1) Error("Can't Find uniform with name:" + (string)unifName);
	glUniform1i(uniform, val);
}
void ShaderWorks::SetFloat(const GLchar *unifName, float val) {
	GLint uniform = glGetUniformLocation(this->shaderProgram, unifName);
	if (uniform == -1) Error("Can't Find uniform with name:" + (string)unifName);
	glUniform1f(uniform, val);
}
void ShaderWorks::SetUniformMatrix(GLchar *uniformName, glm::mat4 matrix) {
	GLint uniform = glGetUniformLocation(this->shaderProgram, uniformName);
	if (uniform == -1) Error("Can't Find Matrix Uniform with name:" + (string)uniformName);
	//	ОТПРАВКА МАТРИЦЫ ЮНИФОРМЕ
	/*		uniform - имя юниформы
			1, если у нас не массив матриц, и больше, если массив матриц
			GL_FALSE - следует ли транспонировать матрицу
			значения матрицы												*/
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));		//value_ptr - передача матрицы в пригодном для opengl виде
}
void ShaderWorks::SetAttribute(GLchar *attribName, int vecDimension, int typeOfAttrib, int numOfVertAttrib, int shift) {
	GLint attrib = glGetAttribLocation(shaderProgram, attribName);			//получение ссылки на атрибут в программе по имени attribName
	if(attrib == -1) Error("Can't Find attribute with name:" + (string)attribName);
	//	ОПРЕДЕЛЕНИЕ МАССИВА АТРИБУТОВ ВЕРШИН
	/*		attrib - обращения к позиции атрибута
			vecDimension - размерность вектора
			typeOfAttrib - тип атрибутов(GL_FLOAT и т.д.)
			должны ли значения быть нормализованы(GL_TRUE) или записываться фиксированно, как есть(GL_FALSE)
			numOfAttrib - кол-во элементов
			shift - смещение от начала для каждого атрибута													*/
	glVertexAttribPointer(attrib, vecDimension, typeOfAttrib, GL_FALSE, numOfVertAttrib * sizeof(float), (void*)(shift * sizeof(float)));
	glEnableVertexAttribArray(attrib);										 //задействие атрибута
}