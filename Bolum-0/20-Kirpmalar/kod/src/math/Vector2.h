#pragma once

#include <cmath>

class Vector2
{
public:
	Vector2();
	Vector2(float x_, float y_);
	~Vector2();
	
	[[nodiscard]] Vector2 operator+ (const Vector2& vec);
	[[nodiscard]] Vector2 operator- (const Vector2& vec);
	[[nodiscard]] Vector2 operator* (float s);
	[[nodiscard]] Vector2 operator/ (float s);

	void normalize();
	float dot(const Vector2& vec);
	float length();
	
	float x, y;

private:

};

