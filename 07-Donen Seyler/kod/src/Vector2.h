#pragma once

class Vector2
{
public:
	Vector2(float x_, float y_);
	~Vector2();

	float x, y;

	[[nodiscard]] Vector2 operator+ (const Vector2& b);
	[[nodiscard]] Vector2 operator- (const Vector2& b);
	void operator* (float s);
	void operator/ (float s);

	void normalize();
	void dot();
	void cross();
	void length();


private:

};

