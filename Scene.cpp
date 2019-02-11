#include "stdafx.h"
#include "Scene.h"

void Scene::DrawModels(ShaderWorks shader, glm::mat4 cameraMatrix, glm::vec3 cameraPos, glm::mat4 projection) {
	for (int i = 0; i < models.size(); i++) {
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 MVP = glm::mat4(1.0f);
		view = cameraMatrix;
		MVP = projection * view * this->models[i].GetMatrix(M_MODEL_MATRIX);
		shader.SetUniformMatrix("model", this->models[i].GetMatrix(M_MODEL_MATRIX));
		shader.SetInt("material.editorMode", false);
		shader.SetInt("LIGHT_NUM", lighters.size());
		if (!lighters.empty()) {
			this->AddLighters(shader);
			shader.SetInt("material.isRendered", this->models[i].GetBool(M_RENDERED));
		} else shader.SetInt("material.isRendered", false);
		if (!this->textureMode) shader.SetInt("material.isTextured", false);
		else shader.SetInt("material.isTextured", this->models[i].GetBool(M_TEXTURED));
		if (this->nowId != models[i].GetInt(M_ID))
			this->models[i].SetBool(M_SELECTED, false);
		else this->models[i].SetBool(M_SELECTED, true);
		shader.SetInt("material.isSelected", this->models[i].GetBool(M_SELECTED));
		shader.SetUniform3f("material.objColor", this->models[i].GetVector(M_COLOR));
		shader.SetUniform3f("material.specular", this->models[i].GetVector(M_SPECULAR));
		shader.SetFloat("material.shininess", this->models[i].GetFloat(M_SHININESS));
		shader.SetUniform3f("viewPos", cameraPos);
		shader.SetUniformMatrix("MVP", MVP);
		this->models[i].DrawModel(shader);
	}
	this->DrawLighters(shader, cameraMatrix, cameraPos, projection);
}
void Scene::AddLighters(ShaderWorks shader) {
	for (int i = 0; i < lighters.size(); i++) {
		string num = "light[" + std::to_string(i) + "].";
		shader.SetUniform3f((num + "position").c_str(), this->lighters[i].pos);
		shader.SetInt((num + "type").c_str(), this->lighters[i].type);
		shader.SetUniform3f((num + "color").c_str(), this->lighters[i].color);
		shader.SetUniform3f((num + "ambient").c_str(), this->lighters[i].ambient);
		shader.SetUniform3f((num + "diffuse").c_str(), this->lighters[i].diffuse);
		shader.SetUniform3f((num + "specular").c_str(), this->lighters[i].specular);
		shader.SetFloat((num + "constant").c_str(), this->lighters[i].constant);
		shader.SetFloat((num + "linear").c_str(), this->lighters[i].linear);
		shader.SetFloat((num + "quadratic").c_str(), this->lighters[i].quadratic);
	}
}
void Scene::DrawLighters(ShaderWorks shader, glm::mat4 cameraMatrix, glm::vec3 cameraPos, glm::mat4 projection) {
	for (int i = 0; i < lighters.size(); i++) {
		if (lighters[i].isViewed) {
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 MVP = glm::mat4(1.0f);
			view = cameraMatrix;
			glm::mat4 model = glm::mat4(1.0f);
			model = this->lighters[i].modelMatrix;
			MVP = projection * view * model;
			shader.SetInt("material.editorMode", true);
			if (this->nowId != this->lighters[i].model.GetInt(M_ID)) this->lighters[i].model.SetBool(M_SELECTED, false);
			else this->lighters[i].model.SetBool(M_SELECTED, true);
			shader.SetInt("material.isSelected", this->lighters[i].model.GetBool(M_SELECTED));
			shader.SetInt("material.isTextured", false);
			shader.SetInt("material.isRendered", false);
			shader.SetUniform3f("material.objColor", this->lighters[i].color);
			shader.SetUniform3f("viewPos", cameraPos);
			shader.SetUniformMatrix("model", model);
			shader.SetUniformMatrix("MVP", MVP);
			this->lighters[i].model.DrawModel(shader);
		}
	}
}
void Scene::ClearAllAboutScene() {
	for (int i = 0; i < models.size(); i++) models[i].ClearModel();
	models.clear();
	for (int i = 0; i < lighters.size(); i++) lighters[i].model.ClearModel();
	lighters.clear();
	this->name = "";
	this->countOfObjects = 0;
}
void Scene::ModelsSerialization(ofstream &file) {
	unsigned int count = models.size();
	file.write((char*)&count, sizeof(unsigned int));
	for (int i = 0; i < models.size(); i++) {
		string modelName = models[i].GetString(M_NAME).c_str();
		count = modelName.size();
		file.write(modelName.c_str(), count);
		file.write("\0", sizeof(char));
		int id = models[i].GetInt(M_ID);
		file.write((char*)&id, sizeof(unsigned int));
		bool val = models[i].GetBool(M_RENDERED);
		file.write((char*)&val, sizeof(bool));
		val = models[i].GetBool(M_TEXTURED);
		file.write((char*)&val, sizeof(bool));
		glm::vec3 vec = models[i].GetVector(M_COLOR);
		file.write((char*)&vec, sizeof(glm::vec3));
		vec = models[i].GetVector(M_SPECULAR);
		file.write((char*)&vec, sizeof(glm::vec3));
		vec = models[i].GetVector(M_POSITION);
		file.write((char*)&vec, sizeof(glm::vec3));
		glm::mat4 mat = models[i].GetMatrix(M_MODEL_MATRIX);
		file.write((char*)&mat, sizeof(glm::mat4));
		float shine = models[i].GetFloat(M_SHININESS);
		file.write((char*)&shine, sizeof(float));
	}
}
void Scene::ModelsDeserialization(ifstream &file) {
	unsigned int count;
	file.read((char*)&count, sizeof(unsigned int));
	for (int i = 0; i < count; i++) {
		Model model;
		string modelName;
		getline(file, modelName, '\0');
		cout << "Load model with name " << modelName << endl;
		if (this->GetModelIdByName(modelName) != -1) {
			unsigned int id = this->GetModelIdByName(modelName);
			model = this->GetModelById(id);
		} else {
			model.LoadFromFile("models/" + modelName + ".obj");
		}
		model.SetString(M_NAME, modelName);
		unsigned int id;
		file.read((char*)&id, sizeof(unsigned int));
		model.SetInt(M_ID, id);
		bool val;
		file.read((char*)&val, sizeof(bool));
		model.SetBool(M_RENDERED, val);
		file.read((char*)&val, sizeof(bool));
		model.SetBool(M_TEXTURED, val);
		glm::vec3 vec;
		file.read((char*)&vec, sizeof(glm::vec3));
		model.SetVector(M_COLOR, vec);
		file.read((char*)&vec, sizeof(glm::vec3));
		model.SetVector(M_SPECULAR, vec);
		file.read((char*)&vec, sizeof(glm::vec3));
		model.SetVector(M_POSITION, vec);
		glm::mat4 mat;
		file.read((char*)&mat, sizeof(glm::mat4));
		model.SetMatrix(M_MODEL_MATRIX, mat);
		float shin;
		file.read((char*)&shin, sizeof(float));
		model.SetFloat(M_SHININESS, shin);
		models.push_back(model);
	}
}
void Scene::LightersSerialization(ofstream &file) {
	unsigned int count = lighters.size();
	file.write((char*)&count, sizeof(unsigned int));
	for (int i = 0; i < lighters.size(); i++) {
		file.write((char*)&lighters[i].type, sizeof(lighters[i].type));
		string pathToLighter = lighters[i].model.GetString(M_NAME).c_str();
		count = pathToLighter.size();
		file.write(pathToLighter.c_str(), count);
		file.write("\0", sizeof(char));
		int id = lighters[i].model.GetInt(M_ID);
		file.write((char*)&id, sizeof(unsigned int));
		file.write((char*)&lighters[i].pos, sizeof(lighters[i].pos));
		file.write((char*)&lighters[i].ambient, sizeof(lighters[i].ambient));
		file.write((char*)&lighters[i].color, sizeof(lighters[i].color));
		file.write((char*)&lighters[i].diffuse, sizeof(lighters[i].diffuse));
		file.write((char*)&lighters[i].linear, sizeof(lighters[i].linear));
		file.write((char*)&lighters[i].modelMatrix, sizeof(lighters[i].modelMatrix));
		file.write((char*)&lighters[i].quadratic, sizeof(lighters[i].quadratic));
		file.write((char*)&lighters[i].specular, sizeof(lighters[i].specular));
	}
}
void Scene::LightersDeserialization(ifstream &file) {
	unsigned int count;
	file.read((char*)&count, sizeof(unsigned int));
	if (count > this->MAX_LIGHTS) count = MAX_LIGHTS;
	for (int i = 0; i < count; i++) {
		Lighter lighter;
		file.read((char*)&lighter.type, sizeof(lighter.type));
		string pathToLighterModel;
		getline(file, pathToLighterModel, '\0');
		lighter.model.SetString(M_NAME, pathToLighterModel);
		lighter.model.LoadFromFile(pathToLighterModel);
		unsigned int id;
		file.read((char*)&id, sizeof(unsigned int));
		lighter.model.SetInt(M_ID, id);
		file.read((char*)&lighter.pos, sizeof(lighter.pos));
		file.read((char*)&lighter.ambient, sizeof(lighter.ambient));
		file.read((char*)&lighter.color, sizeof(lighter.color));
		file.read((char*)&lighter.diffuse, sizeof(lighter.diffuse));
		file.read((char*)&lighter.linear, sizeof(lighter.linear));
		file.read((char*)&lighter.modelMatrix, sizeof(lighter.modelMatrix));
		file.read((char*)&lighter.quadratic, sizeof(lighter.quadratic));
		file.read((char*)&lighter.specular, sizeof(lighter.specular));
		lighters.push_back(lighter);
	}
}
void Scene::ChangeModelById(int id) {
	if (id < 0) return;
	for (int i = 0; i < models.size(); i++) {
		if (id == models[i].GetInt(M_ID)) {
			cout << "Edit model with id = " << id << endl;
			cout << "What you want to edit?" << endl;
			cout << "1 Color" << endl;
			cout << "2 Specular" << endl;
			cout << "3 Nothing" << endl;
			int select = -1;
			do {
				cin >> select;
			} while (select <= 0 && select > 3);
			if (select == 3) return;
			char ch = '|';
			glm::vec3 newVec(1.0f);
			switch (select) {
			case 1:
				cout << "Input color vector:"<<endl;
				do {
					cin >> newVec.x >> ch >> newVec.y >> ch >> newVec.z;
				} while (newVec.x < 0 && newVec.x > 1 || newVec.y < 0 && newVec.y > 1 || newVec.z < 0 && newVec.z > 1);
				models[i].SetVector(M_COLOR, newVec);
				break;
			case 2:
				cout << "Input specular vector:"<<endl;
				do {
					cin >> newVec.x >> ch >> newVec.y >> ch >> newVec.z;
				} while (newVec.x < 0 && newVec.x > 1 || newVec.y < 0 && newVec.y > 1 || newVec.z < 0 && newVec.z > 1);
				models[i].SetVector(M_SPECULAR, newVec);
				break;
			}
		}
	}
	system("CLS");
}
void Scene::ChangeLighterById(int id) {
	if (id < 0) return;
	for (int i = 0; i < lighters.size(); i++) {
		if (id == lighters[i].model.GetInt(M_ID)) {
			cout << "Edit lighter with id = " << id << endl;
			cout << "What you want to edit?" << endl;
			cout << "1 Color" << endl;
			cout << "2 Ambient" << endl;
			cout << "3 Diffuse" << endl;
			cout << "4 Specular" << endl;
			cout << "5 Nothing" << endl;
			int select = -1;
			do {
				cin >> select;
				select;
			} while (select <= 0 || select > 5);
			if (select == 5) return;
			char ch = '|';
			glm::vec3 newVec(1.0f);
			switch (select) {
			case 1:
				cout << "Input color vector:" << endl;
				do {
					cin >> newVec.x;
					cin >> newVec.y;
					cin >> newVec.z;
				} while ((newVec.x < 0 || newVec.x > 1) || (newVec.y < 0 || newVec.y > 1) || (newVec.z < 0 || newVec.z > 1));
				lighters[i].color = newVec;
				break;
			case 2:
				cout << "Input ambient vector:" << endl;
				do {
					cin >> newVec.x;
					cin >> newVec.y;
					cin >> newVec.z;
				} while ((newVec.x < 0 || newVec.x > 1) || (newVec.y < 0 || newVec.y > 1) || (newVec.z < 0 || newVec.z > 1));
				lighters[i].ambient = newVec;
				break;
			case 3:
				cout << "Input diffuse vector:" << endl;
				do {
					cin >> newVec.x;
					cin >> newVec.y;
					cin >> newVec.z;
				} while ((newVec.x < 0 || newVec.x > 1) || (newVec.y < 0 || newVec.y > 1) || (newVec.z < 0 || newVec.z > 1));
				lighters[i].diffuse = newVec;
				break;
			case 4:
				cout << "Input specular vector:" << endl;
				do {
					cin >> newVec.x;
					cin >> newVec.y;
					cin >> newVec.z;
				} while ((newVec.x < 0 || newVec.x > 1) || (newVec.y < 0 || newVec.y > 1) || (newVec.z < 0 || newVec.z > 1));
				lighters[i].specular = newVec;
				break;
			}
		}
	}
	system("CLS");
}
bool Scene::SaveScene(const char *sceneName) {
	cout << "Save scene with name " << sceneName << endl;
	if (this->IsEmpty()) {
		cout << "Scene are empty. Nothing to save" << endl;
		return false;
	}
	string filepath = "scenes/" + (string)sceneName + ".sc";
	ofstream file;
	file.open(filepath, ios::out | ios::binary);
	if (!file.is_open()) {
		cout << "Can't create/open file " + filepath << endl;
		return false;
	}
	unsigned int count = string(sceneName).size();
	file.write(sceneName, count);
	file.write("\0", sizeof(char));
	file.write((char*)&countOfObjects, sizeof(unsigned int));
	file.write((char*)&isAnimated, sizeof(bool));
	this->ModelsSerialization(file);
	this->LightersSerialization(file);
	file.close();
	this->name = sceneName;
	cout << "Scene save success on path " << filepath << endl;
	return true;
}
bool Scene::LoadScene(const char *sceneName) {
	cout << "Load scene with name " << sceneName << endl;
	string filepath = "scenes/" + (string)sceneName + ".sc";
	ifstream file(filepath, ios::in | ios::binary);
	if (!file.is_open()) {
		cout << "Can't open file to load" << endl;
		return false;
	}
	this->ClearAllAboutScene();
	unsigned int count;
	getline(file, this->name, '\0');
	cout << "read name = " << this->name << endl;
	file.read((char*)&this->countOfObjects, sizeof(unsigned int));
	cout << "read countOfObjecs = " << this->countOfObjects << endl;
	file.read((char*)&this->isAnimated, sizeof(bool));
	cout << "read isAnimated = " << this->isAnimated << endl;
	this->ModelsDeserialization(file);
	this->LightersDeserialization(file);
	cout << "end loading" << endl;
	file.close();
	return true;
}
int Scene::CheckObjectById(unsigned int id) {
	if (id < 0 || id > this->countOfObjects) return -1;
	for (int i = 0; i < models.size(); i++)
		if (models[i].GetInt(M_ID) == id) return 1;
	for (int i = 0; i < lighters.size(); i++)
		if (lighters[i].model.GetInt(M_ID) == id) return 2;
	return -1;
}
Model Scene::GetModelById(unsigned int id) {
	if (id > this->countOfObjects) {
		cout << "ID ERROR. Id is oversized. countOfObject = " << this->countOfObjects << " id = " << id << endl;
		system("pause");
		exit(1);
		return nullptr;
	}
	for (int i = 0; i < models.size(); i++) if (this->models[i].GetInt(M_ID) == id) return this->models[i];
	cout << "Can't find model by ID" << endl;
	return nullptr;
}
Lighter Scene::GetLighterById(unsigned int id) {
	if (id > this->countOfObjects) {
		cout << "ID ERROR. Id is oversized. countOfObjects = " << this->countOfObjects << " id = " << id << endl;
		system("pause");
		exit(1);
	}
	if (!lighters.empty()) {
		for (int i = 0; i < lighters.size(); i++) {
			if (this->lighters[i].model.GetInt(M_ID) == id) {
				return this->lighters[i];
			}
		}
	}	
}
void Scene::DeleteById(unsigned int id) {
	if (countOfObjects == 0) return;
	for (int i = 0; i < models.size(); i++) {
		if (models[i].GetInt(M_ID) == id) {
			models[i].ClearModel();
			models.erase(models.begin() + i);
		}
	}
	for (int i = 0; i < lighters.size(); i++) {
		if (lighters[i].model.GetInt(M_ID) == id) {
			lighters[i].model.ClearModel();
			lighters.erase(lighters.begin() + i);
		}
	}
	this->RecountAllId();
	this->ResetSelectedObject();
}
void Scene::PlaceModelOnScene(Model model) {
	model.SetInt(M_ID, countOfObjects++);
	cout << "ID for model = " << countOfObjects - 1 << endl;
	models.push_back(model);
	cout << "Model are placed on scene" << endl;
}
void Scene::PlaceLighterOnScene(Lighter lighter) {
	if (lighters.size() >= this->MAX_LIGHTS) {
		cout << "Can't place more lighters, max lighters = " << this->MAX_LIGHTS << endl;
		return;
	}
	lighter.model.SetInt(M_ID, countOfObjects++);
	cout << "ID for lighter = " << countOfObjects - 1 << endl;
	lighters.push_back(lighter);
	this->DisplayNumOfLights();
	if (this->nowViewed) this->SetLightersViewed(true);
	else this->SetLightersViewed(false);
}
void Scene::EditSelected() {
	if (this->nowId < 0) return;
	int check = this->CheckObjectById(this->nowId);
	if (check == 1) {
		this->ChangeModelById(this->nowId);
	} else if (check == 2) {
		this->ChangeLighterById(this->nowId);
	} else {
		cout << "Can't find object" << endl;
		return;
	}
}
void Scene::ClearScene() {
	if (this->IsEmpty()) return;
	if (!models.empty()) {
		for (int i = 0; i < models.size(); i++) models[i].ClearModel();
		models.clear();
	}
	if (!lighters.empty()) {
		for (int i = 0; i < lighters.size(); i++)
			if (!lighters[i].isEmpty())
				lighters[i].model.ClearModel();
		lighters.clear();
	}
	this->ResetSelectedObject();
	this->countOfObjects = 0;
}