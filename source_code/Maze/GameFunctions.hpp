#pragma once

#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Constants.hpp"
#include "Direct.hpp"
#include "Maze.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "StaticText.hpp"
#include "RandomGenerator.hpp"
#include "resource.h"

enum GameStatus { BEFORE_START, MANUAL, CREDITS, PAUSED, GAME_OVER, FINISHED, RUNNING };
enum InitMode { FIRST_START, RESTART_COURSE, CHANGE_COURSE };
enum RunningMode { START_FADE_IN, NEXT_MAP, KEYBOARD_OPEN };
enum RenderObject { BLOCK, ATTACK_AREA, LIGHT_SOURCE };

struct GameSave {
	int highScore[numCourses] = {};
};

namespace gameData{
	// --- Window ---
	extern SDL_Renderer* renderer;
	// --- Game Variables ---
	extern GameStatus gameStatus;
	// --- Event ---
	extern SDL_Event event;
	// --- is Running ---
	extern bool isRunning;
}

// === SDL ===
bool initSDL();
void closeSDL();
// === Random Generator ===
void initRandomGenerator();
// === Resources ===
void LoadFileInResource(int name, int type, DWORD& size, byte*& data);
SDL_Surface* loadGameSurface(int id);
SDL_Surface* loadCroppedGameSurface(int id);
SDL_Texture* loadGameTexture(int id);
void loadGameResources();
void freeGameResources();
// === Generated Textures ===
void createGeneratedTextures();
void closeGeneratedTextures();
// === Status Bar ===
void createLevelIcon(SDL_Surface* levelIconSurface);
void createStatusBar();
// === Texts ===
void createTexts();
void freeTexts();
// === Drawing ===
SDL_Surface* drawLightSurface(int radius, int areaWidth, int areaHeight, int textureWidth, int textureHeight, int offsetX, int offsetY);
SDL_Texture* drawLightTexture(int radius, int areaWidth, int areaHeight, int textureWidth, int textureHeight, int offsetX, int offsetY);
SDL_Texture* drawBlackScreen(int width, int height);
void drawTitleScreenCell(SDL_Surface* titleScreenSurface, int x, int y, int val);
void drawTitleScreen();
void drawFreeBlock(SDL_Surface* surface, int x, int y);
void drawBlock(SDL_Surface* surface, int x, int y, const SDL_Color& color);
void drawDroneAttackArea(SDL_Surface* droneAttackArea);
void drawSwitchAttackArea(SDL_Surface* switchAttackArea, int status);
void drawAttackAreas();
void drawMap(SDL_Surface* surface);
// === Save File ===
void updateSaveFile();
void writeSaveFile();
void readSaveFile();
// === Game Variables ===
void initGame(InitMode mode);
void closeGame();
// === Map Data ===
void createMapData();
// === Status Update ===
void resetPlayerHealth();
void initPlayer();
void setNextMap();
void updateStatus();
// === Course ===
void updateCourse(int c);
// === Player Direction ===
void addPlayerDirection(DIR dir);
void removePlayerDirection(DIR dir);
// === Unit Movement ===
bool isMovementPossible(Unit &unit, int dx, int dy);
bool moveUnit(Unit &unit, int dx, int dy);
void updateUnitPosition(Unit &unit);
// === Enemies ===
void activateBosses();
void activateShooters();
bool isDroneInRange(Drone& drone, int dx, int dy);
void moveDrones();
// === Hit Detection ===
bool isHitByDrone();
bool isHitBySwitch();
bool isHitByPatrol();
bool isHitByProjectile();
// === Game Status Screens ===
void showBeforeStartScreen();
void showInfoScreen();
void showPauseScreen();
void showGameOverScreen();
void showFinishedScreen();
bool showRunningScreen();
// === Game Running ===
void updatePlayerTimeOut();
bool checkForPaused();
bool checkForGameOver();
void fadeIn();
bool nextMapUpdate();
void updateEnemyPattern();
void updatePlayerMovement();
void updatePositions();
// === Status Bar Values ===
std::string getHealthStatus();
std::string getLevelStatus();
std::string getItemStatus();
// === Rendering ===
bool inView(double vx, double vy, RenderObject renderObject);
void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, int w, int h);
void renderGameView();
void renderLights(double cx, double cy);
void renderTitleScreenLights();
void renderFinishedScreenText();