#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <SDL.h>
#include <ctime>
#include <SOIL2.h>
#include "Editor.h"

bool InitSDL();							
bool InitGLEW();						
void OutSDL();							
void MainLoop();						
void SendEditorKeysPressed(float dTime);
void DrawMainMenu();
void PrintModelsList();
void PrintLightersList();
void DrawEditorMenu();
void SendEditorEvents(float deltaTime);

SDL_Window *window;										
SDL_GLContext context;									
int  keys[1024];
string vertexPathText = "shaders/vertTexture.vert";		
string fragmentPathText = "shaders/fragTexture.frag";
//шейдер для объекта с текстурой
ShaderWorks objShaderText;								
Editor editor;
enum menuModes {NOTHING, EDITOR_MODE, LOAD_SCENE_MODE, SAVE_SCENE_MODE, EXIT_MODE};
string editorMode="MOVE";								
//список моделей
vector<string> models = {
	"bed",
	"Bench",
	"Bricks_wall",
	"build",
	"bullet1",
	"bullet2",
	"car",
	"cat",
	"Chair",
	"eyeball",
	"facade",
	"fence",
	"Hazard_Unit",
	"helicopter",
	"lamb",
	"landscape",
	"men",
	"metal_slug_skfb",
	"nanosuit",
	"retrocar",
	"road",
	"table1",
	"tv",
	"women",
	"wooden_watch_tower2"
};

#endif
