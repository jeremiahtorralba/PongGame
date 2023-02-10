#include "GameEngine.h"

int main(int argc, char * argv[]) {
	//GameEngine* game = new GameEngine();
	if(!GameEngine::Instance()->InitGameEngine()) return -1;
	GameEngine::Instance()->InitGameWorld();
	while (GameEngine::Instance()->IsRunning()) { // Setup game loop
		GameEngine::Instance()->Input();
		GameEngine::Instance()->Update();
		GameEngine::Instance()->Render();
	}
	GameEngine::Instance()->Quit(); //system("pause"); // not needed because of Quit()
	return 0;
}