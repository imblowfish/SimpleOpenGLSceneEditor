#pragma once
#ifndef COLOR
#define COLOR
struct Color {
	float red;
	float green;
	float blue;
	float alpha;
	Color(float r, float g, float b, float a) :red(r), green(g), blue(b), alpha(a) {}
	Color() :red(0.0f), green(0.0f), blue(0.0f), alpha(0.0f) {}
};
#endif COLOR