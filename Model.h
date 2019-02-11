#pragma once
#include <iostream>
#include <cstdlib>
#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "ShaderWorks.h"
#include "SOIL2.h"
#include "Mesh.h"
using namespace std;

enum Info {
	M_ID, M_NAME, M_RENDERED, M_TEXTURED, M_COLOR, M_SPECULAR, M_POSITION, 
	M_MODEL_MATRIX, M_SHININESS, M_SELECTED, M_OX_ANGLE, M_OY_ANGLE, M_OZ_ANGLE, 
	M_SCALE_VEC
};

class Model {
private:
	//структура информации о модели
	struct ModelInfo {
		unsigned int id;
		string name;
		bool isRendered=true;	//если true, идет расчет освещения для объекта и он отображается, если ет, то виден только в Editor									
		bool isTextured=false;
		glm::vec3 modelColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 modelSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 modelMat = glm::mat4(1.0f);
		float modelShininess = 32.0f;
		float OXangle, OYangle, OZangle = 0.0f;
		glm::vec3 scaleVec = glm::vec3(0.2f);
		bool isSelected = false;
	};
	//информация о составляющей модели
	ModelInfo modelInfo;
	//булева, является ли модель источником света(нужно ли относительно ее расположения расчитывать освещение
	bool isLighter;
	void LoadModel(string path);
	//рекурсивный обход по узлам модели
	void ProcessNode(aiNode *node, const aiScene *scene);
	//обработка меша, разбор на вершины, индексы, текстурные координаты
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	//проверка типа текстуры
	vector<Texture> LoadMaterialTexture(aiMaterial *material, aiTextureType type, string typeName);
	//поиск текстуры в директории и привязка ее к модели
	int TextureFromFile(const char *directory, aiString filename);
public:
	vector<Mesh> meshes;
	string directory;
	Model(string path) {this->LoadModel(path);}
	Model() {cout << "Default Model constructor" << endl;}
	~Model() {cout << "Model destructor" << endl;}
	void DeleteBuffers() {
		if (!meshes.empty()) {
			for(int i=0;i<meshes.size();i++) meshes[i].DeleteBuffers();
		}
	}
	void LoadFromFile(string path) {this->LoadModel(path);}
	bool GetBool(unsigned int infoType);
	void SetBool(unsigned int infoType, bool val);
	glm::vec3 GetVector(unsigned int infoType);
	void SetVector(unsigned int infoType, glm::vec3 val);
	glm::mat4 GetMatrix(unsigned int infoType);
	void SetMatrix(unsigned int infoType, glm::mat4 val);
	float GetFloat(unsigned int infoType);
	void SetFloat(unsigned int infoType, float val);
	void SetString(unsigned int infoType, string val);
	string GetString(unsigned int infoType);
	void SetInt(unsigned int infoType, int val);
	int GetInt(unsigned int infoType);
	void DrawModel(ShaderWorks shader) {
		for (int i = 0; i < meshes.size(); i++) {
			meshes[i].DrawMesh(shader);
		}
	}
	bool IsEmpty() {return this->meshes.empty();}
	void ClearModel() {
		for (int i = 0; i < meshes.size(); i++) if(!meshes.empty()) meshes[i].ClearMesh();
		meshes.clear();
		this->modelInfo.isTextured = false;
	}
};