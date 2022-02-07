#pragma once
#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include <string>

class Pong {
	const int
		WINDOW_WIDTH = 1000,
		WINDOW_HEIGHT = 800,
		PADDLE_WIDTH = 15,
		PADDLE_HEIGHT = 175,
		PADDLE_EXTRA = 5,
		HIT_SCORE = 25,
		MISS_SCORE = 100l,

		EASY = 1989,
		MEDIUM = 3500,
		HARD = 7777;

	const float
		BALL_RADIUS = 12,
		BALL_X_VELOCITY = 300,
		BALL_Y_VELOCITY = 150,
		BALL_DESIZE = 3,
		BALL_MIN_RADIUS = 4,

		X_SPEED_UP = -1.12f,
		Y_SPEED_UP = -1.009f,
		MAX_Y_VELOCITY = 1500.f,
		MAX_X_VELOCITY = 5200.f,

		PADDLE_SPEED = 600,
		PADDLE_SPEED_UP = 1.8f,
		PADDLE_SPEED_DOWN = 0.95f,
		LEFT_BOUNCE = 1.105f,
		RIGHT_BOUNCE = 0.99f,
		
		X_SPEEDING_TICKET = 0.7f,
		Y_FOLLOWED_UP = 0.55f,
		Y_SPEEDING_TICKET = 0.78f,
		X_FOLLOWED_UP = 0.75f;

	Ball ball;
	Paddle right_paddle, left_paddle;

	int player_hit = 0,
		computer_hit = 0;

	int player_score = 300,
		computer_score = 300;

	int TARGET_SCORE = 301;

	bool just_reached_max_speed = false,
		winning = false,
		is_started = false,
		is_choosing_difficulty = false,
		is_paused = false;

	std::string winner = "";

public:
	Pong();

	void start();
	void play();

private:
	void init();
	void drawWindow();
	void moveTheBall();
	void speedingUp();
	void paddleUp(int extra);
	void paddleDown();
	void speedingTicket();
	void bouncing();
	void gameAI();
	int gameSpeed();

	void checkWinning();
	void restart();

};