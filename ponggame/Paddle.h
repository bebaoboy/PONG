#pragma once
struct Paddle {
	int X;
	int Y;
	int width;
	int height;
	float speed;

	int centerY() {
		return Y + height / 2;
	}
	int centerX() {
		return X + width / 2;
	}
};