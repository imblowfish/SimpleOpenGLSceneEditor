#pragma once
#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <cstdlib>
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "ShaderWorks.h"
using namespace std;
struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
};
struct Texture {
	int id;
	string path;
	string type;
};

class Mesh {
private:
	GLuint vbo, ebo;
	void SetupMesh();
public:
	GLuint vao;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->SetupMesh();
	}
	~Mesh() {cout << "Mesh destructor" << endl;}
	//очистка буферов
	void DeleteBuffers() {
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
	void ClearMesh() {
		vertices.clear();
		indices.clear();
		textures.clear();	
	}
	void DrawMesh(ShaderWorks shader);
};

#endif