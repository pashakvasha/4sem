#include <iostream>
#include "Vector2.h"
#include <cmath>

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2()
{

}

Vector2 Vector2::operator*(float k) const
{
	return Vector2(x * k, y * k);
}

Vector2 operator*(float k, const Vector2& v)
{
	return Vector2(k * v.x, k * v.y);
}

Vector2 Vector2::operator/(float k) const
{
	return Vector2(x / k, y / k);
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2  Vector2::operator+ (const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2&  Vector2::operator += (const Vector2& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2 Vector2::operator- (const Vector2& other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2&  Vector2::operator -= (const Vector2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

float Vector2::operator^ (const Vector2& other) const
{
	return x * other.y - y * other.x;
}

float  Vector2::operator* (const Vector2& other) const
{
	return x * other.x + y * other.y;
}

float  Vector2::len() const
{
	return sqrt(x * x + y * y);
}

float  Vector2::squareLen() const
{
	return x * x + y * y;
}

Vector2 Vector2::perpendicular() const
{
	return Vector2(-y, x);
}

Vector2 Vector2::norm() const
{
	return Vector2(x, y) / this->len();
}

Vector2& Vector2::rotate(float degree)
{
	float x_ = x, y_ = y;
	float angle = degree / 180 * PI;
	x = x_ * cos(angle) - y_ * sin(angle);
	y = y_ * cos(angle) + x_ * sin(angle);
	return *this;
}

Vector2 Vector2::getRotated(float degree) const
{
	return Vector2(x, y).rotate(degree);
}

std::ostream& operator <<(std::ostream& stream, const Vector2& v)
{
	stream << v.x << " " << v.y;
	return stream;
}

std::istream& operator >> (std::istream& stream, Vector2& v)
{
	stream >> v.x >> v.y;
	return stream;
}