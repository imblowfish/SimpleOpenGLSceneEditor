#include "stdafx.h"
#include "Main.h"
using namespace std;

void DrawMainMenu() {
	system("CLS");
	cout << "ALT+E to RUN EDITOR" << endl;
	cout << "ALT+L to ADD MODEL" << endl;
	cout << "ALT+A to ADD LIGHT SOURCE" << endl;
	cout << "CTRL+S to SAVE SCENE" << endl;
	cout << "CTRL+L to LOAD SCENE" << endl;
	cout << "CTRL+E to EDIT OBJECT" << endl;
	cout << "CTRL+B to CHANGE BACKGROUND COLOR" << endl;
	cout << "CTRL+X to CUT OUT SELECTED OBJECT" << endl;
	cout << "CTRL+C to CLEAR SCENE" << endl;
	cout << "T to SET TEXTURE MODE" << endl;
	cout << "Z to SET GRID MODE" << endl;
	cout << "R to RESET PICKER TO ORIGIN" << endl;
	cout << "H to HIDE/SHOW LIGHTERS ON SCENE" << endl;
	cout << "CTRL+Q to EXIT" << endl;
}
void DrawEditorMenu() {
	system("CLS");
	DrawMainMenu();
	cout << "EDITOR" << endl;
	cout << "W/A/S/D - camera movement" << endl;
	cout << "LEFT/UP/DOWN/RIGHT - camera rotate" << endl;
	cout << "SPACE - place object" << endl;
	if (editorMode == "MOVE") cout << "I/J/K/L/U/O - model/lighter movement"<< endl;
	if (editorMode == "ROTATE") cout << "I/K - OX rotating" << endl << "J/L - OY rotating" << endl << "U/O - OZ rotating" << endl;
	if (editorMode == "SCALE") cout << "PLUS/MINUS - scaling" << endl;
}
void MainLoop() {
	SDL_Event event;
	bool running = true;
	float deltaTime = 0.0f, lastFrame = 0.0f;
	DrawMainMenu();
	while (running) {
		float currentFrame = (float)SDL_GetTicks();
		deltaTime = (currentFrame - lastFrame) / 1000;
		lastFrame = currentFrame;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				keys[event.key.keysym.sym] = 1;
				if (keys[SDLK_q] && keys[SDLK_LCTRL] || keys[SDLK_ESCAPE]) running = false;
				if(editor.isRunned) SendEditorKeysPressed(deltaTime);
				//запуск редактора
				if (keys[SDLK_LALT] && keys[SDLK_e]) {
					if (!editor.isRunned) {
						editor.StartEditor();
						DrawEditorMenu();
					}
				}
			break;
			case SDL_KEYUP:
				keys[event.key.keysym.sym] = 0;
			break;
			}
		}
		if (editor.isRunned) SendEditorEvents(deltaTime);
		if (editor.isRunned) editor.DrawEditorScene(objShaderText);
		SDL_GL_SwapWindow(window);
	}
}
void PrintModelsList() {
	system("CLS");
	cout << "List of models:" << endl;
	for (int i = 0; i < models.size(); i++) cout << i + 1 << ". "+models[i] << endl;
}
void PrintLightersList() {
	system("CLS");
	cout << "1. diffuse" << endl;
	cout << "2. point" << endl;
}
void SendEditorEvents(float dTime) {
	if (!editor.isRunned) return;
	if (keys[SDLK_w]) editor.EventListener(W, dTime);
	if (keys[SDLK_s]) editor.EventListener(S, dTime);
	if (keys[SDLK_a]) editor.EventListener(A, dTime);
	if (keys[SDLK_d]) editor.EventListener(D, dTime);
	if (keys[SDLK_UP]) editor.EventListener(UP, dTime);
	if (keys[SDLK_DOWN]) editor.EventListener(DOWN, dTime);
	if (keys[SDLK_LEFT]) editor.EventListener(LEFT, dTime);
	if (keys[SDLK_RIGHT]) editor.EventListener(RIGHT, dTime);
	if (keys[SDLK_LSHIFT]) editor.EventListener(LSHIFT, dTime);
	if (keys[SDLK_TAB]) editor.EventListener(TAB, dTime);
	if (keys[SDLK_j]) editor.EventListener(J, dTime);
	if (keys[SDLK_l]) editor.EventListener(L, dTime);
	if (keys[SDLK_i]) editor.EventListener(I, dTime);
	if (keys[SDLK_k]) editor.EventListener(K, dTime);
	if (keys[SDLK_u]) editor.EventListener(U, dTime);
	if (keys[SDLK_o]) editor.EventListener(O, dTime);
}
void SendEditorKeysPressed(float deltaTime) {
	if (!editor.isRunned) return;
	//режим отображения
	if (keys[SDLK_z]) editor.EventListener(Z, deltaTime);
	//переход по объектам сцены
	if (keys[SDLK_1]) editor.PrevObject();
	if (keys[SDLK_2]) editor.NextObject();
	//изменение режимов редактора
	if (keys[SDLK_8]) {
		editor.EventListener(K_EIGHT, deltaTime); 
		editorMode = "MOVE"; 
		DrawEditorMenu();
	}
	if (keys[SDLK_9] && editorMode != "LIGHT") {
		editor.EventListener(K_NINE, deltaTime); 
		editorMode = "ROTATE";
		DrawEditorMenu();
	}
	if (keys[SDLK_0] && editorMode != "LIGHT") {
		editor.EventListener(K_ZERO, deltaTime); 
		editorMode = "SCALE"; 
		DrawEditorMenu();
	}
	//скалирование модели
	if (keys[SDLK_MINUS]) editor.EventListener(MINUS, deltaTime);
	if (keys[SDLK_EQUALS]) editor.EventListener(PLUS, deltaTime);
	//расположение объекта
	if (keys[SDLK_SPACE]) editor.EventListener(SPACE, deltaTime);
	//отображение источников света в сцене
	if (keys[SDLK_h]) editor.EventListener(H, deltaTime);
	//сброс положения модели к нулю и отключить любые выборы модели
	if (keys[SDLK_r] && editor.isRunned) editor.EventListener(R, deltaTime);
	//загрузка выбранной модели
	if (keys[SDLK_LALT] && keys[SDLK_l]) {
		if (!editor.isRunned) {
			cout << "ERROR. EDITOR NOT RUNNED" << endl;
			system("pause");
			DrawMainMenu();
			return;
		}
		unsigned int modelNum = 0;
		//вывод списка моделей
		PrintModelsList();
		cout << "input num of model" << endl;
		while (modelNum <= 0 || modelNum > models.size()) {
			cin >> modelNum;
		}
		string modelPath = models[modelNum - 1];
		editor.LoadModel(modelPath.c_str());
		editorMode = "MODEL";
		DrawEditorMenu();
	}
	//загрузка источника света
	if (keys[SDLK_LALT] && keys[SDLK_a]) {
		if (!editor.isRunned) {
			cout << "ERROR. EDITOR NOT RUNNED" << endl;
			system("pause");
			DrawMainMenu();
			return;
		}
		unsigned int lighterNum = 0;
		//вывод списка моделей
		PrintLightersList();
		cout << "input num of lighter source" << endl;
		while (lighterNum <= 0 || lighterNum >2) {
			cin >> lighterNum;
		}
		editor.LoadLighter(lighterNum - 1);
		editor.EventListener(K_EIGHT, deltaTime);
		editorMode = "LIGHT";
		DrawEditorMenu();
	}
	if (keys[SDLK_LCTRL] && keys[SDLK_s]) {
		if (!editor.isRunned) {
			cout << "Editor are not runned. Please run editor." << endl;
			system("pause");
			DrawEditorMenu();
			return;
		}
		if (editor.NowSceneEmpty()) {
			cout << "Scene are empty. Nothing to save" << endl;
			system("pause");
			DrawEditorMenu();
			return;
		}
		string nameOfScene;
		cout << "please, input name of scene to save:";
		cin >> nameOfScene;
		editor.SaveSceneFromEditor(nameOfScene);
		DrawEditorMenu();
	}
	if (keys[SDLK_LCTRL] && keys[SDLK_l]) {
		string nameOfScene;
		cout << "please, input name of scene to load:";
		cin >> nameOfScene;
		if (!editor.isRunned) editor.StartEditor();
		editor.LoadSceneInEditor(nameOfScene);
		DrawEditorMenu();
	}
	//очистка сцены
	if (keys[SDLK_LCTRL] && keys[SDLK_c]) {
		editor.Clear();
	}
	//режим отображения текстур
	if (keys[SDLK_t]) editor.SetTextureMode();
	//редактирование выбранного объекта
	if (keys[SDLK_LCTRL] && keys[SDLK_x]) editor.CutObject();
	if (keys[SDLK_LCTRL] && keys[SDLK_e]) {
		editor.EditSelectedObject();
		DrawEditorMenu();
	}
	if (keys[SDLK_LCTRL] && keys[SDLK_b]) {
		editor.SetNewBackground();
		DrawEditorMenu();
	}
}
int main(int argc, char *argvp[]) {
	if (!InitSDL()) return -1;
	if (!InitGLEW()) return -1;
	if (!objShaderText.LoadShader(vertexPathText, fragmentPathText)) return -1;			//шейдера объекта с текстурой
	cout << "OpenGL version:" << glGetString(GL_VERSION) << endl;
	MainLoop();
	OutSDL();
	return 0;
}
bool InitSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init Error:" << SDL_GetError() << endl;
		getchar();
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);	//функция настройки атрибутов окна OpenGL до создания окна
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);							//устанавливаем мажор и минор версию 3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);										//минимальное число бит для красного канала(по-умолчанию 3)
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);										//минимальное число бит для зеленого канала(по-умолчанию 3)
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);										//минимальное число бит для синего канала(по-умолчанию 3)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);										//минимальное число бит для буфера глубины
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);									//двойная буферизация
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);									//буфер трафарета
																					//передаем окну флаг OpenGL, т.е. создаем окно OpenGL
	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, editor.GetScreenWidth(), editor.GetScreenHeight(), SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		getchar();
		SDL_Quit();
		return false;
	}
	context = SDL_GL_CreateContext(window);
	if (context == nullptr) {
		cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << endl;
		getchar();
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	//для поддержки альфа-канала
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	return true;
}
bool InitGLEW() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "glewInit Error" << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	return true;
}
void OutSDL() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}