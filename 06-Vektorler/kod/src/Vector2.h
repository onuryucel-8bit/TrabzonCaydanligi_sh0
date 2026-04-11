#pragma once

class Vector2
{
public:
	Vector2(float x_, float y_);
	~Vector2();

	float x, y;

	[[nodiscard]] Vector2 operator+ (const Vector2& vec);
	[[nodiscard]] Vector2 operator- (const Vector2& vec);
	void operator* (float s);
	void operator/ (float s);

	void normalize();
	void dot();
	void cross();
	float length();

	//TODO STATIC fonksiyon olarak ekleme yap

	static float length(const Vector2& vec);

private:

};

