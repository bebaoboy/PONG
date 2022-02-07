#include "PongServer.h"
#include <iostream>

Pong::Pong()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	/*ball = Ball{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f, 
		BALL_RADIUS, BALL_X_VELOCITY, BALL_Y_VELOCITY };*/
	init();
}

void Pong::start()
{	
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PONG GAME");
	SetWindowState(FLAG_VSYNC_HINT);
	SetTargetFPS(60);

	while (!WindowShouldClose()) 
	{
		if (IsKeyPressed(' ')) is_started = true;
		play();
	}
}

void Pong::play()
{
	if (is_started && !is_choosing_difficulty) {
		DrawText("Easy(E), Medium(M), Hard(H)", WINDOW_WIDTH / 2 - MeasureText("Easy(E), Medium(M), Hard(H)", 40) + 300,
			WINDOW_HEIGHT - 200, 40, WHITE);
		if (IsKeyPressed('E')) {
			TARGET_SCORE = EASY;
			is_choosing_difficulty = true;
		}
		else if (IsKeyPressed('M')) {
			TARGET_SCORE = MEDIUM;
			is_choosing_difficulty = true;
		}
		else if (IsKeyPressed('H')) {
			TARGET_SCORE = HARD;
			is_choosing_difficulty = true;
		}
	}

	if (is_started && is_choosing_difficulty)
	{

		if (!winning && IsKeyPressed('P')) {
			is_paused = true;
		}
		if (is_paused && IsKeyPressed(' ')) {
			is_paused = false;
		}
		if (!is_paused) {
			moveTheBall();
			if (IsKeyPressed('Q')) {
				is_started = false;
				restart();
			}
		}
	}

	drawWindow();

	if (!winning)
		checkWinning();
	else {
		if (winner == "Computer")
			DrawText("Computer won!!!", WINDOW_WIDTH / 2 - MeasureText("Computer won!!!", 60) + 230,
				WINDOW_HEIGHT / 2 - 30, 60, YELLOW);
		else
			DrawText("Player won!!!", WINDOW_WIDTH / 2 - MeasureText("Player won!!!", 60) + 190,
				WINDOW_HEIGHT / 2 - 30, 60, GREEN);
		DrawText("Press R to restart", WINDOW_WIDTH / 2 - MeasureText("Press R to restart", 40) + 200,
			WINDOW_HEIGHT - 200, 40, WHITE);
		if (IsKeyPressed('R')) {
			restart();
		}
	}
}

Rectangle getRec(Paddle p) {
	return Rectangle{ float(p.X), float(p.Y), 
		float(p.width), float(p.height) };
}

void Pong::init()
{
	left_paddle = Paddle{ 20, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2,
			PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED };
	right_paddle = Paddle{ WINDOW_WIDTH - 20 - PADDLE_WIDTH,
			WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2,
			PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED };

	int x_v = rand() % (5 + 5 - 1) - 5,
		y_v = rand() % (5 + 5 - 1) - 5;

	if (x_v == 0) x_v++;
	if (y_v == 0) y_v++;

	this->ball = Ball{ WINDOW_WIDTH / 2.f + (rand() % (100 + 10 - 1) - 10),
		   WINDOW_HEIGHT / 2.f + (rand() % (100 + 10 - 1) - 10),
		   BALL_RADIUS,
		   x_v / abs(x_v) * (BALL_X_VELOCITY),
		   y_v / abs(y_v) * (BALL_Y_VELOCITY) };
}

void Pong::drawWindow()
{
	if (!winning)
		right_paddle.Y = GetMouseY();
	
	BeginDrawing();
		ClearBackground(BLACK);

		if (is_started) {
			// draw the ball
			if (!is_paused && is_choosing_difficulty) {
				DrawCircle(int(ball.X), int(ball.Y), ball.radius, WHITE);
			
				std::string text = "Target score: " + std::to_string(TARGET_SCORE);
				DrawText(text.c_str(), WINDOW_WIDTH / 2 - MeasureText(text.c_str(), 40) + 200,
					WINDOW_HEIGHT - 100, 35, WHITE);

				DrawText("Press Q to quit, P to pause", WINDOW_WIDTH / 2 - MeasureText("Press Q to quit, p to pause", 40) + 300,
					WINDOW_HEIGHT - 50, 35, WHITE);
			}

			if (is_paused) {
				DrawText("PAUSED, press Space to continue", WINDOW_WIDTH / 2 - MeasureText("PAUSED, press Space to continue", 40) + 300,
					WINDOW_HEIGHT - 200, 40, WHITE);
			}

			// draw two paddles

			DrawRectangle(left_paddle.X, left_paddle.Y,
				left_paddle.width, left_paddle.height, WHITE);
			DrawRectangle(right_paddle.X, right_paddle.Y,
				right_paddle.width, right_paddle.height, WHITE);
			DrawText("Computer: ", WINDOW_WIDTH / 2  / 2 + 40, 10, 30, YELLOW);
			DrawText(std::to_string(computer_hit).c_str(),
				WINDOW_WIDTH / 2 / 2 + 190, 10, 30, YELLOW);
			DrawText("Player: ", WINDOW_WIDTH / 2 + 155, 10, 30, GREEN);
			DrawText(std::to_string(player_hit).c_str(),
				WINDOW_WIDTH / 2 + 270, 10, 30, GREEN);

			DrawText(std::to_string(computer_score).c_str(),
				WINDOW_WIDTH / 2 / 2 + 100, 40, 30, YELLOW);
			DrawText(std::to_string(player_score).c_str(),
				WINDOW_WIDTH / 2 + 200, 40, 30, GREEN);
		}
		else {
			DrawText("Press Space to play", WINDOW_WIDTH / 2 - MeasureText("Press Space to play", 40) + 200,
				WINDOW_HEIGHT - 200, 40, WHITE);
		}

		DrawFPS(10, 10);

	EndDrawing();
}

void Pong::moveTheBall()
{
	gameAI();

	speedingUp();

	speedingTicket();
		
	ball.X += ball.x_velocity * GetFrameTime();
	ball.Y += ball.y_velocity * GetFrameTime();

	if (ball.Y < 0) 
		ball.Y = ball.radius;
	if (ball.X < 0)
		ball.X = ball.radius;

	std::cout << ball.y_velocity << " " << ball.Y << std::endl;
	std::cout << ball.x_velocity << " " << ball.X << std::endl;
	if (just_reached_max_speed)
		std::cout << "MAXXXXXXXXXXXXXXXx" << std::endl;
	std::cout << std::endl;
}

void Pong::speedingUp()
{
	if ((ball.Y <= ball.radius && ball.y_velocity < 0) ||
		(ball.Y > WINDOW_HEIGHT - ball.radius && ball.y_velocity > 0)
		) {
		ball.y_velocity *= Y_SPEED_UP;
		left_paddle.speed *= PADDLE_SPEED_UP;
	}

	else if ((ball.X <= ball.radius && ball.x_velocity < 0) ||
		(ball.X > WINDOW_WIDTH - ball.radius && ball.x_velocity > 0)
		) {
		ball.x_velocity *= X_SPEED_UP;
		left_paddle.speed *= PADDLE_SPEED_UP;
		paddleUp(PADDLE_EXTRA);

		if (ball.X > WINDOW_HEIGHT / 2)
			player_score -= MISS_SCORE * gameSpeed();
		else
			computer_score -= MISS_SCORE * gameSpeed() * 3;
	}

	else {
		bouncing();
	}

}

void Pong::paddleUp(int extra)
{
	left_paddle.Y -= PADDLE_EXTRA / 3;
	left_paddle.height += PADDLE_EXTRA / 3;
	right_paddle.Y -= PADDLE_EXTRA / 3;
	right_paddle.height += PADDLE_EXTRA / 3;
}

void Pong::paddleDown()
{
	left_paddle.height = right_paddle.height = PADDLE_HEIGHT;
}

void Pong::speedingTicket()
{
	if (abs(ball.x_velocity) > MAX_X_VELOCITY
	|| (just_reached_max_speed && abs(ball.x_velocity) <= MAX_X_VELOCITY && abs(ball.x_velocity) > BALL_X_VELOCITY)) {
		ball.x_velocity *= X_SPEEDING_TICKET;
		std::cout << "X DOWWNNNNNNNNNNNNNN" << std::endl;

		just_reached_max_speed = true;
		if (abs(ball.y_velocity) > BALL_Y_VELOCITY) {
			ball.y_velocity *= Y_FOLLOWED_UP;
			std::cout << "Y DOWWNNNNNNNNNNNNNN" << std::endl;
		}		
		left_paddle.speed *= PADDLE_SPEED_DOWN;
		paddleUp(-PADDLE_EXTRA);
	}
	else {
		if (just_reached_max_speed) {
			paddleDown();
			std::cout << "ENDDDDDDDDDDDDDDDDDDDDDD MAXXXXXXXXXXXXXXXx" << std::endl;
			ball.radius -= BALL_DESIZE;
		}
		just_reached_max_speed = false;
	}

	if (abs(ball.y_velocity) > MAX_Y_VELOCITY
		|| (just_reached_max_speed && abs(ball.y_velocity) <= MAX_Y_VELOCITY && abs(ball.y_velocity) > BALL_Y_VELOCITY)) {
		ball.y_velocity *= Y_SPEEDING_TICKET;
		std::cout << "Y DOWWNNNNNNNNNNNNNN" << std::endl;
		
		just_reached_max_speed = true;
		if (abs(ball.x_velocity) > BALL_X_VELOCITY) {
			ball.x_velocity *= X_FOLLOWED_UP;
			std::cout << "X DOWWNNNNNNNNNNNNNN" << std::endl;
		}
		left_paddle.speed *= PADDLE_SPEED_DOWN;
		paddleUp(-PADDLE_EXTRA);
	}
	else {
		if (just_reached_max_speed) {
			paddleDown();
			std::cout << "ENDDDDDDDDDDDDDDDDDDDDDD MAXXXXXXXXXXXXXXXx" << std::endl;
			ball.radius -= BALL_DESIZE;
		}
		just_reached_max_speed = false;	
	}
	if (ball.radius <= BALL_MIN_RADIUS)
		ball.radius = BALL_RADIUS;
}

void Pong::bouncing()
{
	if (CheckCollisionCircleRec(
			Vector2{ ball.X, ball.Y }, ball.radius,
			getRec(right_paddle)))
	{
		ball.X = right_paddle.X - ball.radius - .1f;
		ball.x_velocity *= -RIGHT_BOUNCE;
		float dif = ball.Y - right_paddle.centerY();
		if (dif > 0) {
			ball.y_velocity = abs(ball.y_velocity) + dif / 50.f + 5;
		}
		else {
			ball.y_velocity = -abs(ball.y_velocity) + dif / 50.f - 5;
		}
		ball.y_velocity *= RIGHT_BOUNCE;
		player_hit++;
		player_score += HIT_SCORE * gameSpeed();
	}
	if (CheckCollisionCircleRec(
		Vector2{ ball.X, ball.Y }, ball.radius,
		getRec(left_paddle)))
	{
		ball.X = left_paddle.X + left_paddle.width + ball.radius + .1f;
		ball.x_velocity *= -LEFT_BOUNCE;
		float dif = ball.Y - left_paddle.centerY();
		if (dif > 0) {
			ball.y_velocity = abs(ball.y_velocity) + dif / 50.f + 5;
		}
		else {
			ball.y_velocity = -abs(ball.y_velocity) + dif / 50.f - 5;
		}
		ball.y_velocity *= LEFT_BOUNCE;
		computer_hit++;
		computer_score += HIT_SCORE * gameSpeed() / 2;
	}
}

void Pong::gameAI()
{
	if (ball.X <= WINDOW_HEIGHT / 2.f) {
		if (left_paddle.centerY() > ball.Y + 45) {
			// move up
			left_paddle.Y -= int(PADDLE_SPEED * GetFrameTime());
		}
		if (left_paddle.centerY() <= ball.Y - 45) {
			left_paddle.Y += int(PADDLE_SPEED * GetFrameTime());
		}
	}
	else {
		if (left_paddle.centerY() > WINDOW_HEIGHT / 2) {
			left_paddle.Y -= int(PADDLE_SPEED * GetFrameTime());
		}
		if (left_paddle.centerY() < WINDOW_HEIGHT / 2) {
			left_paddle.Y += int(PADDLE_SPEED * GetFrameTime());
		}
	}
}

int Pong::gameSpeed()
{
	return int(std::max(
		abs(ball.x_velocity) / BALL_X_VELOCITY, abs(ball.y_velocity) / BALL_Y_VELOCITY));
}

void Pong::checkWinning()
{
	if (player_score <= 0) {
		winning = true;
		player_score = 0;
		winner = "Computer";
	}
	else if (computer_score <= 0 || player_score >= TARGET_SCORE) {
		winning = true;
		winner = "Player";
	}
}

void Pong::restart()
{
	winning = false;
	player_hit = computer_hit = 0;
	player_score = computer_score = 300;
	just_reached_max_speed = false;
	TARGET_SCORE = 301;
	is_choosing_difficulty = false;
	init();
}


//void Pong::speedingTicket()
//{
//	if (abs(ball.y_velocity) > MAX_Y_VELOCITY) {
//		if (ball.y_velocity < 0)
//			ball.y_velocity = -BALL_Y_VELOCITY;
//		else
//			ball.y_velocity = BALL_Y_VELOCITY;
//		/*if (abs(ball.x_velocity) > MAX_X_VELOCITY)
//			ball.x_velocity /= 2.f;*/
//	}
//	if (abs(ball.x_velocity) > MAX_X_VELOCITY) {
//		if (ball.x_velocity < 0)
//			ball.x_velocity = -BALL_X_VELOCITY;
//		else
//			ball.x_velocity = BALL_X_VELOCITY;
//		/*if (abs(ball.y_velocity) > MAX_Y_VELOCITY)
//			ball.y_velocity /= 2.f;*/
//	}
//	if (abs(ball.y_velocity) == BALL_Y_VELOCITY
//		&& abs(ball.x_velocity) > BALL_X_VELOCITY) {
//		ball.x_velocity *= 0.983f;
//	}
//	if (abs(ball.x_velocity) == BALL_X_VELOCITY
//		&& abs(ball.y_velocity) > BALL_Y_VELOCITY) {
//		ball.y_velocity *= 0.983f;
//	}
//}
//
