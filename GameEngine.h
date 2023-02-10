#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <SDL.h>
#include <SDL_ttf.h> // to render ttf fonts
#include "Sprite.h"
using namespace std;
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class GameEngine
{
private:
	SDL_Event event; // to handle events
	int mouseX, mouseY; // for mouse coordinates
	SDL_Window* window; // moved from InitGameEngine
	SDL_Renderer* renderer;
	bool isRunning = true;
	TTF_Font* font; // font declarations
	SDL_Surface* fontSurface;
	SDL_Texture* fontTexture;
	SDL_Rect textRectScore; // defines 'score' text on screen
	void RenderFont(const char* text, int x, int y, bool isRefreshText); // font related function
	int p1score = 0; // for demo score is hardcoded, this score will be tracked and made in a different function
	int aiscore = 0; // AI paddle score
	string s1 = to_string(p1score);
	string s2 = to_string(aiscore);
	string aiwins = ".";
	string p1wins = ".";
	Sprite* background; // Sprite Actors
	Sprite* paddleHuman;
	Sprite* paddleAI;
	Sprite* ball;
	void InitializeSpriteBackground(const char* loadPath, int cellX, int cellY, int cellWidth, int cellHeight,
	int destX, int destY, int destW, int destH);
	void InitializeSpritepaddleHuman(const char* loadPath, int cellX, int cellY, int cellWidth, int cellHeight,
	int destX, int destY, int destW, int destH);
	void InitializeSpritepaddleAI(const char* loadPath, int cellX, int cellY, int cellWidth, int cellHeight,
	int destX, int destY, int destW, int destH);
	void InitializeSpriteBall(const char* loadPath, int cellX, int cellY, int cellWidth, int cellHeight,
	int destX, int destY, int destW, int destH);
	SDL_Rect divider; // Visual Improvements: center divider
	SDL_Rect p1goal;
	SDL_Rect aigoal;
	SDL_Rect hide1;
	SDL_Rect hide2;

public:
	int speed_x, speed_y; // x and y speeds of the ball
	int direction[2] = { -1, 1 }; // x and y array directions
	static GameEngine* Instance(); // function returns
	bool InitGameEngine();
	bool CheckCollision(SDL_Rect A, SDL_Rect B);
	void PaddleHumanMoveMouse(); // Sprite, to:
	void UpdateBallPosition();
	void ReverseBallPositionY();
	void ResetBallPositionX();
	void PaddleAIMove();
	void ResetPaddleAIBallNotAIArea();
	void BallInAIArea();
	void CheckBallPaddleCollision();
	void BallInPaddleHumanGoalArea();
	void BallInPaddleAIGoalArea(); // Sprite, from
	bool NotAIArea(SDL_Rect BALL, SDL_Rect AIAREA);
	bool InAIArea(SDL_Rect BALL, SDL_Rect AIAREA);
	void PlayerServe();
	void AIServe();
	void AI();
	void AddToPlayerScore();
	void AddToAIScore();
	void KeepPlayerScore();
	void KeepAIScore();
	void PlayerWins();
	void AIWins();
	void InitGameWorld();
	void Render();
	void Input();
	void Quit();
	void Update();
	bool IsRunning() { return isRunning; }
};