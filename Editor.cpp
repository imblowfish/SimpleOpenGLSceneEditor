#include "stdafx.h"
#include "Editor.h"

void Editor::ModelEventListener(Keys key, float deltaTime) {
	if (nowMode == MOVE_MODE) {
		float speed = deltaTime * 2;
		glm::vec3 moveDir;
		if (key == J) {
			moveDir = glm::vec3(1.0f, 0.0f, 0.0f);
			picker -= moveDir * speed;
			this->UpdateNowModelPos();
		}
		if (key == L) {
			moveDir = glm::vec3(1.0f, 0.0f, 0.0f);
			picker += moveDir * speed;
			this->UpdateNowModelPos();
		}
		if (key == K) {
			moveDir = glm::vec3(0.0f, 0.0f, 1.0f);
			picker += moveDir * speed;
			this->UpdateNowModelPos();
		}
		if (key == I) {
			moveDir = glm::vec3(0.0f, 0.0f, 1.0f);
			picker -= moveDir * speed;
			this->UpdateNowModelPos();
		}
		if (key == U) {
			moveDir = glm::vec3(0.0f, 1.0f, 0.0f);
			picker += moveDir * speed;
			this->UpdateNowModelPos();
		}
		if (key == O) {
			moveDir = glm::vec3(0.0f, 1.0f, 0.0f);
			picker -= moveDir * speed;
			this->UpdateNowModelPos();
		}
	} else if (nowMode == ROTATE_MODE) {
		if (nowModel.IsEmpty()) return;
		float val = 0.0f;
		if (key == J) {
			val = nowModel.GetFloat(M_OY_ANGLE);
			val += 0.07f;
			nowModel.SetFloat(M_OY_ANGLE, val);
		}
		if (key == L) {
			val = nowModel.GetFloat(M_OY_ANGLE);
			val -= 0.07f;
			nowModel.SetFloat(M_OY_ANGLE, val);
		}
		if (key == K) {
			val = nowModel.GetFloat(M_OX_ANGLE);
			val -= 0.07f;
			nowModel.SetFloat(M_OX_ANGLE, val);
		}
		if (key == I) {
			val = nowModel.GetFloat(M_OX_ANGLE);
			val += 0.07f;
			nowModel.SetFloat(M_OX_ANGLE, val);
		}
		if (key == U) {
			val = nowModel.GetFloat(M_OZ_ANGLE);
			val -= 0.07f;
			nowModel.SetFloat(M_OZ_ANGLE, val);
		}
		if (key == O) {
			val = nowModel.GetFloat(M_OZ_ANGLE);
			val += 0.07f;
			nowModel.SetFloat(M_OZ_ANGLE, val);
		}
	} else if (nowMode == SCALE_MODE) {
		if (nowModel.IsEmpty()) return;
		glm::vec3 val;
		if (key == PLUS) {
			val = nowModel.GetVector(M_SCALE_VEC);
			val += glm::vec3(0.03f);
			nowModel.SetVector(M_SCALE_VEC, val);
		}
		if (key == MINUS) {
			val = nowModel.GetVector(M_SCALE_VEC);
			val -= glm::vec3(0.03f);
			nowModel.SetVector(M_SCALE_VEC, val);
		}
	}
}
void Editor::UpdateNowModelPos() {
	if (!this->nowModel.IsEmpty())nowModel.SetVector(M_POSITION, this->picker);
	if (!this->nowLighter.isEmpty()) {
		nowLighter.pos = this->picker;
		nowLighter.model.SetVector(M_POSITION, this->picker);
	}
}
void Editor::SetupEditor() {
	cout << "--------------------" << endl;
	cout << "Setup Editor..." << endl;
	cout << "--------------------" << endl;
	projPersp = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	MVP = glm::mat4(1.0f);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	editorCamera.cameraKeyMove(true);																						//разрешение на пердвижение WASD		
	editorCamera.cameraKeyRotate(true);																						//разрешение на вращение стрелками
	projPersp = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);	//настройка перспективы															//настройка позиции камеры
	editorCamera.SetPosition(glm::vec3(0.0f, -1.0f, 6.0f));
	this->ChangeBackground(Color(0.5f, 0.5f, 0.5f, 1.0f));
	this->picker = glm::vec3(0.0f);
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(model, this->picker);
	this->model = glm::scale(model, glm::vec3(0.2f));
	cout << "--------------------" << endl;
	cout << "Editor setup success" << endl;
	cout << "--------------------" << endl;
}
void Editor::DrawNowModel(ShaderWorks shader) {
	shader.Use();
	this->view = glm::mat4(1.0f);
	this->view = this->editorCamera.GetCameraMatrix();
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, nowModel.GetVector(M_POSITION));
	this->model = glm::rotate(this->model, glm::radians(nowModel.GetFloat(M_OX_ANGLE)), glm::vec3(1.0f, 0.0f, 0.0f));
	this->model = glm::rotate(this->model, glm::radians(nowModel.GetFloat(M_OY_ANGLE)), glm::vec3(0.0f, 1.0f, 0.0f));
	this->model = glm::rotate(this->model, glm::radians(nowModel.GetFloat(M_OZ_ANGLE)), glm::vec3(0.0f, 0.0f, 1.0f));
	this->model = glm::scale(this->model, nowModel.GetVector(M_SCALE_VEC));
	nowModel.SetMatrix(M_MODEL_MATRIX, this->model);
	this->MVP = this->projPersp * this->view * nowModel.GetMatrix(M_MODEL_MATRIX);
	shader.SetInt("material.editorMode", true);
	shader.SetInt("material.isSelected", false);
	shader.SetInt("material.isTextured", nowModel.GetBool(M_TEXTURED));
	shader.SetInt("material.isRendered", false);
	shader.SetUniform3f("material.objColor", nowModel.GetVector(M_COLOR));
	shader.SetUniform3f("material.specular", nowModel.GetVector(M_SPECULAR));
	shader.SetFloat("material.shininess", nowModel.GetFloat(M_SHININESS));
	glm::vec3 camPos = this->editorCamera.getCameraPosition();
	shader.SetUniform3f("viewPos", camPos);
	shader.SetUniformMatrix("model", nowModel.GetMatrix(M_MODEL_MATRIX));
	shader.SetUniformMatrix("MVP", this->MVP);
	nowModel.DrawModel(shader);
}
void Editor::DrawNowLighter(ShaderWorks shader) {
	shader.Use();
	this->view = glm::mat4(1.0f);
	this->view = this->editorCamera.GetCameraMatrix();
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, nowLighter.model.GetVector(M_POSITION));
	this->model = glm::scale(this->model, nowLighter.model.GetVector(M_SCALE_VEC));
	nowLighter.modelMatrix = this->model;
	nowLighter.model.SetMatrix(M_MODEL_MATRIX, this->model);
	this->MVP = this->projPersp * this->view * nowLighter.model.GetMatrix(M_MODEL_MATRIX);
	shader.SetInt("material.editorMode", true);
	shader.SetInt("material.isSelected", false);
	shader.SetInt("material.isTextured", false);
	shader.SetInt("material.isRendered", false);
	shader.SetUniform3f("material.objColor", nowLighter.color);
	glm::vec3 camPos = this->editorCamera.getCameraPosition();
	shader.SetUniform3f("viewPos", camPos);
	shader.SetUniformMatrix("model", nowLighter.model.GetMatrix(M_MODEL_MATRIX));
	shader.SetUniformMatrix("MVP", this->MVP);
	nowLighter.model.DrawModel(shader);
}
void Editor::PreloadLighters() {
	for (int i = 0; i < this->lightersPaths.size(); i++) {
		Lighter lighter;
		lighter.model.LoadFromFile(this->lightersPaths[i]);
		lighter.type = i;
		lighter.pos = this->picker;
		lighter.model.SetVector(M_POSITION, this->picker);
		lighter.model.SetString(M_NAME, lightersPaths[i]);
		this->lighters.push_back(lighter);
	}
}
void Editor::ClearPicker() {
	if (!this->nowModel.IsEmpty()) {
		this->nowModel.ClearModel();
		this->nowModel.DeleteBuffers();
	}
	for (int i = 0; i < lighters.size(); i++) {
		if (lighters[i].isViewed) lighters[i].isViewed = false;
		if (nowLighter.isViewed) nowLighter.isViewed = false;
	}
	if (!nowLighter.isEmpty()) nowLighter.model.ClearModel();
}
void Editor::LoadModel(string modelName) {
	if (nowModel.GetString(M_NAME) == modelName) return;
	this->ClearPicker();
	if (nowScene.GetModelIdByName(modelName) != -1) {
		nowModel = nowScene.GetModelById(nowScene.GetModelIdByName(modelName));
		return;
	}
	this->nowModel.LoadFromFile("models/" + modelName + ".obj");
	this->nowModel.SetString(M_NAME, modelName);
	this->nowModel.SetVector(M_POSITION, this->picker);
}
void Editor::LoadLighter(unsigned int lighterType) {
	this->ClearPicker();
	this->nowLighter.type = lighterType;
	this->nowLighter.model = this->lighters[lighterType].model;
	this->nowLighter.model.SetVector(M_POSITION, this->picker);
	if (nowLighter.type == 0) nowLighter.model.SetVector(M_SCALE_VEC, glm::vec3(0.2f));
	else nowLighter.model.SetVector(M_SCALE_VEC, glm::vec3(0.02f));
	this->nowLighter.pos = this->picker;
	if (!this->nowLighter.isViewed) this->nowLighter.isViewed = true;
}
void Editor::CutObject() {
	if (!this->isRunned) return;
	if (this->nowScene.IsEmpty()) return;
	int id = this->nowScene.GetSelectedId();
	cout << "Edit selected object with id=" << id << endl;
	if (this->nowScene.CheckObjectById(id) == 1) {
		this->ClearPicker();
		this->nowModel = this->nowScene.GetModelById(id);
		this->picker = nowModel.GetVector(M_POSITION);
	} else if (this->nowScene.CheckObjectById(id) == 2) {
		this->ClearPicker();
		this->nowLighter = this->nowScene.GetLighterById(id);
		this->picker = nowLighter.model.GetVector(M_POSITION);
		if (this->nowLighter.type == 0) nowLighter.model.SetVector(M_SCALE_VEC, glm::vec3(0.2f));
		else nowLighter.model.SetVector(M_SCALE_VEC, glm::vec3(0.02f));
	} else return;
	nowScene.DeleteById(id);
}
void Editor::EventListener(Keys key, float deltaTime) {
	if (!this->isRunned) return;
	this->CameraEventListener(key, deltaTime);
	this->ModelEventListener(key, deltaTime);
	if (key == Z) this->ChangeRenderMode();
	if (key == K_EIGHT) this->nowMode = MOVE_MODE;
	if (key == K_NINE && !nowModel.IsEmpty()) this->nowMode = ROTATE_MODE;
	if (key == K_ZERO && !nowModel.IsEmpty()) this->nowMode = SCALE_MODE;
	if (key == SPACE) {
		if (!this->nowModel.IsEmpty()) this->PlaceModel();
		else if (!this->nowLighter.isEmpty()) this->PlaceLighter();
	}
	if (key == H) this->nowScene.SetLightersViewed(nowViewed = !nowViewed);
	if (key == R) {
		this->ToOrigin();
		nowScene.ResetSelectedObject();
	}
}
void Editor::DrawEditorScene(ShaderWorks shader) {
	this->buffer.SetupScene(GL_FRONT_AND_BACK, this->renderMode, this->background);
	shader.Use();
	if (!nowModel.IsEmpty()) this->DrawNowModel(shader);
	if (!nowLighter.isEmpty()) this->DrawNowLighter(shader);
	if (!nowScene.IsEmpty()) nowScene.DrawScene(shader, this->editorCamera.GetCameraMatrix(), this->editorCamera.getCameraPosition(), this->projPersp);
}
void Editor::SaveSceneFromEditor(string nameOfScene) {
	cout << "Save scene from editor" << endl;
	if (!this->nowScene.SaveScene(nameOfScene.c_str())) {
		cout << "ERROR. Can't save scene" << endl;
		system("pause");
	} else {
		cout << "Succes scene save" << endl;
		system("pause");
	}
}
void Editor::LoadSceneInEditor(string nameOfScene) {
	nowModel.ClearModel();
	nowLighter.model.ClearModel();
	cout << "Load scene in editor" << endl;
	if (!this->nowScene.LoadScene(nameOfScene.c_str())) {
		cout << "ERROR. Can't load scene" << endl;
		system("pause");
	} else {
		cout << "Success scene load" << endl;
		system("pause");
	}
}
void Editor::SetNewBackground() {
	cout << "Input new color" << endl;
	glm::vec3 newColor;
	do { 
		cin >> newColor.x; cin >> newColor.y; cin >> newColor.z;
	} while (newColor.x < 0 || newColor.x> 1 || newColor.y < 0 || newColor.y>1 || newColor.z < 0 || newColor.z>1);
	this->ChangeBackground(Color(newColor.x, newColor.y, newColor.z, 1.0f));
}
void Editor::ToOrigin() {
	this->picker = glm::vec3(0.0f);
	if (!this->nowScene.IsEmpty()) nowScene.ResetSelectedObject();
	if (!nowModel.IsEmpty()) {
		nowModel.SetVector(M_SCALE_VEC, glm::vec3(0.2f));
		nowModel.SetFloat(M_OX_ANGLE, 0.0f);
		nowModel.SetFloat(M_OY_ANGLE, 0.0f);
		nowModel.SetFloat(M_OZ_ANGLE, 0.0f);
	}
}