# SimpleOpenGLSceneEditor
Simple Scene Editor based on OpenGL 3.0 width basic oppotrunities and terrible console interface
  Простой редактор сцен написанный с использованием библиотек: OpenGL 3.0, Assimp, GLEW, SOIL, GLM и SDL 2.0. Представляет из себя оконное приложение с не самым лучшим консольным интерфейсом. Позволяет добавлять на сцену объекты из заранее заготовленного, размещать точечные и направленные источники света, управлять камерой, редактировать элементы, менять цвета объектов и фона,  а также сохранять и загружать созданные сцены.
  К сожалению отсутствуют тени.
  Для размещения и редактирования объектов на сцене предусмотрены следующие клавиши:
1.	Alt+E – запуск редактора;
2.	Alt+L – загрузка модли для дальнейшего размещения на сцене;
3.	Alt+A – загрузка источника (диффузного или точечного) для дальнейшего размещения на сцене;
4.	Ctrl+S – сохранение созданной сцены;
5.	Ctrl+L – загрузка созданной ранее сцены;
6.	Ctrl+B –  изменение цвета фона в редакторе;
7.	Ctrl+X – вырезать объект из сцены и  начать его повторное размещение;
8.	Ctrl+C – очистка сцены;
9.	T – включить/отключить режим отображения текстур;
10.	 Z – включить/отключить режим отображения сетки;
11.	R -  сбросить 3D курсор к нулевой позиции, а также нулевым параметрам поворота и скалирования;
12.	Ctrl+Q – выход из приложения.
  В редакторе для управления положением, поворотом и скалированием объектов и камеры предусмотрены следующие клавиши:
1.	W/A/S/D – изменение позиции камеры;
2.	LeftArrow/RightArrow/UpArror/DownArrow – для изменения угла поворота камеры;
3.	Space – для размещения объекта на сцене;
4.	I/J/K/L/U/O – в зависимости от режима работы редактора перемещение объекта, либо его поворот;
5.	+/- –  увеличение/уменьшение объекта;
6.	8/9/0 – переключение между режимами перемещения/поворота/скалирования. 

