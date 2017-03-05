#pragma once
#include <iostream>
#include <cmath>

/*
Лучше вместо define использовать 
const float PI = 3.15159265f;

Просто в чистом си нельзя было так написать.
*/
#define PI 3.14159265

struct Vector2 {
	float x;
	float y;
	Vector2(float x, float y);
	Vector2();
	Vector2 operator * (float k) const;
	Vector2 operator / (float k) const;
	Vector2 operator - () const;
	Vector2 operator + (const Vector2& other) const;
	Vector2& operator += (const Vector2& other);
	Vector2 operator - (const Vector2& other) const;
	Vector2& operator -= (const Vector2& other);
	float operator ^ (const Vector2& other) const;
	float operator * (const Vector2& other) const;
	float len() const;
	float squareLen() const;
	Vector2 perpendicular() const;
	Vector2 norm() const;
	Vector2& rotate(float degree);
	Vector2 getRotated(float degree) const;
};

Vector2 operator*(float k, const Vector2& v);

std::ostream& operator <<(std::ostream& stream, const Vector2& v);
std::istream& operator >> (std::istream& stream, Vector2& v);
