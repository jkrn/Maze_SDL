#include "GameFunctions.hpp"

using namespace std;
using namespace gameData;

int main(int argc, char *argv[]) {
	// Init SDL
	if (!initSDL()) { return EXIT_FAILURE; }
	// Load game resources
	loadGameResources();
	// Init Game
	initGame(FIRST_START);
	// --- Main loop ---
	while (true) {
		// Process Events
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_WINDOWEVENT) {
				// Quit
				if (event.window.event == SDL_WINDOWEVENT_CLOSE) { isRunning = false; }
				// Lost Focus
				else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST && gameStatus == RUNNING) {
					gameStatus = PAUSED;
				}
			}
		}
		// Check isRunning
		if (!isRunning) { break; }
		// Render Clear
		SDL_RenderClear(renderer);
		// --- BEFORE START ---
		if (gameStatus == BEFORE_START) {
			showBeforeStartScreen();
		}
		// --- MANUAL / CREDITS ---
		else if (gameStatus == MANUAL || gameStatus == CREDITS) {
			showInfoScreen();
		}
		// --- PAUSED ---
		else if (gameStatus == PAUSED) {
			showPauseScreen();
		}
		// --- GAME OVER ---
		else if (gameStatus == GAME_OVER) {
			showGameOverScreen();
		}
		// --- FINISHED ---
		else if (gameStatus == FINISHED) {
			showFinishedScreen();
		}
		// --- RUNNING ---
		else if (gameStatus == RUNNING) {
			if (showRunningScreen()) { continue; }
		}
		// Check isRunning
		if (!isRunning) { break; }
		// Render Present
		SDL_RenderPresent(renderer);
	}
	// Close Game
	closeGame();
	// Free game resources
	freeGameResources();
	// Close SDL
	closeSDL();
	return EXIT_SUCCESS;
}