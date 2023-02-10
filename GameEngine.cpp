#include "GameEngine.h"

static GameEngine* instance; // Singleton pattern for GameEngine class, static instance

GameEngine* GameEngine::Instance() {
	if (instance == nullptr)
		instance = new GameEngine();
	return instance;
}

bool GameEngine::InitGameEngine() {
	window = SDL_CreateWindow("Pong Game", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0
	);

	if (!window) {
		cout << "Window creation failed... \n";
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		cout << "Render initialization failed. \n";
		return false;
	}

	if (TTF_Init() != 0) { // check if TTF can be used and if it can't, then show a message
		cout << "TTF Font engine init failed.\n";
		return false;
	}

	return true;
}

bool GameEngine::CheckCollision(SDL_Rect A, SDL_Rect B) {
	int leftA, leftB; // sides of rectangles
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = A.x; // calculate sides of A
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	leftB = B.x; // calculate sides of B
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;

	if (bottomA <= topB) { // if any of the sides from A are outside of B
		return false;
	}
	if (topA >= bottomB) {
		return false;
	}
	if (rightA <= leftB) {
		return false;
	}
	if (leftA >= rightB) {
		return false;
	}

	return true; // if none of the sides from A are outside B
}

void GameEngine::PaddleHumanMoveMouse() { // Map paddleHuman's y position to the mouse's Y pos
	paddleHuman->spriteDestRect.y = mouseY - 72.5;
}

void GameEngine::UpdateBallPosition() { // Change destRect's x and y values to the x and y variables from the function's argument.
	ball->spriteDestRect.x += speed_x;
	ball->spriteDestRect.y += speed_y;
}

void GameEngine::ReverseBallPositionY() { // check ball's y position is above the top wall (y less than zero)
	if (ball->spriteDestRect.y < 0 || ball->spriteDestRect.y >(WINDOW_HEIGHT - ball->spriteDestRect.h)) { // or if y position is lower than the bottom wall (y greater than WINDOW_HEIGHT)
		speed_y = -speed_y;
	}
}

void GameEngine::ResetBallPositionX() { // If ball goes out on sides, left and right, reset to centre of screen
	if (ball->spriteDestRect.x < 0 || ball->spriteDestRect.x > WINDOW_WIDTH) {
		ball->spriteDestRect.x = WINDOW_WIDTH / 2;
		ball->spriteDestRect.y = WINDOW_HEIGHT / 2;
		speed_x = (rand() % 2 + 1) * direction[rand() % 2] * 2.0; // produces random numbers -1, -2, 1 and 2
		speed_y = (rand() % 2 + 1) * direction[rand() % 2] * 2.0;
	}
}

void GameEngine::PaddleAIMove() { // AI paddle moves with the ball (in the y-direction)
	paddleAI->spriteDestRect.y = ball->spriteDestRect.y - paddleAI->spriteDestRect.h / 2 + ball->spriteDestRect.h / 2;
}

void GameEngine::ResetPaddleAIBallNotAIArea() { // Reset paddleAI and stop it when ball is away from right side of divider
	if (NotAIArea(ball->spriteDestRect, divider)) { 
		paddleAI->spriteDestRect.y = WINDOW_HEIGHT / 2 - paddleAI->spriteDestRect.h / 2 - 10;
	}
}

void GameEngine::BallInAIArea() { // If ball is on the right side of the divider, paddleAI moves with improved AI
	if (InAIArea(ball->spriteDestRect, divider)) {
		AI(); // Improve AI Paddle: making it challenging for player to win
	}
}

void GameEngine::CheckBallPaddleCollision() { // continually check if ball and player paddle or the ball and AI paddle collide
	if (CheckCollision(ball->spriteDestRect, paddleAI->spriteDestRect) || CheckCollision(ball->spriteDestRect, paddleHuman->spriteDestRect)) {
		speed_x = -speed_x; // if collision is detected in either, speed is reversed in the x-direction only	
	}
}

void GameEngine::BallInPaddleHumanGoalArea() { // Add score for AI if ball collides with paddleHuman's goal area behind paddleHuman
	if (CheckCollision(ball->spriteDestRect, p1goal)) { 
		AddToAIScore();
		PlayerServe(); // ball is served to aiPaddle
	}
}

void GameEngine::BallInPaddleAIGoalArea() { // Add score for Player if ball collides with paddleAI's goal area behind paddleAI
	if (CheckCollision(ball->spriteDestRect, aigoal)) { 
		AddToPlayerScore();
		AIServe(); // ball is served to playerPaddle
	}
}

bool GameEngine::NotAIArea(SDL_Rect BALL, SDL_Rect AIAREA) {
	int leftBALL, leftAIAREA; // sides of rectangles
	int rightBALL, rightAIAREA;
	int topBALL, topAIAREA;
	int bottomBALL, bottomAIAREA;

	leftBALL = BALL.x; // calculate sides of Ball
	rightBALL = BALL.x + BALL.w;
	topBALL = BALL.y;
	bottomBALL = BALL.y + BALL.h;

	leftAIAREA = AIAREA.x; // calculate sides of AIArea
	rightAIAREA = AIAREA.x + AIAREA.w;
	topAIAREA = AIAREA.y;
	bottomAIAREA = AIAREA.y + AIAREA.h;

	if (bottomBALL <= topAIAREA) { // if any of the sides from Ball are outside of AIArea
		return false;
	}
	if (topBALL >= bottomAIAREA) {
		return false;
	}
	if (leftBALL >= rightAIAREA) {
		return false;
	}

	return true; // if none of the sides from Ball are outside AIArea
}

bool GameEngine::InAIArea(SDL_Rect BALL, SDL_Rect AIAREA) {
	int leftBALL, leftAIAREA; // sides of rectangles
	int rightBALL, rightAIAREA;
	int topBALL, topAIAREA;
	int bottomBALL, bottomAIAREA;

	leftBALL = BALL.x; // calculate sides of Ball
	rightBALL = BALL.x + BALL.w;
	topBALL = BALL.y;
	bottomBALL = BALL.y + BALL.h;

	leftAIAREA = AIAREA.x; // calculate sides of AIArea
	rightAIAREA = AIAREA.x + AIAREA.w;
	topAIAREA = AIAREA.y;
	bottomAIAREA = AIAREA.y + AIAREA.h;

	if (bottomBALL <= topAIAREA) { // if any of the sides from Ball are outside of AIArea
		return false;
	}
	if (topBALL >= bottomAIAREA) {
		return false;
	}
	if (rightBALL <= leftAIAREA) {
		return false;
	}

	return true; // if none of the sides from Ball are outside AIArea
}

void GameEngine::PlayerServe() {
	ball->spriteDestRect.x = WINDOW_WIDTH / 2;
	ball->spriteDestRect.y = WINDOW_HEIGHT / 2;
	speed_x = (rand() % 2 + 1) * direction[rand() % 1] * 2.0; // serve to playerPaddle
	speed_y = (rand() % 2 + 1) * direction[rand() % 2] * 2.0; // produces random numbers -1, -2, 1 and 2
}

void GameEngine::AIServe() {
	ball->spriteDestRect.x = WINDOW_WIDTH / 2;
	ball->spriteDestRect.y = WINDOW_HEIGHT / 2;
	speed_x = (rand() % 2 + 1) * (1) * 2.0; // serve to aiPaddle
	speed_y = (rand() % 2 + 1) * direction[rand() % 2] * 2.0; // produces random numbers -1, -2, 1 and 2
}

void GameEngine::AI() { // Improve AI Paddle: making it challenging for player to win
	if (ball->spriteDestRect.y < WINDOW_HEIGHT / 2 + 2) { // top: aiPaddle moves upward in direction + 2
		paddleAI->spriteDestRect.y = ball->spriteDestRect.y / 0.9209; // aiPaddle's upward movement is divided/slowed by 0.921, 0.9208 
	}
	if (ball->spriteDestRect.y < WINDOW_HEIGHT / 2 + 1) { // middle-top: aiPaddle moves upward in direction + 1
		paddleAI->spriteDestRect.y = ball->spriteDestRect.y / 0.8580; // aiPaddle's upward movement is divided/slowed by 0.780, 0.8070 
	}
	if (ball->spriteDestRect.y >= WINDOW_HEIGHT / 2 - 1) { // middle-bottom: aiPaddle moves downward in direction - 1
		paddleAI->spriteDestRect.y = ball->spriteDestRect.y * 0.8105; // aiPaddle's downward movement is multiplied/slowed by 0.660, 0.66729 
	}
	if (ball->spriteDestRect.y >= WINDOW_HEIGHT / 2 - 2) { // bottom: aiPaddle moves downward in direction - 2
		paddleAI->spriteDestRect.y = ball->spriteDestRect.y * 0.7610; // aiPaddle's downward movement is multiplied/slowed by 0.660, 0.8208
	}
}

void GameEngine::AddToPlayerScore() { // Scoring System: Player's Score
	p1score++;
	s1 = to_string(p1score);
	RenderFont(s1.c_str(), 190, 70, true);
}

void GameEngine::AddToAIScore() { // Scoring System: AI's Score
	aiscore++;
	s2 = to_string(aiscore);
	RenderFont(s2.c_str(), 595, 30, true);
}

void GameEngine::KeepPlayerScore() { // Game-End Condition: if AI scores 5 points
	if (aiscore == 5) { 
		AIWins();
	}
}
 
void GameEngine::KeepAIScore() { // Game-End Condition: if Player scores 5 points
	if (p1score == 5) { 
		PlayerWins();
	}
}

void GameEngine::PlayerWins() { // Game-End Condition: Player wins
	ball->spriteDestRect.x = -20;
	ball->spriteDestRect.y = -20;
	paddleAI->spriteDestRect.y = WINDOW_HEIGHT / 2 - paddleAI->spriteDestRect.h / 2 - 10;
	paddleHuman->spriteDestRect.y = WINDOW_HEIGHT / 2 - paddleHuman->spriteDestRect.h / 2 - 10;
	p1wins = "PLAYER WINS! " + s1 + " - " + s2;
	RenderFont(p1wins.c_str(), 35, 70, true);
}

void GameEngine::AIWins() { // Game-End Condition: AI wins
	ball->spriteDestRect.x = -20;
	ball->spriteDestRect.y = -20;
	paddleAI->spriteDestRect.y = WINDOW_HEIGHT / 2 - paddleAI->spriteDestRect.h / 2 - 10;
	paddleHuman->spriteDestRect.y = WINDOW_HEIGHT / 2 - paddleHuman->spriteDestRect.h / 2 - 10;
	aiwins = "A.I. WINS! " + s2 + " - " + s1;
	RenderFont(aiwins.c_str(), 460, 60, true);
}

void GameEngine::InitGameWorld() {
	font = TTF_OpenFont("Assets/Fonts/LTYPE.TTF", 30); // initial size of 30, LTYPE.TTF file placed in created Assets/Fonts folders
	speed_x = -3.5; // speed variables
	speed_y = -3.5;
	int destW = 800; // Sprites:
	int destH = 600;
	int destW2 = 128;
	int destH2 = 128;
	int destW3 = 128;
	int destH3 = 128;
	int destW4 = 32;
	int destH4 = 32;
	float destX = WINDOW_WIDTH * 0.5f - destW * 0.5f;
	float destY = WINDOW_HEIGHT * 0.5f - destH * 0.5f;
	float destX2 = WINDOW_WIDTH * 0.1f - destW2 * 0.1f - 152.0;
	float destY2 = WINDOW_HEIGHT * 0.4816f - destH2 * 0.4816f;
	float destX3 = WINDOW_WIDTH * 1.0f - destW3 * 1.0 + 84.0f;
	float destY3 = WINDOW_HEIGHT * 0.4816f - destH3 * 0.4816f;
	float destX4 = WINDOW_WIDTH * 0.5030f - destW4 * 0.5030f;
	float destY4 = WINDOW_HEIGHT * 0.4925f - destH4 * 0.4925f;
	InitializeSpriteBackground("Assets/Sprites/spritestrip-800.png", 0, 0, 800, 600, destX, destY, destW, destH);
	InitializeSpritepaddleHuman("Assets/Sprites/spritestrip-768.png", 0, 0, 256, 256, destX2, destY2, destW2, destH2);
	InitializeSpritepaddleAI("Assets/Sprites/spritestrip-768.png", 256, 0, 256, 256, destX3, destY3, destW3, destH3);
	InitializeSpriteBall("Assets/Sprites/spritestrip-768.png", 512, 0, 256, 256, destX4, destY4, destW4, destH4);
	divider.x = WINDOW_WIDTH / 2; // Center divider
	divider.y += 0;
	divider.w = 1;
	divider.h = WINDOW_HEIGHT;
	p1goal.x = 0;
	p1goal.y += 0;
	p1goal.w = 1;
	p1goal.h = WINDOW_HEIGHT;
	aigoal.x = WINDOW_WIDTH - 1;
	aigoal.y += 0;
	aigoal.w = 1;
	aigoal.h = WINDOW_HEIGHT;
	hide1.x = 42;
	hide1.y = 94;
	hide1.w = 5;
	hide1.h = 5;
	hide2.x = 456;
	hide2.y = 94;
	hide2.w = 5;
	hide2.h = 5;
}

void GameEngine::Render() {
	SDL_RenderClear(renderer); // Clears the previously drawn frame, Draw current frame:
	background->Render(renderer); // background Sprite
	paddleHuman->Render(renderer); // paddleHmuan Sprite
	paddleAI->Render(renderer); // paddleAI Sprite
	ball->Render(renderer); // ball Sprite
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Visual Improvements: center divider
	SDL_RenderFillRect(renderer, &divider);
	RenderFont(s1.c_str(), 190, 30, true); // render player's score  
	RenderFont(s2.c_str(), 595, 30, true); // render ai's score 
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // player's goal area
	SDL_RenderFillRect(renderer, &p1goal);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // ai's goal area
	SDL_RenderFillRect(renderer, &aigoal);
	RenderFont(p1wins.c_str(), 35, 70, true); // player's win statement
	RenderFont(aiwins.c_str(), 450, 70, true); // ai's win statement
	SDL_SetRenderDrawColor(renderer, 63, 72, 204, 255); // indigo
	SDL_RenderFillRect(renderer, &hide1); // hide1
	SDL_SetRenderDrawColor(renderer, 63, 72, 204, 255); // indigo
	SDL_RenderFillRect(renderer, &hide2); // hide2
	SDL_RenderPresent(renderer); // must call this to render all of the above
}

void GameEngine::Input() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_MOUSEMOTION) {
			SDL_GetMouseState(&mouseX, &mouseY); // get and store x, y mouse states
		}
		if (event.type == SDL_QUIT) { // if we click "X" button to close the window then SDL_Quit event type is triggered
			isRunning = false;
		}
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				isRunning = false;
				break;
			}
		}
	}
}

void GameEngine::Quit() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit(); // shutdown SDL, any clearing of properties should be placed here
	TTF_CloseFont(font);
	TTF_Quit();
}

void GameEngine::Update() {
	PaddleHumanMoveMouse(); // Map paddleHuman's y position to the mouse's Y pos
	SDL_Delay(1); // Pauses the game loop for 10 ms before continuing on to the next frame 
	UpdateBallPosition(); // Continually update ball's x and y position by the speed amount
	ResetBallPositionX(); // If ball goes out on sides, left and right, reset to centre of screen
	ReverseBallPositionY(); // Reverse the ball's y direction
	PaddleAIMove(); // Move paddleAI with the ball (in the y-direction)
	ResetPaddleAIBallNotAIArea(); // Reset paddleAI and stop it when ball is away from right side of divider
	BallInAIArea(); // If ball is on the right side of the divider, paddleAI moves with improved AI
	CheckBallPaddleCollision(); // Continually check if ball and player paddle or the ball and AI paddle collide
	BallInPaddleHumanGoalArea(); // Add score for AI if ball collides with paddleHuman's goal area behind paddleHuman
	BallInPaddleAIGoalArea(); // Add score for Player if ball collides with paddleAI's goal area behind paddleAI
	KeepPlayerScore(); // Game-End Condition: if AI scores 5 points
	KeepAIScore(); // Game-End Condition: if Player scores 5 points
}

void GameEngine::RenderFont(const char* text, int x, int y, bool isRefreshText) {
if (isRefreshText) {
	SDL_Color textColor = { 255, 255, 255, 255 }; // white
	fontSurface = TTF_RenderText_Solid(font, text, textColor);

	if (fontTexture) // de-allocate previously assigned font texture, if any
		SDL_DestroyTexture(fontTexture);

	fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	textRectScore = { x, y, fontSurface->w, fontSurface->h };
}
SDL_RenderCopy(renderer, fontTexture, 0, &textRectScore); // draws the font
}

void GameEngine::InitializeSpriteBackground(const char* loadPath, int cellX, int cellY, int cellWidth, int cellHeight,
	int destX, int destY, int destW, int destH) {
	SDL_Rect src;
	src.x = cellX;
	src.y = cellY;
	src.w = cellWidth;
	src.h = cellHeight;
	SDL_Rect dest;
	dest.x = destX;
	dest.y = destY;
	dest.w = destW;
	dest.h = destH;
	background = new Sprite(loadPath, src, dest, renderer);
}

void GameEngine::InitializeSpritepaddleHuman(const char* loadPath2, int cellX2, int cellY2, int cellWidth2, int cellHeight2,
	int destX2, int destY2, int destW2, int destH2) {
	SDL_Rect src2;
	src2.x = cellX2;
	src2.y = cellY2;
	src2.w = cellWidth2;
	src2.h = cellHeight2;
	SDL_Rect dest2;
	dest2.x = destX2;
	dest2.y = destY2;
	dest2.w = destW2;
	dest2.h = destH2;
	paddleHuman = new Sprite(loadPath2, src2, dest2, renderer);
}

void GameEngine::InitializeSpritepaddleAI(const char* loadPath3, int cellX3, int cellY3, int cellWidth3, int cellHeight3,
	int destX3, int destY3, int destW3, int destH3) {
	SDL_Rect src3;
	src3.x = cellX3;
	src3.y = cellY3;
	src3.w = cellWidth3;
	src3.h = cellHeight3;
	SDL_Rect dest3;
	dest3.x = destX3;
	dest3.y = destY3;
	dest3.w = destW3;
	dest3.h = destH3;
	paddleAI = new Sprite(loadPath3, src3, dest3, renderer);
}

void GameEngine::InitializeSpriteBall(const char* loadPath4, int cellX4, int cellY4, int cellWidth4, int cellHeight4,
	int destX4, int destY4, int destW4, int destH4) {
	SDL_Rect src4;
	src4.x = cellX4;
	src4.y = cellY4;
	src4.w = cellWidth4;
	src4.h = cellHeight4;
	SDL_Rect dest4;
	dest4.x = destX4;
	dest4.y = destY4;
	dest4.w = destW4;
	dest4.h = destH4;
	ball = new Sprite(loadPath4, src4, dest4, renderer);
}