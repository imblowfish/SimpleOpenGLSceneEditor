#pragma once
#ifndef SCENE
#define SCENE
#include <fstream>
#include "Model.h"
#include "ShaderWorks.h"
#include "Lights.h"
using namespace std;

class Scene {
private:
	string name;
	unsigned int countOfObjects;	//кол-во объектов на сцене(своеобразный id для каждой модели)
	int nowId = -1;
	const unsigned int MAX_LIGHTS = 10;
	vector<Model> models;			//массив моделей в сцене
	vector<Lighter> lighters;		//массив источников освещения в сцене
	bool nowEdit;					//сейчас сцена редактируется
	bool nowViewed = true;
	bool textureMode = true;
	void DrawModels(ShaderWorks shader, glm::mat4 cameraMatrix, glm::vec3 cameraPos, glm::mat4 projection);
	void AddLighters(ShaderWorks shader);
	void DrawLighters(ShaderWorks shader, glm::mat4 cameraMatrix, glm::vec3 cameraPos, glm::mat4 projection);
	void DisplayNumOfLights() {
		cout << "Max light num = " << this->MAX_LIGHTS << endl;
		cout << "You can place " << this->MAX_LIGHTS - this->lighters.size() << endl;
	}
	void ClearAllAboutScene();
	void ModelsSerialization(ofstream &file);
	void ModelsDeserialization(ifstream &file);
	void LightersSerialization(ofstream &file);
	void LightersDeserialization(ifstream &file);
	int GetLighterIdByType(unsigned int type) {
		if (lighters.empty()) return -1;
		for (int i = 0; i < lighters.size(); i++) 
			if (lighters[i].type == type) return i;
		return -1;
	}
	void RecountAllId() {
		if (countOfObjects == 0) return;
		countOfObjects = 0;
		for (int i = 0; i < models.size(); i++) models[i].SetInt(M_ID, countOfObjects++);
		for (int i = 0; i < lighters.size(); i++) lighters[i].model.SetInt(M_ID, countOfObjects++);
	}
	void ChangeModelById(int id);
	void ChangeLighterById(int id);
public:
	bool isAnimated = false;		//анимированная ли сцена
	Scene() {
		cout << "Scene constructor" << endl;
	}
	~Scene() {
		this->ClearScene();
		cout << "Scene destructor" << endl;
	}
	bool SaveScene(const char *sceneName);
	bool LoadScene(const char *sceneName);
	string GetSceneName() {
		if (this->name == "") return name;
		if (this->isAnimated) return this->name + "(Animated)";
		else return this->name;
	}
	void SetTextureMode() {
		if (this->IsEmpty()) return;
		this->textureMode = !this->textureMode;
	}
	int CheckObjectById(unsigned int id);
	Model GetModelById(unsigned int id);
	Lighter GetLighterById(unsigned int id);
	void DeleteById(unsigned int id);
	void PlaceModelOnScene(Model model);
	void PlaceLighterOnScene(Lighter lighter);
	void SetLightersViewed(bool val) {
		for (int i = 0; i < lighters.size(); i++) lighters[i].isViewed = val;
		nowViewed = val;
	}
	int GetModelIdByName(string name) {
		for (int i = 0; i < models.size(); i++)
			if (models[i].GetString(M_NAME) == name) return models[i].GetInt(M_ID);
		return -1;
	}
	void SelectNextObject() {
		if (this->nowId < 0) {
			nowId = 0;
			return;
		}
		if (nowId < this->countOfObjects-1) nowId++;
	}
	void SelectPrevObject() {
		if (this->nowId < 0) {
			nowId = 0;
			return;
		}
		if (nowId > 0) nowId--;
	}
	void ResetSelectedObject() {this->nowId = -1;}
	int GetSelectedId() {
		if (countOfObjects == 0) return -1;
		return this->nowId;
	}
	void EditSelected();
	bool IsEmpty() {return models.empty() && lighters.empty();}
	void ClearScene();
	void DrawScene(ShaderWorks shader, glm::mat4 cameraMatrix, glm::vec3 cameraPos, glm::mat4 projection) {
		if (!models.empty()) DrawModels(shader, cameraMatrix, cameraPos, projection);
		if (models.empty() && !lighters.empty()) this->DrawLighters(shader, cameraMatrix, cameraPos, projection);
	}
};

#endif
