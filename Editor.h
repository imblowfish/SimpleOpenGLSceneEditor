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
	enum editorMode{MOVE_MODE, ROTATE_MODE, SCALE_MODE};	//режимы работы редактора
	unsigned int nowMode=MOVE_MODE;							//режим редактора для управления клавишами
	const GLuint SCREEN_WIDTH = 800;						//горизонтальный размер окна
	const GLuint SCREEN_HEIGHT = 600;						//вертикальный размер окна
	Camera editorCamera;									//камера редактора
	Scene nowScene;											//сцена, отображаемая в редакторе
	glm::vec3 picker;										//3d курсор								
	Color background;
	Model nowModel;											//модель, которая управляется в данный момент
	Lighter nowLighter;										//источник, который управляется в данный момент
	vector<Lighter> lighters;
	vector<string> lightersPaths = {
		"lighters/lamp.obj",
		"lighters/oil_lantern.obj"
	};
	GLenum renderMode = GL_FILL;
	bool nowViewed = true;
	bool textureMode = false;
	BufferWorks buffer;
	glm::mat4 projPersp;									//перспективная проекция												
	glm::mat4 view;											//матрица вида(для сдвига)
	glm::mat4 model;										//матрица модели(для поворота)
	glm::mat4 MVP;											//общая матрица, которая отсылается в шейдер
	//размещение модели
	void PlaceLighter() {this->nowScene.PlaceLighterOnScene(this->nowLighter);}
	//размещение источника
	void PlaceModel(){this->nowScene.PlaceModelOnScene(nowModel);}
	//реагирование на клики
	void ModelEventListener(Keys key, float deltaTime);
	void UpdateNowModelPos();
	//режим отображения
	void ChangeRenderMode() {
		if (this->renderMode == GL_FILL) this->renderMode = GL_LINE;
		else this->renderMode = GL_FILL;
	}
	//движение камеры в сцене
	void CameraEventListener(Keys key, float deltaTime){editorCamera.KeyEvent(key, deltaTime);}
	//преднастройка Editor
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
	//отсрисовка сцены
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
	//слушатель событий
	void EventListener(Keys key, float deltaTime);
};

#endif