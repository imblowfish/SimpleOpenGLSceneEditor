#version 330 core
#define MAX_LIGHT_NUM 10
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 color;

uniform vec3 viewPos;
uniform sampler2D ourTexture;
uniform int LIGHT_NUM;

struct Light{
	vec3 position;
	vec3 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
	int type;
};

struct Material{
	int id;
	sampler2D diffuse;
	vec3 specular;
	float shininess;
	vec3 objColor;
	int isTextured;
	int isRendered;
	int editorMode;
	int isSelected;
};

uniform Light light[MAX_LIGHT_NUM];
uniform Material material;

vec3 CalcLightText(Light light, vec3 normal, vec3 fragPos, vec3 vPos);
vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 vPos);
vec3 CalcDiffuseLight(Light light, vec3 normal, vec3 fragPos, vec3 vPos);

void main(){
	vec3 allResult;
	if(material.isRendered == 1){
		for(int i=0;i<LIGHT_NUM;i++){
		vec3 result;
		if(light[i].type == 2){
			if(material.isTextured == 1) result = CalcLightText(light[i], Normal, FragPos, viewPos);
			else result = CalcLight(light[i], Normal, FragPos, viewPos);
			result += CalcDiffuseLight(light[i], Normal, FragPos, viewPos);
		}else if(light[i].type == 1){
			if(material.isTextured == 1) result = CalcLightText(light[i], Normal, FragPos, viewPos);
			else result = CalcLight(light[i], Normal, FragPos, viewPos);
		}else{
			result = CalcDiffuseLight(light[i], Normal, FragPos, viewPos);
		}
		if(i==0) allResult=result;
		else allResult+=result;
		}
		if(material.isSelected == 1){
			color = vec4(vec3(0.0f, 0.6f, 1.0f), 0.3f);
		}else{
			if(material.isTextured == 1) color = vec4(allResult * vec3(texture(material.diffuse, TexCoord)), 1.0f);
			else color = vec4(allResult * material.objColor, 1.0f);
		}
	}else{
		if(material.editorMode == 1){
			if(material.isSelected == 1){
				color = vec4(vec3(0.0f, 0.6f, 1.0f), 0.3f);
			}else{
				if(material.isTextured == 1) color = vec4(vec3(1.0f) * vec3(texture(material.diffuse, TexCoord)), 0.3f);
				else color = vec4(material.objColor, 0.3f);
			}
			
		}else{
			if(material.isSelected == 1){
				color = vec4(vec3(0.0f, 0.6f, 1.0f), 0.3f);
			}else{
				if(material.isTextured == 1) color = vec4(vec3(1.0f) * vec3(texture(material.diffuse, TexCoord)), 1.0f);
				else color = vec4(material.objColor, 1.0f);
			}
		}
	}
}

vec3 CalcDiffuseLight(Light light, vec3 normal, vec3 fragPos, vec3 vPos){
	//фоновый
	float ambientStrength = 0.1f;
	light.ambient = ambientStrength * vec3(1.0f);
	//направленный
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diffCoeff = max(dot(norm, lightDir), 0.0);
	light.diffuse = diffCoeff * light.color;
	//бликовый
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(vPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specComp = pow(max(dot(viewDir, reflectDir),0.0),32);
	light.specular = specularStrength * specComp * light.color;
	return (light.ambient + light.diffuse + light.specular);
}

vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 vPos){
	//затухание
	float distance = length(light.position-fragPos);
	float attenuation = 1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	//фоновый
	float ambientStrength = 0.1f;
	//light.ambient = ambientStrength * light.color;
	//материалы
	vec3 ambient = ambientStrength * light.color;
	//направленный
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diffCoeff = max(dot(norm, lightDir), 0.0);
	//light.diffuse = diffCoeff * light.color;
	//материалы
	vec3 diffuse = diffCoeff * light.color;
	//бликовый
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(vPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	//float specComp = pow(max(dot(viewDir, reflectDir),0.0),32);
	float specComp = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);
	//light.specular  = specularStrength * specComp * light.color;
	//материалы
	vec3 specular =  specularStrength * specComp * light.color;

	//light.ambient *= attenuation;
	//light.diffuse *= attenuation;
	//light.specular *= attenuation;
	//материалы
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	//return (light.ambient + light.diffuse + light.specular);
	//материалы
	return (ambient + diffuse + specular);
}

vec3 CalcLightText(Light light, vec3 normal, vec3 fragPos, vec3 vPos){
	//затухание
	float distance = length(light.position - fragPos);
	float attenuation = 1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	//фоновый
	vec3 ambient = light.ambient * light.color *  vec3(texture(material.diffuse, TexCoord));
	//направленный
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diffCoeff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * light.color * diffCoeff * vec3(texture(material.diffuse, TexCoord));
	//бликовый
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(vPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specComp = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);
	vec3 specular = light.specular * light.color *  specComp * material.specular;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;	
	return (ambient + diffuse + specular);
}