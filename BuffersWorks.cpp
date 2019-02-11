#include "stdafx.h"
#include "BuffersWorks.h"

BufferWorks::BufferWorks() {
	cout << "BufferWorks constructor" << endl;
}
BufferWorks::~BufferWorks() {
	if (vert) {
		delete[] vert;
		vert = nullptr;
	}
	if (idx) {
		delete[] idx;
		idx = nullptr;
	}
}
void BufferWorks::SetupScene(GLenum polygonSideDraw,GLenum polygonDrawMode, Color background) {
	glEnable(GL_DEPTH_TEST);
	glClearColor(background.red, background.green, background.blue, background.alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glPolygonMode(polygonSideDraw, polygonDrawMode);
}
void BufferWorks::VertsInBuffer(float *arr, int size) {
	if (vert) {delete[] vert; vert = nullptr;}
	vert = new float[size];
	for (int i = 0; i < size; i++) {vert[i] = arr[i];}
	PutVertsIntoBuffer(vert, size);
}
void BufferWorks::IdxInBuffer(GLuint *arr, int size) {
	if (idx) {delete[] idx; idx = nullptr;}
	idx = new GLuint[size];
	for (int i = 0; i < size; i++) {idx[i] = arr[i];}
	PutIdxIntoBuffer(idx, size);
}