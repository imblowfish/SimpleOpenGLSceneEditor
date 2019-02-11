#version 330 core

layout (location = 0) in vec3 objPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 MVP;
uniform mat4 model;

void main(){
	gl_Position = MVP * vec4(objPos, 1.0f);
	FragPos = vec3(model * vec4(objPos, 1.0));
	Normal = mat3(transpose(inverse(model)))*normal;
	TexCoord = vec2(texCoord.x, texCoord.y);
}