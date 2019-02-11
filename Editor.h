#pragma once
#ifndef EDITOR
#define EDITOR
#include <cstdlib>
#include <ctime>
#include "BuffersWorks.h"
#include "Camera.h"
#include "Color.h"
#include "Scene.h"
#include <vector>
using namespace std;

class Editor {
private:
	enum editorMode{MOVE_MODE, ROTATE_MODE, SCALE_MODE};	//������ ������ ���������
	unsigned int nowMode=MOVE_MODE;							//����� ��������� ��� ���������� ���������
	const GLuint SCREEN_WIDTH = 800;						//�������������� ������ ����
	const GLuint SCREEN_HEIGHT = 600;						//������������ ������ ����
	Camera editorCamera;									//������ ���������
	Scene nowScene;											//�����, ������������ � ���������
	glm::vec3 picker;										//3d ������								
	Color background;
	Model nowModel;											//������, ������� ����������� � ������ ������
	Lighter nowLighter;										//��������, ������� ����������� � ������ ������
	vector<Lighter> lighters;
	vector<string> lightersPaths = {
		"lighters/lamp.obj",
		"lighters/oil_lantern.obj"
	};
	GLenum renderMode = GL_FILL;
	bool nowViewed = true;
	bool textureMode = false;
	BufferWorks buffer;
	glm::mat4 projPersp;									//������������� ��������												
	glm::mat4 view;											//������� ����(��� ������)
	glm::mat4 model;										//������� ������(��� ��������)
	glm::mat4 MVP;											//����� �������, ������� ���������� � ������
	//���������� ������
	void PlaceLighter() {this->nowScene.PlaceLighterOnScene(this->nowLighter);}
	//���������� ���������
	void PlaceModel(){this->nowScene.PlaceModelOnScene(nowModel);}
	//������������ �� �����
	void ModelEventListener(Keys key, float deltaTime);
	void UpdateNowModelPos();
	//����� �����������
	void ChangeRenderMode() {
		if (this->renderMode == GL_FILL) this->renderMode = GL_LINE;
		else this->renderMode = GL_FILL;
	}
	//�������� ������ � �����
	void CameraEventListener(Keys key, float deltaTime){editorCamera.KeyEvent(key, deltaTime);}
	//������������� Editor
	void SetupEditor();
	void ChangeBackground(Color color) {this->background = color;}
	void DrawNowModel(ShaderWorks shader);
	void DrawNowLighter(ShaderWorks shader);
	void PreloadLighters();
	void ClearPicker();
public:
	bool isRunned = false;
	Editor() {
		cout << "Editor constructor" << endl;
	}
	void StartEditor() {
		cout << "Editor start" << endl; 
		this->SetupEditor(); 
		this->isRunned = true;
		this->PreloadLighters();
	}
	~Editor() {
		cout << "Editor destructor" << endl;
		nowModel.ClearModel();
		nowLighter.model.ClearModel();
		for (int i = 0; i < lighters.size(); i++) lighters[i].model.ClearModel();
		lighters.clear();
		nowScene.ClearScene();
	}
	//���������� �����
	void DrawEditorScene(ShaderWorks shader);
	void LoadModel(string modelName);
	void SetTextureMode() {nowScene.SetTextureMode();}
	void LoadLighter(unsigned int lighterType);
	void SaveSceneFromEditor(string nameOfScene);
	void LoadSceneInEditor(string nameOfScene);
	int GetScreenWidth() {return this->SCREEN_WIDTH;}
	int GetScreenHeight() {return this->SCREEN_HEIGHT;}
	void CutObject();
	void EditSelectedObject() {
		if (!this->isRunned) return;
		if (this->nowScene.IsEmpty()) return;
		this->nowScene.EditSelected();
	}
	bool NowSceneEmpty() {return this->nowScene.IsEmpty();}
	void NextObject() {
		if (!this->isRunned) return;
		if (nowScene.IsEmpty()) return;
		nowScene.SelectNextObject();
	}
	void PrevObject() {
		if (!this->isRunned) return;
		if (nowScene.IsEmpty()) return;
		nowScene.SelectPrevObject();
	}
	void Clear() {if (!nowScene.IsEmpty()) nowScene.ClearScene();}
	void ToOrigin();
	void SetNewBackground();
	//��������� �������
	void EventListener(Keys key, float deltaTime);
};

#endif