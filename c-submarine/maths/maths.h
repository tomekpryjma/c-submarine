#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

#define M_2_PI (M_PI * 2)
#define DEG_TO_RAD (M_PI / 180)
#define RAD_TO_DEG (180 / M_PI)

// Vector
typedef struct {
	float x;
	float y;
} Vector;

Vector Vector_DirectionTo(const Vector* vec1, const Vector* vec2);
Vector Vector_Normalize(const Vector* vec);
float Vector_MagnitudeF(float a, float b);
float Vector_Magnitude(const Vector* vec1, const Vector* vec2);

// Range
int Range_MappedToRange(int input, int a1, int a2, int b1, int b2);

// General maths functions
float M_Lerp(float start, float end, float t);