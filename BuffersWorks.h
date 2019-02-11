#pragma once
#ifndef BUFFER_WORKS
#define BUFFER_WORKS
#include "Color.h"
#include "ShaderWorks.h"
using namespace std; 
class BufferWorks {
private:
	float *vert;																			
	GLuint *idx;		
	void PutVertsIntoBuffer(float *arr, int size) {glBufferData(GL_ARRAY_BUFFER, size * sizeof(arr), arr, GL_STATIC_DRAW);}
	void PutIdxIntoBuffer(GLuint *arr, int size){glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(arr), arr, GL_STATIC_DRAW);}
public:
	void SetupScene(GLenum polygonSideDraw, GLenum polygonDrawMode, Color background);         
	void VertsInBuffer(float *arr, int size);													
	void IdxInBuffer(GLuint *arr, int size);													
	void DrawIdx(GLenum drawMode, int numOfArrElements, GLenum typeOfIdxArr, GLuint *idxArr);	
	void DrawVerts(GLenum drawMode, int beginShift, int numOfVertices);				
	BufferWorks();
	~BufferWorks();
};
#endif BUFFER_WORKS