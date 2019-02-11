#include "stdafx.h"
#include "Model.h"

void Model::LoadModel(string path) {
	cout << "Load model with path:" << path << endl;
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Assimp Error:" << importer.GetErrorString() << endl;
		return;
	}
	this->ProcessNode(scene->mRootNode, scene);
	directory = "textures";
	cout << "Model load success" << endl;
}
void Model::ProcessNode(aiNode *node, const aiScene *scene) {
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, scene));
	}
	for (int i = 0; i < node->mNumChildren; i++) this->ProcessNode(node->mChildren[i], scene);
}
Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x; vector.y = mesh->mVertices[i].y; vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;
		vector.x = mesh->mNormals[i].x; vector.y = mesh->mNormals[i].y; vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x; vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		} else vertex.texCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
	}
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	vector<Texture> diffuseMaps = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	vector<Texture> specularMaps = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	return Mesh(vertices, indices, textures);
}
vector<Texture> Model::LoadMaterialTexture(aiMaterial *material, aiTextureType type, string typeName) {
	vector<Texture> textures;
	if (typeName == "texture_diffuse" && material->GetTextureCount(type) == 0 || typeName == "texure_specular" && material->GetTextureCount(type) == 0) {
		cout << "No Textures for model" << endl;
	}
	for (int i = 0; i < material->GetTextureCount(type); i++) {
		aiString str;
		material->GetTexture(type, i, &str);
		Texture texture;
		texture.id = this->TextureFromFile("textures", str);
		if (texture.id == -1 && !modelInfo.isTextured) this->modelInfo.isTextured = false;
		else this->modelInfo.isTextured = true;
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}
int Model::TextureFromFile(const char *directory, aiString filename) {
	string test = filename.C_Str();
	string path;
	if (test.find("textures") != std::string::npos || test.find("textures") != std::string::npos) path = test;
	else path = string(directory) + '/' + filename.C_Str();
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char *image = SOIL_load_image(string(path).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		SOIL_free_image_data(image);
		cout << "Success load texture " << string(path).c_str() << endl;
	} else {
		cout << "Failed to load texture " << string(path).c_str() << endl;
		SOIL_free_image_data(image);
		return -1;
	}
	return textureID;
}
bool Model::GetBool(unsigned int infoType) {
	switch (infoType) {
		case M_TEXTURED:
			return this->modelInfo.isTextured;
		break;
		case M_RENDERED:
			return this->modelInfo.isRendered;
		break;
		case M_SELECTED:
			return this->modelInfo.isSelected;
		break;
	}
	return false;
}
void Model::SetBool(unsigned int infoType, bool val) {
	switch (infoType) {
		case M_TEXTURED:
			this->modelInfo.isTextured = val;
		break;
		case M_RENDERED:
			this->modelInfo.isRendered = val;
		break;
		case M_SELECTED:
			this->modelInfo.isSelected = val;
		break;
	}
}
glm::vec3 Model::GetVector(unsigned int infoType) {
	switch (infoType) {
		case M_COLOR:
			return this->modelInfo.modelColor;
		break;
		case M_SPECULAR:
			return this->modelInfo.modelSpecular;
		break;
		case M_POSITION:
			return this->modelInfo.modelPosition;
		break;
		case M_SCALE_VEC:
			return this->modelInfo.scaleVec;
		break;
	}
}
void Model::SetVector(unsigned int infoType, glm::vec3 val) {
	switch (infoType) {
		case M_COLOR:
			this->modelInfo.modelColor = val;
		break;
		case M_SPECULAR:
			this->modelInfo.modelSpecular = val;
		break;
		case M_POSITION:
			this->modelInfo.modelPosition = val;
		break;
		case M_SCALE_VEC:
			this->modelInfo.scaleVec = val;
		break;
	}
}
glm::mat4 Model::GetMatrix(unsigned int infoType) {
	switch (infoType) {
		case M_MODEL_MATRIX:
			return this->modelInfo.modelMat;
		break;
	}
	return glm::mat4(0.0f);
}
void Model::SetMatrix(unsigned int infoType, glm::mat4 val) {
	switch (infoType) {
		case M_MODEL_MATRIX:
			this->modelInfo.modelMat = val;
		break;
	}
}
float Model::GetFloat(unsigned int infoType) {
	switch (infoType) {
		case M_SHININESS:
			return this->modelInfo.modelShininess;
		break;
		case M_OX_ANGLE:
			return this->modelInfo.OXangle;
		break;
		case M_OY_ANGLE:
			return this->modelInfo.OYangle;
		break;
		case M_OZ_ANGLE:
			return this->modelInfo.OZangle;
		break;
	}
	return -1;
}
void Model::SetFloat(unsigned int infoType, float val) {
	switch (infoType) {
		case M_SHININESS:
			this->modelInfo.modelShininess = val;
		break;
		case M_OX_ANGLE:
			this->modelInfo.OXangle = val;
		break;
		case M_OY_ANGLE:
			this->modelInfo.OYangle = val;
		break;
		case M_OZ_ANGLE:
			this->modelInfo.OZangle = val;
		break;
	}
}
void Model::SetString(unsigned int infoType, string val) {
	switch (infoType) {
		case M_NAME:
			this->modelInfo.name = val;
		break;
	}
}
string Model::GetString(unsigned int infoType) {
	switch (infoType) {
		case M_NAME:
			return this->modelInfo.name;
		break;
	}
	return "";
}
void Model::SetInt(unsigned int infoType, int val) {
	switch (infoType) {
		case M_ID:
			this->modelInfo.id = val;
		break;
	}
}
int Model::GetInt(unsigned int infoType) {
	switch (infoType) {
		case M_ID:
			return this->modelInfo.id;
		break;
	}
	return -1;
}