#pragma once
#ifndef LIGHTS
#define LIGHTS
#include "Model.h"

enum colorLightConstants {
	RED, ORANGE, YELLOW,
	GREEN, BLUE, DBLUE,
	VIOLET, WHITE
};
enum lightType {DIFFUSE, POINT, DIFFUSE_AND_POINT};

struct Lighter {
	Model model;											
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);			
	glm::vec3 color = glm::vec3(1.0f);					
	glm::vec3 ambient = glm::vec3(0.8f);					
	glm::vec3 diffuse = glm::vec3(0.5f);
	glm::vec3 specular = glm::vec3(1.0f);
	glm::mat4 modelMatrix;
	float constant = 1.0f;									
	float linear = 0.09f;
	float quadratic = 0.032f;
	unsigned int type;										
	bool isViewed = true;									
	bool isEmpty() {return model.IsEmpty();}
};

#endif