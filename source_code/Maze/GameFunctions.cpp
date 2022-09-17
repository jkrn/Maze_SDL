#include "GameFunctions.hpp"

namespace gameData {
	// --- Font ---
	TTF_Font *statusTextFont = NULL;
	TTF_Font *statusFont = NULL;
	// --- Static Texts ---
	StaticText* gameStatusText[2] = {};
	StaticText* healthText = NULL;
	StaticText* levelText = NULL;
	StaticText* keyItemsText = NULL;
	StaticText* pointsText = NULL;
	StaticText* highScoreText = NULL;
	StaticText* selectionText[numSelectionTexts] = {};
	StaticText* exitSelectionText[2] = {};
	StaticText* infoSelectionText[4] = {};
	StaticText* informationSelectionText[6] = {};
	// --- Surfaces ---
	SDL_Surface* freeBlockSurface = NULL;
	SDL_Surface* keyItemBlockSurface = NULL;
	SDL_Surface* playerBlockSurface = NULL;
	SDL_Surface* faceSurface = NULL;
	SDL_Surface* statusBarSurface = NULL;
	// --- Textures ---
	SDL_Texture* exitBlockTexture = NULL;
	SDL_Texture* keyItemBlockTexture = NULL;
	SDL_Texture* bonusItemBlockTexture[5] = {};
	SDL_Texture* projBlockTexture = NULL;
	SDL_Texture* patrolBlockTexture = NULL;
	SDL_Texture* shooterBlockTexture = NULL;
	SDL_Texture* droneBlockTexture = NULL;
	SDL_Texture* switchBlockTexture = NULL;
	SDL_Texture* playerBlockTexture = NULL;
	SDL_Texture* bossBlockTexture = NULL;
	SDL_Texture* statusBarTexture = NULL;
	SDL_Texture* highScoreTexture = NULL;
	SDL_Texture* titleScreenTexture = NULL;
	SDL_Texture* blackScreenTexture = NULL;
	SDL_Texture* droneAttackAreaTexture = NULL;
	SDL_Texture* switchAttackAreaTexture[3] = {};
	SDL_Texture* lightTexture[numLightTypes] = {};
	SDL_Texture* mapTexture = NULL;
	// --- Window ---
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	// --- Event ---
	SDL_Event event;
	// --- is Running ---
	bool isRunning = true;
	// --- Game Variables ---
	int level = 1;
	int points = 0;
	int levelMazeWidth = 2;
	int levelMazeHeight = 2;
	int levelKeyItems = 1;
	int levelBonusItems = 0;
	int levelDrones = 0;
	int levelShooters = 0;
	int levelSwitches = 0;
	int levelPatrols = 0;
	int gameStepCount = 0;
	bool newHighScore = false;
	RunningMode runningMode = START_FADE_IN;
	GameStatus gameStatus = BEFORE_START;
	GameSave gameSave = {};
	// --- Keyboard ---
	bool EscPressed = false;
	bool WPressed = false;
	bool APressed = false;
	bool SPressed = false;
	bool DPressed = false;
	// --- Player ---
	Player player;
	std::vector<DIR> playerDirections;
	int playerRotation = 0;
	double playerAlpha = 0.0;
	// --- Map ---
	Map map;
	int mapWidthInPixel = 0;
	int mapHeightInPixel = 0;
	// --- Course ---
	int course = -1;
	double currEnemyRatio = 0;
	int currKeyItemValue = 0;
	SDL_Color currCourseColorDark = {};
	SDL_Color currCourseColorBright = {};
	SDL_Color currTitleScreenLightColor = {};
	// --- Title Screen Lights ---
	int titleScreenLightsPos[titleScreenNumLights][2];
	int titleScreenLightsVec[titleScreenNumLights];
	double titleScreenLightsVal[titleScreenNumLights];
	// --- FPS ---
	int targetFPS = 0;
	int playerTimeoutFramesStart = 0;
	// --- Game Step Status ---
	int gameStepStatusB = 1;
	int gameStepStatusC = 1;
	int gameStepStatusD = 1;
	// --- Boss Pause ---
	int bossPause = 0;
	// --- Blink ---
	int blinkFrames = 0;
	// --- Movement ---
	double alphaStepSize = 0.0;
	double lightChangeStepSize = 0.0;
	int nrMovementSteps = 0;
}

using namespace std;
using namespace gameData;

// === SDL ===

bool initSDL() {
	// SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { return false; }
	// SDL Render Scale Quality
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2")) { return false; }
	// Window
	window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidthInPixel, windowHeightInPixel, SDL_WINDOW_SHOWN);
	if (window == NULL) { return false; }
	// Create renderer
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) { return false; }
	// Render Draw Color
	SDL_SetRenderDrawColor(renderer, emptyColor.r, emptyColor.g, emptyColor.b, emptyColor.a);
	// FPS
	SDL_DisplayMode mode = {};
	SDL_GetDisplayMode(0, 0, &mode);
	targetFPS = mode.refresh_rate;
	playerTimeoutFramesStart = targetFPS;
	// Game Step Status
	gameStepStatusB = targetFPS + 1;
	gameStepStatusC = targetFPS * 2 + 1;
	gameStepStatusD = targetFPS * 3 + 1;
	// Boss Pause
	bossPause = targetFPS / 2;
	// Blink
	blinkFrames = targetFPS / 2;
	// Movement
	nrMovementSteps = (int)(targetFPS / movementSpeed);
	alphaStepSize = alphaSpeed / targetFPS;
	lightChangeStepSize = lightChangeSpeed / targetFPS;
	// Initialize PNG loading
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) { return false; }
	// Initialize SDL_ttf
	if (TTF_Init() == -1) { return false; }
	return true;
}

void closeSDL() {
	// Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// === Random Generator ===

void initRandomGenerator() {
	srand((unsigned int)time(0));
}

// === Resources ===

void LoadFileInResource(int name, int type, DWORD& size, byte*& data) {
	HMODULE handle = GetModuleHandle(NULL);
	HRSRC rc = FindResource(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
	HGLOBAL rcData = LoadResource(handle, rc);
	size = SizeofResource(handle, rc);
	data = static_cast<byte*>(LockResource(rcData));
}

SDL_Surface* loadGameSurface(int id) {
	DWORD resourceSize = 0;
	byte* data = NULL;
	LoadFileInResource(id, TEXTURE, resourceSize, data);
	return IMG_Load_RW(SDL_RWFromConstMem(data, resourceSize), 1);
}

SDL_Surface* loadCroppedGameSurface(int id) {
	SDL_Surface* loadedSurface = loadGameSurface(id);
	SDL_Surface* scaledSurface = SDL_CreateRGBSurfaceWithFormat(0, blockSizeInPixel, blockSizeInPixel, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_Surface* croppedSurface = SDL_CreateRGBSurfaceWithFormat(0, blockSizeInPixel - 2, blockSizeInPixel - 2, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_BlitScaled(loadedSurface, NULL, scaledSurface, NULL);
	SDL_Rect pos = { 1, 1, blockSizeInPixel - 2, blockSizeInPixel - 2 };
	SDL_BlitSurface(scaledSurface, &pos, croppedSurface, NULL);
	SDL_FreeSurface(loadedSurface);
	SDL_FreeSurface(scaledSurface);
	return croppedSurface;
}

SDL_Texture* loadGameTexture(int id) {
	SDL_Surface* loadedSurface = loadGameSurface(id);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	return texture;
}

void loadGameResources() {
	// Random Generator
	initRandomGenerator();
	// Font
	DWORD resourceSize = 0;
	byte* data = NULL;
	LoadFileInResource(ID_GAME_FONT, FONT, resourceSize, data);
	statusTextFont = TTF_OpenFontRW(SDL_RWFromConstMem(data, resourceSize), 0, statusTextFontSize);
	statusFont = TTF_OpenFontRW(SDL_RWFromConstMem(data, resourceSize), 1, statusFontSize);
	// Surfaces
	freeBlockSurface = loadGameSurface(ID_FREE_TEXTURE);
	keyItemBlockSurface = loadCroppedGameSurface(ID_KEYITEM_TEXTURE);
	playerBlockSurface = loadGameSurface(ID_PLAYER_TEXTURE);
	faceSurface = loadGameSurface(ID_FACE_TEXTURE);
	// Textures
	exitBlockTexture = loadGameTexture(ID_EXIT_TEXTURE);
	keyItemBlockTexture = loadGameTexture(ID_KEYITEM_TEXTURE);
	for (int i = 0; i < 5; i++) {
		bonusItemBlockTexture[i] = loadGameTexture(ID_BONUSITEM1_TEXTURE + i);
	}
	projBlockTexture = loadGameTexture(ID_PROJECTILE_TEXTURE);
	patrolBlockTexture = loadGameTexture(ID_PATROL_TEXTURE);
	shooterBlockTexture = loadGameTexture(ID_SHOOTER_TEXTURE);
	droneBlockTexture = loadGameTexture(ID_DRONE_TEXTURE);
	switchBlockTexture = loadGameTexture(ID_SWITCH_TEXTURE);
	playerBlockTexture = loadGameTexture(ID_PLAYER_TEXTURE);
	bossBlockTexture = loadGameTexture(ID_BOSS_TEXTURE);
	// Status Bar
	createStatusBar();
	// HighScore
	highScoreTexture = loadGameTexture(ID_HIGHSCORE_TEXTURE);
	// Create Generated Textures
	createGeneratedTextures();
	// Create Texts
	createTexts();
}

void freeGameResources() {
	// Fonts
	TTF_CloseFont(statusTextFont);
	TTF_CloseFont(statusFont);
	// Surfaces
	SDL_FreeSurface(freeBlockSurface);
	SDL_FreeSurface(keyItemBlockSurface);
	SDL_FreeSurface(playerBlockSurface);
	SDL_FreeSurface(faceSurface);
	SDL_FreeSurface(statusBarSurface);
	// Textures
	SDL_DestroyTexture(exitBlockTexture);
	SDL_DestroyTexture(keyItemBlockTexture);
	for (int i = 0; i < 5; i++) {
		SDL_DestroyTexture(bonusItemBlockTexture[i]);
	}
	SDL_DestroyTexture(projBlockTexture);
	SDL_DestroyTexture(patrolBlockTexture);
	SDL_DestroyTexture(shooterBlockTexture);
	SDL_DestroyTexture(droneBlockTexture);
	SDL_DestroyTexture(switchBlockTexture);
	SDL_DestroyTexture(playerBlockTexture);
	SDL_DestroyTexture(bossBlockTexture);
	SDL_DestroyTexture(statusBarTexture);
	SDL_DestroyTexture(highScoreTexture);
	// Close Generated Textures
	closeGeneratedTextures();
	// Free texts
	freeTexts();
}

// === Generated Textures ===

void createGeneratedTextures() {
	// Title Screen Texture
	drawTitleScreen();
	// Attack Areas
	drawAttackAreas();
	// Black Screen Texture
	blackScreenTexture = drawBlackScreen(windowWidthInPixel, windowHeightInPixel);
	// Light Texture
	for (int i = 0; i < numLightTypes; i++) {
		lightTexture[i] = drawLightTexture((i < 6) ? lightRadiusInPixel : titleScreenLightRadiusInPixel, lightAreaSizes[i][0], lightAreaSizes[i][1], lightTextureSizes[i][0], lightTextureSizes[i][1], lightTextureOffsets[i][0], lightTextureOffsets[i][1]);
		if (i >= 3 && i < 6) { SDL_SetTextureColorMod(lightTexture[i], attackAreaLightColor.r, attackAreaLightColor.g, attackAreaLightColor.b); }
	}
}

void closeGeneratedTextures() {
	// Title Screen Texture
	SDL_DestroyTexture(titleScreenTexture);
	// Drone Block Attack Area Texture
	SDL_DestroyTexture(droneAttackAreaTexture);
	// Switch Attack Area Texture
	SDL_DestroyTexture(switchAttackAreaTexture[0]);
	SDL_DestroyTexture(switchAttackAreaTexture[1]);
	// Black Screen Texture
	SDL_DestroyTexture(blackScreenTexture);
	// Light Texture
	for (int i = 0; i < numLightTypes; i++) {
		SDL_DestroyTexture(lightTexture[i]);
	}
}

// === Status Bar ===

void createLevelIcon(SDL_Surface* levelIconSurface) {
	Maze maze(4, 4);
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			SDL_Rect pos = { x * 8, y * 8, 6, 6 };
			SDL_FillRect(levelIconSurface, &pos, SDL_MapRGBA(levelIconSurface->format, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a));
			if (y < 3 && (maze.maze[x][y] & SOUTH_VAL) == SOUTH_VAL) {
				pos = { x * 8, y * 8 + 6, 6, 2 };
				SDL_FillRect(levelIconSurface, &pos, SDL_MapRGBA(levelIconSurface->format, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a));
			}
			if (x < 3 && (maze.maze[x][y] & EAST_VAL) == EAST_VAL) {
				pos = { x * 8 + 6, y * 8, 2, 6 };
				SDL_FillRect(levelIconSurface, &pos, SDL_MapRGBA(levelIconSurface->format, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a));
			}
		}
	}
}

void createStatusBar() {
	statusBarSurface = SDL_CreateRGBSurfaceWithFormat(0, statusWidthInPixel, statusHeightInPixel, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_FillRect(statusBarSurface, NULL, SDL_MapRGBA(statusBarSurface->format, statusBarColor.r, statusBarColor.g, statusBarColor.b, statusBarColor.a));
	// Health
	SDL_Surface* loadedSurface = loadGameSurface(ID_HEALTH_TEXTURE);
	SDL_Rect pos = { healthTextOffsetInPixel - blockSizeInPixel + 1, 1, blockSizeInPixel - 2, blockSizeInPixel - 2 };
	SDL_BlitSurface(loadedSurface, NULL, statusBarSurface, &pos);
	SDL_FreeSurface(loadedSurface);
	loadedSurface = NULL;
	// Level
	SDL_Surface* levelIconSurface = SDL_CreateRGBSurfaceWithFormat(0, blockSizeInPixel - 2, blockSizeInPixel - 2, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_FillRect(levelIconSurface, NULL, SDL_MapRGBA(levelIconSurface->format, blackColor.r, blackColor.g, blackColor.b, blackColor.a));
	createLevelIcon(levelIconSurface);
	pos.x = levelTextOffsetInPixel - blockSizeInPixel + 1;
	SDL_BlitSurface(levelIconSurface, NULL, statusBarSurface, &pos);
	SDL_FreeSurface(levelIconSurface);
	// Points
	loadedSurface = loadGameSurface(ID_POINTS_TEXTURE);
	pos.x = pointsTextOffsetInPixel - blockSizeInPixel + 1;
	SDL_BlitSurface(loadedSurface, NULL, statusBarSurface, &pos);
	SDL_FreeSurface(loadedSurface);
	loadedSurface = NULL;
	// HighScore
	loadedSurface = loadGameSurface(ID_HIGHSCORE_TEXTURE);
	pos.x = highScoreTextOffsetInPixel - blockSizeInPixel + 1;
	SDL_BlitSurface(loadedSurface, NULL, statusBarSurface, &pos);
	SDL_FreeSurface(loadedSurface);
	// Create Texture
	statusBarTexture = SDL_CreateTextureFromSurface(renderer, statusBarSurface);
}

// === Texts ===

void createTexts() {
	SDL_Rect gameStatusTextPosition = { selectionTextOffsetX*blockSizeInPixel, selectionTitleOffsetY*blockSizeInPixel, 0, 0 };
	gameStatusText[0] = new StaticText(statusTextFont, beforeStartText, statusTextColor, &gameStatusTextPosition, windowWidthInPixel / 2, renderer);
	gameStatusTextPosition.x += selectionOffsetD * blockSizeInPixel;
	gameStatusText[1] = new StaticText(statusTextFont, string("-"), statusTextColor, &gameStatusTextPosition, windowWidthInPixel / 2, renderer);
	SDL_Rect exitSelectionTextPosition = { selectionTextOffsetX*blockSizeInPixel, selectionExitTextOffsetY*blockSizeInPixel, 0, 0 };
	exitSelectionText[0] = new StaticText(statusTextFont, exitText[0], statusTextColor, &exitSelectionTextPosition, windowWidthInPixel / 2, renderer);
	exitSelectionTextPosition.x += selectionOffsetA * blockSizeInPixel;
	exitSelectionText[1] = new StaticText(statusTextFont, exitText[1], statusTextColor, &exitSelectionTextPosition, windowWidthInPixel / 2, renderer);
	for (int i = 0; i < 2; i++) {
		SDL_Rect infoGameSelectionTextPosition = { selectionTextOffsetX*blockSizeInPixel, (selectionInfoTextOffsetY + i)*blockSizeInPixel, 0, 0 };
		infoSelectionText[i * 2] = new StaticText(statusTextFont, infoText[i * 2], statusTextColor, &infoGameSelectionTextPosition, windowWidthInPixel / 2, renderer);
		infoGameSelectionTextPosition.x += selectionOffsetA * blockSizeInPixel;
		infoSelectionText[i * 2 + 1] = new StaticText(statusTextFont, infoText[i * 2 + 1], statusTextColor, &infoGameSelectionTextPosition, windowWidthInPixel / 2, renderer);
		SDL_Rect informationGameSelectionTextPosition = { selectionInformationTextOffsetX * blockSizeInPixel, selectionInformationTextOffsetY * blockSizeInPixel, 0, 0 };
		informationSelectionText[i * 3] = new StaticText(statusTextFont, informationText[i * 3], greenColor, &informationGameSelectionTextPosition, (int)(windowWidthInPixel * 0.75f), renderer);
		informationSelectionText[i * 3 + 1] = new StaticText(statusTextFont, informationText[i * 3 + 1], statusTextColor, &informationGameSelectionTextPosition, (int)(windowWidthInPixel * 0.75f), renderer);
		informationGameSelectionTextPosition.x += selectionOffsetE * blockSizeInPixel;
		informationSelectionText[i * 3 + 2] = new StaticText(statusTextFont, informationText[i * 3 + 2], statusTextColor, &informationGameSelectionTextPosition, (int)(windowWidthInPixel * 0.75f), renderer);
	}
	SDL_Rect healthTextPosition = { healthTextOffsetInPixel + statusValueOffsetX, statusValueOffsetY, 0, 0 };
	healthText = new StaticText(statusFont, string("-"), statusColor, &healthTextPosition, blockSizeInPixel * statusTextWidth, renderer);
	SDL_Rect levelTextPosition = { levelTextOffsetInPixel + statusValueOffsetX, statusValueOffsetY, 0, 0 };
	levelText = new StaticText(statusFont, string("-"), statusColor, &levelTextPosition, blockSizeInPixel * statusTextWidth, renderer);
	SDL_Rect keyItemsTextPosition = { keyItemsTextOffsetInPixel + statusValueOffsetX, statusValueOffsetY, 0, 0 };
	keyItemsText = new StaticText(statusFont, string("-"), statusColor, &keyItemsTextPosition, blockSizeInPixel * statusTextWidth, renderer);
	SDL_Rect pointsTextPosition = { pointsTextOffsetInPixel + statusValueOffsetX, statusValueOffsetY, 0, 0 };
	pointsText = new StaticText(statusFont, string("-"), statusColor, &pointsTextPosition, blockSizeInPixel * statusTextWidth, renderer);
	SDL_Rect highScoreTextPosition = { highScoreTextOffsetInPixel + statusValueOffsetX, statusValueOffsetY, 0, 0 };
	highScoreText = new StaticText(statusFont, string("-"), statusColor, &highScoreTextPosition, blockSizeInPixel * statusTextWidth, renderer);
	for (int i = 0; i < numCourses; i++) {
		SDL_Rect selectionTextPosition = { selectionTextOffsetX*blockSizeInPixel,(selectionTextOffsetY + i)*blockSizeInPixel,0,0 };
		selectionText[i*numTextsPerCourse] = new StaticText(statusTextFont, string("[") + to_string(i + 1) + string("]"), courseColorsBright[i], &selectionTextPosition, blockSizeInPixel * selectionItemTextWidth, renderer);
		selectionTextPosition.x = (selectionTextOffsetX + selectionOffsetA)*blockSizeInPixel;
		selectionText[i*numTextsPerCourse + 1] = new StaticText(statusTextFont, selectionItemTexts[i], courseColorsBright[i], &selectionTextPosition, blockSizeInPixel * selectionItemTextWidth, renderer);
		selectionTextPosition.x = (selectionTextOffsetX + selectionOffsetB)*blockSizeInPixel;
		selectionText[i*numTextsPerCourse + 2] = new StaticText(statusTextFont, string("-"), courseColorsBright[i], &selectionTextPosition, blockSizeInPixel * selectionItemTextWidth, renderer);
		selectionTextPosition.x = (selectionTextOffsetX + selectionOffsetC)*blockSizeInPixel;
		selectionText[i*numTextsPerCourse + 3] = new StaticText(statusTextFont, string("/") + to_string(maxPoints), courseColorsBright[i], &selectionTextPosition, blockSizeInPixel * selectionItemTextWidth, renderer);
	}
}

void freeTexts() {
	for (int i = 0; i < 2; i++) {
		delete gameStatusText[i];
		delete exitSelectionText[i];
	}
	for (int i = 0; i < 4; i++) {
		delete infoSelectionText[i];
	}
	for (int i = 0; i < 6; i++) {
		delete informationSelectionText[i];
	}
	delete healthText;
	delete levelText;
	delete keyItemsText;
	delete pointsText;
	delete highScoreText;
	for (int i = 0; i < numSelectionTexts; i++) {
		delete selectionText[i];
	}
}

// === Drawing ===

SDL_Surface* drawLightSurface(int radius, int areaWidth, int areaHeight, int textureWidth, int textureHeight, int offsetX, int offsetY) {
	int center = radius - 1;
	SDL_Surface* lightSurface = SDL_CreateRGBSurfaceWithFormat(0, textureWidth, textureHeight, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_FillRect(lightSurface, NULL, SDL_MapRGBA(lightSurface->format, blackColor.r, blackColor.g, blackColor.b, blackColor.a));
	SDL_Rect pos = { radius + offsetX, radius + offsetY, areaWidth, areaHeight };
	SDL_FillRect(lightSurface, &pos, SDL_MapRGBA(lightSurface->format, lightValue, lightValue, lightValue, 255));
	pos = { 0, 0, 1, 1 };
	for (int x = 0; x < textureWidth; x++) {
		for (int y = 0; y < textureHeight; y++) {
			if (!((x > center && x <= center + areaWidth) && (y > center && y <= center + areaHeight))) {
				int centerAddX = 0;
				int centerAddY = 0;
				if (x > center + areaWidth) { centerAddX = areaWidth + 1; }
				else if (x > center) { centerAddX = x - center; }
				if (y > center + areaHeight) { centerAddY = areaHeight + 1; }
				else if (y > center) { centerAddY = y - center; }
				double dx = x - (center + centerAddX);
				double dy = y - (center + centerAddY);
				double d = sqrt(dx*dx + dy * dy);
				if (d > center) { d = center; }
				int val = (int)(lightValue * (1.0 - (d / center)));
				pos.x = x + offsetX;
				pos.y = y + offsetY;
				SDL_FillRect(lightSurface, &pos, SDL_MapRGBA(lightSurface->format, val, val, val, 255));
			}
		}
	}
	return lightSurface;
}

SDL_Texture* drawLightTexture(int radius, int areaWidth, int areaHeight, int textureWidth, int textureHeight, int offsetX, int offsetY) {
	SDL_Surface* lightSurface = drawLightSurface(radius, areaWidth, areaHeight, textureWidth, textureHeight, offsetX, offsetY);
	SDL_Texture* lightTexture = SDL_CreateTextureFromSurface(renderer, lightSurface);
	SDL_SetTextureBlendMode(lightTexture, SDL_BLENDMODE_ADD);
	SDL_FreeSurface(lightSurface);
	return lightTexture;
}

SDL_Texture* drawBlackScreen(int width, int height) {
	SDL_Surface* pauseScreenSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_FillRect(pauseScreenSurface, NULL, SDL_MapRGBA(pauseScreenSurface->format, blackScreenColor.r, blackScreenColor.g, blackScreenColor.b, blackScreenColor.a));
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, pauseScreenSurface);
	SDL_FreeSurface(pauseScreenSurface);
	return texture;
}

void drawTitleScreenCell(SDL_Surface* titleScreenSurface, int x, int y, int val) {
	SDL_Rect pos = { x*blockSizeInPixel + 1, y*blockSizeInPixel + 1, blockSizeInPixel - 2, blockSizeInPixel - 2 };
	SDL_FillRect(titleScreenSurface, &pos, SDL_MapRGB(titleScreenSurface->format, blackColor.r, blackColor.g, blackColor.b));
	if (y == 0 && ((val & NORTH_VAL) == NORTH_VAL)) {
		pos = { x*blockSizeInPixel + 1, 0, blockSizeInPixel - 2, 1 };
		SDL_FillRect(titleScreenSurface, &pos, SDL_MapRGB(titleScreenSurface->format, blackColor.r, blackColor.g, blackColor.b));
	}
	if (x == 0 && ((val & WEST_VAL) == WEST_VAL)) {
		pos = { 0, y*blockSizeInPixel + 1, 1, blockSizeInPixel - 2 };
		SDL_FillRect(titleScreenSurface, &pos, SDL_MapRGB(titleScreenSurface->format, blackColor.r, blackColor.g, blackColor.b));
	}
	if ((val & SOUTH_VAL) == SOUTH_VAL) {
		pos = { x*blockSizeInPixel + 1, (y + 1)*blockSizeInPixel - 1, blockSizeInPixel - 2, 2 };
		SDL_FillRect(titleScreenSurface, &pos, SDL_MapRGB(titleScreenSurface->format, blackColor.r, blackColor.g, blackColor.b));
	}
	if ((val & EAST_VAL) == EAST_VAL) {
		pos = { (x + 1)*blockSizeInPixel - 1, y*blockSizeInPixel + 1, 2, blockSizeInPixel - 2 };
		SDL_FillRect(titleScreenSurface, &pos, SDL_MapRGB(titleScreenSurface->format, blackColor.r, blackColor.g, blackColor.b));
	}
}

void drawTitleScreen() {
	// Maze
	Maze maze(windowWidth + 2, windowHeight + 2);
	// Title Screen Texture
	SDL_Surface* titleScreenSurface = SDL_CreateRGBSurfaceWithFormat(0, windowWidthInPixel, windowHeightInPixel, 32, SDL_PIXELFORMAT_RGB888);
	SDL_FillRect(titleScreenSurface, NULL, SDL_MapRGB(titleScreenSurface->format, greyColor.r, greyColor.g, greyColor.b));
	for (int x = 0; x < windowWidth; x++) {
		for (int y = 0; y < windowHeight; y++) {
			drawTitleScreenCell(titleScreenSurface, x, y, maze.maze[x + 1][y + 1]);
		}
	}
	titleScreenTexture = SDL_CreateTextureFromSurface(renderer, titleScreenSurface);
	SDL_SetTextureColorMod(titleScreenTexture, blueColor.r, blueColor.g, blueColor.b);
	currTitleScreenLightColor = blueColor;
	SDL_FreeSurface(titleScreenSurface);
	// Set Lights
	RandomGenerator rb(1, 254);
	RandomGenerator rd(0, 1);
	RandomGenerator rs(1, lightChangeSpeedLevels);
	int i = 0;
	for (int x = 0; x < titleScreenNumLightsInRow; x++) {
		for (int y = 0; y < titleScreenNumLightsInRow; y++) {
			titleScreenLightsPos[i][0] = titleScreenLightRadiusInPixel * (x - 1) + blockSizeInPixel / 2;
			titleScreenLightsPos[i][1] = titleScreenLightRadiusInPixel * (y - 1) + blockSizeInPixel;
			titleScreenLightsVec[i] = rs.getNextValue() * (2 * rd.getNextValue() - 1);
			titleScreenLightsVal[i] = rb.getNextValue() / 255.0;
			i++;
		}
	}
}

void drawFreeBlock(SDL_Surface* surface, int x, int y) {
	SDL_Rect currPos = { x*blockSizeInPixel, y*blockSizeInPixel, blockSizeInPixel, blockSizeInPixel };
	SDL_BlitSurface(freeBlockSurface, NULL, surface, &currPos);
}

void drawBlock(SDL_Surface* surface, int x, int y, const SDL_Color& color) {
	SDL_Rect currPos = { x*blockSizeInPixel, y*blockSizeInPixel, blockSizeInPixel, blockSizeInPixel };
	SDL_FillRect(surface, &currPos, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
}

void drawDroneAttackArea(SDL_Surface* droneAttackArea) {
	for (int x = -attackAreaRadius; x <= attackAreaRadius; x++) {
		for (int y = -attackAreaRadius; y <= attackAreaRadius; y++) {
			int ax = abs(x);
			int ay = abs(y);
			if (ax == 0 && ay == 0) {
				drawBlock(droneAttackArea, x + attackAreaRadius, y + attackAreaRadius, emptyColor);
			}
			else {
				drawBlock(droneAttackArea, x + attackAreaRadius, y + attackAreaRadius, attackAreaColor);
			}
		}
	}
}

void drawSwitchAttackArea(SDL_Surface* switchAttackArea, int status) {
	for (int x = -attackAreaRadius; x <= attackAreaRadius; x++) {
		for (int y = -attackAreaRadius; y <= attackAreaRadius; y++) {
			if ((x != 0 || y != 0) && ((status == 0 && y == 0) || (status == 1 && x == 0))) {
				drawBlock(switchAttackArea, (x + attackAreaRadius), (y + attackAreaRadius), attackAreaColor);
			}
			else {
				drawBlock(switchAttackArea, (x + attackAreaRadius), (y + attackAreaRadius), emptyColor);
			}
		}
	}
}

void drawAttackAreas() {
	// Drone Block Attack Area Texture
	SDL_Surface* droneAttackAreaSurface = SDL_CreateRGBSurfaceWithFormat(0, attackAreaSizeInPixel, attackAreaSizeInPixel, 32, SDL_PIXELFORMAT_RGBA8888);
	drawDroneAttackArea(droneAttackAreaSurface);
	droneAttackAreaTexture = SDL_CreateTextureFromSurface(renderer, droneAttackAreaSurface);
	SDL_FreeSurface(droneAttackAreaSurface);
	// Switch Attack Area Texture
	for (int i = 0; i <= 1; i++) {
		SDL_Surface* switchAttackAreaSurface = SDL_CreateRGBSurfaceWithFormat(0, attackAreaSizeInPixel, attackAreaSizeInPixel, 32, SDL_PIXELFORMAT_RGBA8888);
		drawSwitchAttackArea(switchAttackAreaSurface, i);
		switchAttackAreaTexture[i] = SDL_CreateTextureFromSurface(renderer, switchAttackAreaSurface);
		SDL_FreeSurface(switchAttackAreaSurface);
	}
	switchAttackAreaTexture[2] = droneAttackAreaTexture;
}

void drawMap(SDL_Surface* surface) {
	for (int x = 0; x < map.width; x++) {
		for (int y = 0; y < map.height; y++) {
			if (map(x,y) == freeVal) {
				drawFreeBlock(surface, x, y);
			}
		}
	}
}

// === Save File ===

void updateSaveFile() {
	if (points > gameSave.highScore[course]) {
		newHighScore = true;
		gameSave.highScore[course] = points;
		highScoreText->updateText(to_string(gameSave.highScore[course]));
	}
	writeSaveFile();
}

void writeSaveFile() {
	ofstream saveFile(saveFileName);
	saveFile.write((char*)&gameSave, sizeof(gameSave));
	saveFile.close();
}

void readSaveFile() {
	ifstream saveFile(saveFileName);
	if (saveFile) {
		saveFile.read((char*)&gameSave, sizeof(gameSave));
	}
	saveFile.close();
}

// === Game Variables ===

void initGame(InitMode mode) {
	if (mode == FIRST_START) {
		// Game Status
		gameStatus = BEFORE_START;
		// GameSave
		for (int i = 0; i < numCourses; i++) {
			gameSave.highScore[i] = 0;
		}
		readSaveFile();
		for (int i = 0; i < numCourses; i++) {
			selectionText[i*numTextsPerCourse + 2]->updateText(to_string(gameSave.highScore[i]));
		}
		// Course
		course = -1;
		// is Running
		isRunning = true;
	}
	else if (mode == RESTART_COURSE) {
		gameStatus = RUNNING;
	}
	else if (mode == CHANGE_COURSE) {
		// Game Status
		gameStatus = BEFORE_START;
		gameStatusText[0]->updateText(beforeStartText);
		// HighScore
		for (int i = 0; i < numCourses; i++) {
			selectionText[i*numTextsPerCourse + 2]->updateText(to_string(gameSave.highScore[i]));
		}
	}
	// start fade in
	runningMode = START_FADE_IN;
	// new High Score
	newHighScore = false;
	// Game Status
	level = 1;
	points = 0;
	// Game step count
	gameStepCount = 0;
	// Level status
	levelMazeWidth = mazeWidths[level - 1];
	levelMazeHeight = mazeHeights[level - 1];
	levelKeyItems = level;
	levelBonusItems = 0;
	// Level Enemies init
	levelDrones = 0;
	levelShooters = 0;
	levelSwitches = 0;
	levelPatrols = 0;
	// Random Enemy select
	RandomGenerator enemyRnd(0, numEnemyTypes - 1);
	int r = enemyRnd.getNextValue();
	if (r == 0) { levelDrones++; }
	else if (r == 1) { levelShooters++; }
	else if (r == 2) { levelSwitches++; }
	else if (r == 3) { levelPatrols++; }
	// Player directions
	playerDirections.clear();
	// Player sprite angle
	playerRotation = 0;
	// Player Alpha
	playerAlpha = 0.0;
	// Map data
	if (mode == RESTART_COURSE || mode == CHANGE_COURSE) {
		// Free map texture
		SDL_DestroyTexture(mapTexture);
		mapTexture = NULL;
	}
	createMapData();
	// Set Drone color
	SDL_SetTextureColorMod(droneBlockTexture, droneIdleColor.r, droneIdleColor.g, droneIdleColor.b);
	if (mode == FIRST_START) {
		// Player
		player = Player(map.startX, map.startY);
		// Keyboard
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		EscPressed = state[SDL_SCANCODE_ESCAPE];
		WPressed = state[SDL_SCANCODE_W];
		APressed = state[SDL_SCANCODE_A];
		SPressed = state[SDL_SCANCODE_S];
		DPressed = state[SDL_SCANCODE_D];
		// status values
		levelText->updateText(getLevelStatus());
		keyItemsText->updateText(getItemStatus());
		pointsText->updateText(to_string(points));
	}
	else if (mode == RESTART_COURSE || mode == CHANGE_COURSE) {
		// Player and view
		initPlayer();
		// status values
		if (mode == RESTART_COURSE) {
			resetPlayerHealth();
		}
		levelText->updateText(getLevelStatus());
		keyItemsText->updateText(getItemStatus());
		pointsText->updateText(to_string(points));
		// status colors
		keyItemsText->updateColor(statusColor);
		highScoreText->updateColor(statusColor);
		pointsText->updateColor(statusColor);
	}
}

void closeGame() {
	// Free map texture
	SDL_DestroyTexture(mapTexture);
}

// === Map Data ===

void createMapData() {
	// Map
	if (level < maxLevel) {
		// Maze
		int bonusValue = (int)(ceil(level / 2.0) - 1);
		map = Map(levelMazeWidth, levelMazeHeight, levelKeyItems, levelBonusItems, levelDrones, levelShooters, levelSwitches, levelPatrols, bonusValue);
	}
	else {
		// Boss
		map = Map(course);
	}
	// Map Surface
	mapWidthInPixel = map.width * blockSizeInPixel;
	mapHeightInPixel = map.height * blockSizeInPixel;
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, mapWidthInPixel, mapHeightInPixel, 32, SDL_PIXELFORMAT_RGBA8888);
	// Draw Map
	drawMap(surface);
	// Map Texture
	mapTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

// === Status Update ===

void resetPlayerHealth() {
	player.health = playerMaxHealth;
	healthText->updateText(getHealthStatus());
	healthText->updateColor(statusColor);
}

void initPlayer() {
	// Set player
	player.w = 0.0;
	player.steps = 0;
	player.prevPosX = map.startX;
	player.prevPosY = map.startY;
	player.nextPosX = map.startX;
	player.nextPosY = map.startY;
	player.dx = 0;
	player.dy = 0;
	player.isMoving = false;
}

void setNextMap() {
	// Set drone color
	SDL_SetTextureColorMod(droneBlockTexture, droneIdleColor.r, droneIdleColor.g, droneIdleColor.b);
	// Level
	level++;
	if (level < maxLevel) {
		// Key Items
		levelKeyItems = level;
		// Width, Height
		int w = mazeWidths[level - 1];
		int h = mazeHeights[level - 1];
		if (w != h) {
			int v[2] = { 0, 1 };
			std::random_shuffle(std::begin(v), std::end(v));
			if (v[0] == 0) {
				levelMazeWidth = w;
				levelMazeHeight = h;
			}
			else {
				levelMazeWidth = h;
				levelMazeHeight = w;
			}
		}
		else {
			levelMazeWidth = w;
			levelMazeHeight = h;
		}
		// Init enemies
		levelDrones = 0;
		levelShooters = 0;
		levelSwitches = 0;
		levelPatrols = 0;
		int numFreeCells = levelMazeWidth * levelMazeHeight - levelKeyItems - 2;
		int numEnemies = (int)(ceil(currEnemyRatio*numFreeCells));
		// Number of Bonus Items
		levelBonusItems = numFreeCells - numEnemies;
		// Random Enemy select
		RandomGenerator enemyRnd(0, numEnemyTypes - 1);
		for (int i = 0; i < numEnemies; i++) {
			int r = enemyRnd.getNextValue();
			if (r == 0) { levelDrones++; }
			else if (r == 1) { levelShooters++; }
			else if (r == 2) { levelSwitches++; }
			else if (r == 3) { levelPatrols++; }
		}
	}
	// Free map texture
	SDL_DestroyTexture(mapTexture);
	mapTexture = NULL;
	// Create new map
	createMapData();
	// Player
	initPlayer();
	if (course != 2) {
		resetPlayerHealth();
	}
	else {
		healthText->updateColor(statusColor);
	}
	// Update status
	levelText->updateText(getLevelStatus());
	keyItemsText->updateColor(statusColor);
	keyItemsText->updateText(getItemStatus());
	// Game Step Count
	gameStepCount = 0;
}

void updateStatus() {
	if (map.isExitOpen) {
		if ( ( (int)(player.getX()) ) == map.exitX && ( (int)(player.getY()) ) == map.exitY ) {
			// Next map
			runningMode = NEXT_MAP;
			// Update points
			points += currKeyItemValue;
			pointsText->updateText(to_string(points));
		}
	}
	else {
		int cx = (int)(player.getX());
		int cy = (int)(player.getY());
		// Collect Key Item
		int size = (int)(map.keyItems.size());
		for (int i = 0; i < size; i++) {
			Point& p = map.keyItems.at(i);
			if (cx == p.x && cy == p.y) {
				// Update points
				points += currKeyItemValue;
				// Delete Key Item
				map.keyItems.erase(map.keyItems.begin() + i);
				// Update status
				map.nrCollectedKeyItems++;
				if (map.nrCollectedKeyItems == map.nrKeyItems) {
					map.isExitOpen = true;
					keyItemsText->updateColor(currCourseColorBright);
					if (map.isBossMap) {
						// --- Update Boss map ---
						map.removeBosses();
					}
				}
				keyItemsText->updateText(getItemStatus());
				pointsText->updateText(to_string(points));
				break;
			}
		}
	}
	// Collect Bonus Item
	int cx = (int)(player.getX());
	int cy = (int)(player.getY());
	int size = (int)(map.bonusItems.size());
	for (int i = 0; i < size; i++) {
		BonusItem& b = map.bonusItems.at(i);
		if (cx == b.x && cy == b.y) {
			// Add bonus points
			points = points + b.value;
			// Delete Bonus Item
			map.bonusItems.erase(map.bonusItems.begin() + i);
			// Update status
			pointsText->updateText(to_string(points));
			break;
		}
	}
}

// === Course ===

void updateCourse(int c) {
	if (course != c) {
		// Set Course
		course = c;
		// Set High Score
		highScoreText->updateText(to_string(gameSave.highScore[course]));
		// Set Enemy Ratio
		currEnemyRatio = enemyRatios[course];
		// Set Key Item Value
		currKeyItemValue = keyItemValues[course];
		// Set Course Color
		currCourseColorDark = courseColorsDark[course];
		currCourseColorBright = courseColorsBright[course];
		// Update Status Bar Surface
		SDL_SetSurfaceColorMod(keyItemBlockSurface, currCourseColorDark.r, currCourseColorDark.g, currCourseColorDark.b);
		SDL_Rect posDst = { keyItemsTextOffsetInPixel - blockSizeInPixel + 1, 1, blockSizeInPixel - 2, blockSizeInPixel - 2 };
		SDL_BlitSurface(keyItemBlockSurface, NULL, statusBarSurface, &posDst);
		// Update Status Bar Texture
		SDL_DestroyTexture(statusBarTexture);
		statusBarTexture = NULL;
		statusBarTexture = SDL_CreateTextureFromSurface(renderer, statusBarSurface);
		// Update Player Block Surface
		SDL_SetSurfaceColorMod(faceSurface, currCourseColorBright.r, currCourseColorBright.g, currCourseColorBright.b);
		posDst = { faceOffsetXInPixel, faceOffsetYInPixel, faceWidthInPixel, faceHeightInPixel };
		SDL_BlitSurface(faceSurface, NULL, playerBlockSurface, &posDst);
		// Update Player Block Texture
		SDL_DestroyTexture(playerBlockTexture);
		playerBlockTexture = NULL;
		playerBlockTexture = SDL_CreateTextureFromSurface(renderer, playerBlockSurface);
		// Set Texture Colors (Key Item, Exit)
		SDL_SetTextureColorMod(keyItemBlockTexture, currCourseColorDark.r, currCourseColorDark.g, currCourseColorDark.b);
		SDL_SetTextureColorMod(exitBlockTexture, currCourseColorBright.r, currCourseColorBright.g, currCourseColorBright.b);
	}
	// Reset Player Health
	resetPlayerHealth();
}

// === Player Direction ===

void addPlayerDirection(DIR dir) {
	if (playerDirections.size()<maxPlayerDirections) {
		playerDirections.push_back(dir);
	}
}

void removePlayerDirection(DIR dir) {
	playerDirections.erase(std::remove(playerDirections.begin(), playerDirections.end(), dir), playerDirections.end());
}

// === Unit Movement ===

bool isMovementPossible(Unit &unit, int dx, int dy){
	int cx = (int)(unit.prevPosX + dx);
	int cy = (int)(unit.prevPosY + dy);
	// Wall
	if (map(cx,cy) == wallVal) {
		return false;
	}
	// Drone
	int size = map.nrDrones;
	for (int i = 0; i < size; i++) {
		if (map.drones[i].nextPosX == cx && map.drones[i].nextPosY == cy) {
			return false;
		}
	}
	// Player
	if (cx == player.nextPosX && cy == player.nextPosY) {
		return false;
	}
	return true;
}

bool moveUnit(Unit &unit, int dx, int dy) {
	if (isMovementPossible(unit, dx, dy)) {
		unit.move(dx, dy);
		return true;
	}
	else {
		return false;
	}
}

void updateUnitPosition(Unit &unit) {
	unit.steps++;
	unit.w = ((double)(unit.steps)) / nrMovementSteps;
	if (unit.steps == nrMovementSteps) {
		unit.resetMovement();
		if (unit.isPlayer) {
			updateStatus();
		}
	}
}

// === Enemies ===

void activateBosses() {
	int size = map.nrBosses;
	for (int i = 0; i < size; i++) {
		Boss& b = map.bosses[i];
		double bx = b.getX();
		double by = b.getY();
		double dx = player.getX() - bx;
		double dy = player.getY() - by;
		double n = sqrt(dx*dx + dy * dy);
		double dnx = dx / n;
		double dny = dy / n;
		map.projectiles.push_back(Projectile(bx, by, dnx, dny, true));
	}
}

void activateShooters() {
	int size = map.nrShooters;
	for (int i = 0; i < size; i++) {
		Shooter& shooter = map.shooters[i];
		double x = shooter.getX();
		double y = shooter.getY();
		map.projectiles.push_back(Projectile(x, y, 0, -1, false));
		map.projectiles.push_back(Projectile(x, y, -1, 0, false));
		map.projectiles.push_back(Projectile(x, y, 0, 1, false));
		map.projectiles.push_back(Projectile(x, y, 1, 0, false));
	}
}

bool isDroneInRange(Drone& drone, int dx, int dy) {
	double x = drone.getX();
	double y = drone.getY();
	int rx = (int)abs(x + dx - drone.cx);
	int ry = (int)abs(y + dy - drone.cy);
	return (rx <= droneRange && ry <= droneRange);
}

void moveDrones() {
	int size = map.nrDrones;
	for (int i = 0; i < size; i++) {
		Drone& drone = map.drones[i];
		DIR dirs[4] = { NORTH, WEST, SOUTH, EAST };
		std::random_shuffle(std::begin(dirs), std::end(dirs));
		for (int i = 0; i < 4; i++) {
			Direct direct(dirs[i]);
			if (isDroneInRange(drone, direct.dx, direct.dy) && moveUnit(drone, direct.dx, direct.dy)) {
				break;
			}
		}
	}
}

// === Hit Detection ===

bool isHitByDrone() {
	int size = map.nrDrones;
	for (int i = 0; i < size; i++) {
		Drone& drone = map.drones[i];
		// Check Distance to Player
		double dx = abs(player.getX() - drone.getX());
		if (dx <= attackAreaGameOverThresholdSquare) {
			double dy = abs(player.getY() - drone.getY());
			if (dy <= attackAreaGameOverThresholdSquare) {
				return true;
			}
		}
	}
	return false;
}

bool isHitBySwitch() {
	int size = map.nrSwitches;
	for (int i = 0; i < size; i++) {
		Switch& s = map.switches[i];
		// Switch State 0
		if (map.switchState == 0) {
			double dx = abs(player.getX() - s.getX());
			if (dx <= attackAreaGameOverThresholdSquare) {
				double dy = abs(player.getY() - s.getY());
				if (dy <= attackAreaGameOverThresholdLine) {
					return true;
				}
			}
		}
		// Switch State 1
		else if (map.switchState == 1) {
			double dx = abs(player.getX() - s.getX());
			if (dx <= attackAreaGameOverThresholdLine) {
				double dy = abs(player.getY() - s.getY());
				if (dy <= attackAreaGameOverThresholdSquare) {
					return true;
				}
			}
		}
		// Switch State 2
		else if (map.switchState == 2) {
			double dx = abs(player.getX() - s.getX());
			if (dx <= attackAreaGameOverThresholdSquare) {
				double dy = abs(player.getY() - s.getY());
				if (dy <= attackAreaGameOverThresholdSquare) {
					return true;
				}
			}
		}
	}
	return false;
}

bool isHitByPatrol() {
	int size = map.nrPatrols;
	for (int i = 0; i < size; i++) {
		Patrol& patrol = map.patrols[i];
		// Check Distance to Player
		double dx = abs(player.getX() - patrol.getX());
		if (dx <= projectileGameOverThreshold) {
			double dy = abs(player.getY() - patrol.getY());
			if (dy <= projectileGameOverThreshold) {
				return true;
			}
		}
	}
	return false;
}

bool isHitByProjectile() {
	int size = (int)(map.projectiles.size());
	for (int i = 0; i < size; i++) {
		Projectile& projectile = map.projectiles.at(i);
		// Check Distance to Player
		double dx = abs(player.getX() - projectile.getX());
		if (dx <= projectileGameOverThreshold) {
			double dy = abs(player.getY() - projectile.getY());
			if (dy <= projectileGameOverThreshold) {
				return true;
			}
		}
	}
	return false;
}

// === Game Status Screens ===

void showBeforeStartScreen() {
	// Show Screen
	SDL_RenderCopy(renderer, titleScreenTexture, NULL, NULL);
	renderTitleScreenLights();
	gameStatusText[0]->showText();
	for (int i = 0; i < numCourses; i++) {
		for (int j = 0; j < numTextsPerCourse; j++) {
			selectionText[i*numTextsPerCourse + j]->showText();
		}
	}
	for (int i = 0; i < 2; i++) {
		exitSelectionText[i]->showText();
	}
	for (int i = 0; i < 4; i++) {
		infoSelectionText[i]->showText();
	}
	SDL_RenderCopy(renderer, highScoreTexture, NULL, &selectionHighScorePos);
	// Check for Input
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	// Check if Number is pressed
	bool numberPressed = false;
	int selCourse = 0;
	for (int i = 0; i < numCourses; i++) {
		if (state[SDL_SCANCODE_1 + i]) {
			numberPressed = true;
			selCourse = i;
			break;
		}
	}
	if (numberPressed) {
		updateCourse(selCourse);
		gameStatus = RUNNING;
	}
	// Check if Escape is pressed
	bool EscCurr = state[SDL_SCANCODE_ESCAPE];
	if (EscPressed != EscCurr) {
		EscPressed = EscCurr;
		if (EscPressed) {
			// Exit Game
			isRunning = false;
		}
	}
	// Check if F1 is pressed
	if (state[SDL_SCANCODE_F1]) {
		gameStatus = MANUAL;
	}
	// Check if F2 is pressed
	if (state[SDL_SCANCODE_F2]) {
		gameStatus = CREDITS;
	}
}

void showInfoScreen() {
	// Offset
	int offset = 0;
	if (gameStatus == CREDITS) {
		offset = 3;
	}
	// Show Screen
	SDL_RenderCopy(renderer, titleScreenTexture, NULL, NULL);
	renderTitleScreenLights();
	for (int i = 0; i < 3; i++) {
		informationSelectionText[i + offset]->showText();
	}
	// Check for Input
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	// Check if Escape is pressed
	bool EscCurr = state[SDL_SCANCODE_ESCAPE];
	if (EscPressed != EscCurr) {
		EscPressed = EscCurr;
		if (EscPressed) {
			gameStatus = BEFORE_START;
		}
	}
}

void showPauseScreen() {
	// Show Status
	static bool showPauseStatusOnScreen = true;
	if (showPauseStatusOnScreen) {
		gameStatusText[0]->updateText(pauseText[0]);
		gameStatusText[1]->updateText(pauseText[1]);
		showPauseStatusOnScreen = false;
	}
	// Render Game View
	renderGameView();
	// Show Text
	SDL_RenderCopy(renderer, blackScreenTexture, NULL, NULL);
	gameStatusText[0]->showText();
	gameStatusText[1]->showText();
	// Check if Space Bar is pressed
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_SPACE]) {
		showPauseStatusOnScreen = true;
		gameStatus = RUNNING;
	}
	// Check if Escape is pressed
	bool EscCurr = state[SDL_SCANCODE_ESCAPE];
	if (EscPressed != EscCurr) {
		EscPressed = EscCurr;
		if (EscPressed) {
			showPauseStatusOnScreen = true;
			initGame(CHANGE_COURSE);
		}
	}
}

void showGameOverScreen() {
	static bool statusVisible = false;
	static int blinks = 0;
	static int currFrames = 0;
	// Status 0 - Blinking
	if (!statusVisible) {
		// Frames
		currFrames++;
		if (currFrames == 1) {
			SDL_SetTextureAlphaMod(playerBlockTexture, 0);
		}
		if (currFrames % blinkFrames == 0) {
			blinks++;
		}
		// Blinks
		if (blinks % 2 == 1) {
			SDL_SetTextureAlphaMod(playerBlockTexture, 255);
			if (newHighScore) {
				highScoreText->updateColor(currCourseColorBright);
				pointsText->updateColor(currCourseColorBright);
			}
		}
		else if (blinks < nrBlinks) {
			SDL_SetTextureAlphaMod(playerBlockTexture, 0);
			if (newHighScore) {
				highScoreText->updateColor(statusColor);
				pointsText->updateColor(statusColor);
			}
		}
		// End blinks
		if (blinks == nrBlinks) {
			statusVisible = true;
			blinks = 0;
			currFrames = 0;
			// Update Text
			gameStatusText[0]->updateText(gameOverText[0]);
			gameStatusText[1]->updateText(gameOverText[1]);
		}
	}
	// Render Game View
	renderGameView();
	// Status is visible
	if (statusVisible) {
		// Show Text
		SDL_RenderCopy(renderer, blackScreenTexture, NULL, NULL);
		gameStatusText[0]->showText();
		gameStatusText[1]->showText();
		// Check if Space Bar is pressed
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_SPACE]) {
			statusVisible = false;
			initGame(RESTART_COURSE);
		}
		// Check if Escape is pressed
		bool EscCurr = state[SDL_SCANCODE_ESCAPE];
		if (EscPressed != EscCurr) {
			EscPressed = EscCurr;
			if (EscPressed) {
				statusVisible = false;
				initGame(CHANGE_COURSE);
			}
		}
	}
}

void showFinishedScreen() {
	static bool textUpdated = false;
	static bool blinksFinished = false;
	static int blinks = 0;
	static int currFrames = 0;
	// Update Text
	if (!textUpdated) {
		int missedPoints = maxPoints - points;
		if (missedPoints == 0) {
			SDL_SetTextureColorMod(titleScreenTexture, currCourseColorBright.r, currCourseColorBright.g, currCourseColorBright.b);
			currTitleScreenLightColor = currCourseColorBright;
		}
		else {
			SDL_SetTextureColorMod(titleScreenTexture, blueColor.r, blueColor.g, blueColor.b);
			currTitleScreenLightColor = blueColor;
		}
		gameStatusText[0]->updateText(finishedText[0]);
		gameStatusText[1]->updateText(finishedText[1]);
		resetPlayerHealth();
		textUpdated = true;
	}
	// Render Finished Screen Text
	renderFinishedScreenText();
	// Blinking
	if (!blinksFinished) {
		if (newHighScore) {
			// Frames
			currFrames++;
			if (currFrames % blinkFrames == 0) {
				blinks++;
			}
			// Blinks
			if (blinks % 2 == 1) {
				highScoreText->updateColor(currCourseColorBright);
				pointsText->updateColor(currCourseColorBright);
			}
			else if (blinks < nrBlinks) {
				highScoreText->updateColor(statusColor);
				pointsText->updateColor(statusColor);
			}
			// End blinks
			if (blinks == nrBlinks) {
				blinksFinished = true;
				blinks = 0;
				currFrames = 0;
			}
		}
		else {
			blinksFinished = true;
		}
	}
	else {
		// Check if Space Bar is pressed
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_SPACE]) {
			textUpdated = false;
			blinksFinished = false;
			SDL_SetTextureColorMod(titleScreenTexture, blueColor.r, blueColor.g, blueColor.b);
			currTitleScreenLightColor = blueColor;
			initGame(RESTART_COURSE);
		}
		// Check if Escape is pressed
		bool EscCurr = state[SDL_SCANCODE_ESCAPE];
		if (EscPressed != EscCurr) {
			EscPressed = EscCurr;
			if (EscPressed) {
				textUpdated = false;
				blinksFinished = false;
				SDL_SetTextureColorMod(titleScreenTexture, blueColor.r, blueColor.g, blueColor.b);
				currTitleScreenLightColor = blueColor;
				initGame(CHANGE_COURSE);
			}
		}
	}
}

bool showRunningScreen() {
	// Check for Paused
	if (checkForPaused()) { return true; }
	// --- START FADE IN ---
	if (runningMode == START_FADE_IN) {
		// Fade in
		fadeIn();
	}
	// --- NEXT MAP ---
	else if (runningMode == NEXT_MAP) {
		// Next Map Update
		if (nextMapUpdate()) { return true; }
	}
	// --- KEYBOARD OPEN ---
	else if (runningMode == KEYBOARD_OPEN) {
		// Update Enemy pattern
		updateEnemyPattern();
		// player movement
		if (!player.isMoving) {
			updatePlayerMovement();
		}
		// Update Positions
		updatePositions();
		// Update player time out
		updatePlayerTimeOut();
		// Check for Game Over
		if (checkForGameOver()) { return true; }
	}
	// Render Game View
	renderGameView();
	return false;
}

// === Game Running ===

void updatePlayerTimeOut() {
	if (player.timeOutFrames > 0) {
		player.timeOutFrames--;
		if (player.timeOutFrames == 0) {
			SDL_SetTextureAlphaMod(playerBlockTexture, 255);
			healthText->updateColor(statusColor);
		}
	}
}

bool checkForPaused() {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	bool EscCurr = state[SDL_SCANCODE_ESCAPE];
	if (EscPressed != EscCurr) {
		EscPressed = EscCurr;
		if (EscPressed) {
			// Pause game
			gameStatus = PAUSED;
			return true;
		}
	}
	return false;
}

bool checkForGameOver() {
	if (player.timeOutFrames == 0 && ((map.droneAttack && isHitByDrone()) || isHitBySwitch() || isHitByPatrol() || isHitByProjectile())) {
		// Decrease health
		player.health--;
		healthText->updateText(getHealthStatus());
		healthText->updateColor(redColor);
		if (player.health > 0) {
			// Time out
			player.timeOutFrames = playerTimeoutFramesStart;
			SDL_SetTextureAlphaMod(playerBlockTexture, playerTimeOutColor.a);
		}
		else {
			// Game Over
			gameStatus = GAME_OVER;
			// Update Save File
			updateSaveFile();
			return true;
		}
	}
	return false;
}

void fadeIn() {
	playerAlpha += alphaStepSize;
	if (playerAlpha >= 1.0) {
		// Reset alpha
		playerAlpha = 1.0;
		// Set gameStatus
		runningMode = KEYBOARD_OPEN;
	}
	SDL_SetTextureAlphaMod(playerBlockTexture, (Uint8)(255 * playerAlpha));
}

bool nextMapUpdate() {
	static bool fadeOut = true;
	// Change playerAlpha
	if (fadeOut) { playerAlpha -= alphaStepSize; }
	else { playerAlpha += alphaStepSize; }
	// End ouf fade out
	if (fadeOut && playerAlpha <= 0.0) {
		// Reset alpha
		playerAlpha = 0.0;
		// Transition
		if (level == maxLevel) {
			// Fade Out
			fadeOut = true;
			// Set gameStatus
			gameStatus = FINISHED;
			// High Score
			updateSaveFile();
			return true;
		}
		else {
			// Fade In
			fadeOut = false;
			// Set Next Map
			setNextMap();
		}
	}
	// End of fade in
	else if (!fadeOut && playerAlpha >= 1.0) {
		// Reset alpha
		playerAlpha = 1.0;
		// Fade Out
		fadeOut = true;
		// Set runningMode
		runningMode = KEYBOARD_OPEN;
	}
	SDL_SetTextureAlphaMod(playerBlockTexture, (Uint8)(255 * playerAlpha));
	return false;
}

void updateEnemyPattern() {
	// Game Step
	gameStepCount++;
	if (gameStepCount == gameStepStatusA) {
		// Move Drones
		moveDrones();
	}
	else if (gameStepCount == gameStepStatusB) {
		// Load Drones
		SDL_SetTextureColorMod(droneBlockTexture, redColor.r, redColor.g, redColor.b);
		map.isDroneRed = true;
		// Activate shooters
		activateShooters();
		// Switch State
		map.switchState = ((map.switchState + 1) % 2);
	}
	else if (gameStepCount == gameStepStatusC) {
		// Fire Drones
		map.droneAttack = true;
		// Switch State
		map.prevSwitchState = map.switchState;
		map.switchState = 2;
	}
	else if (gameStepCount == gameStepStatusD) {
		// Set Drones to IDLE
		map.droneAttack = false;
		map.isDroneRed = false;
		SDL_SetTextureColorMod(droneBlockTexture, droneIdleColor.r, droneIdleColor.g, droneIdleColor.b);
		// Switch State
		map.switchState = map.prevSwitchState;
		// Reset game step count
		gameStepCount = 0;
	}
	if (map.isBossMap && map.nrBosses > 0 && (gameStepCount % bossPause == 1)) {
		// Boss
		activateBosses();
	}
}

void updatePlayerMovement() {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	// Update player direction - North
	bool Wcurr = state[SDL_SCANCODE_W];
	if (WPressed != Wcurr) {
		WPressed = Wcurr;
		if (WPressed) { addPlayerDirection(NORTH); }
		else { removePlayerDirection(NORTH); }
	}
	// Update player direction - WEST
	bool Acurr = state[SDL_SCANCODE_A];
	if (APressed != Acurr) {
		APressed = Acurr;
		if (APressed) { addPlayerDirection(WEST); }
		else { removePlayerDirection(WEST); }
	}
	// Update player direction - SOUTH
	bool Scurr = state[SDL_SCANCODE_S];
	if (SPressed != Scurr) {
		SPressed = Scurr;
		if (SPressed) { addPlayerDirection(SOUTH); }
		else { removePlayerDirection(SOUTH); }
	}
	// Update player direction - EAST
	bool Dcurr = state[SDL_SCANCODE_D];
	if (DPressed != Dcurr) {
		DPressed = Dcurr;
		if (DPressed) { addPlayerDirection(EAST); }
		else { removePlayerDirection(EAST); }
	}
	// Set movement
	if (playerDirections.size() > 0) {
		DIR dir = playerDirections.back();
		switch (dir) {
		case NORTH:
			playerRotation = 180;
			moveUnit(player, 0, -1);
			break;
		case WEST:
			playerRotation = 90;
			moveUnit(player, -1, 0);
			break;
		case SOUTH:
			playerRotation = 0;
			moveUnit(player, 0, 1);
			break;
		case EAST:
			playerRotation = 270;
			moveUnit(player, 1, 0);
			break;
		}
	}
}

void updatePositions() {
	// --- Change player position ---
	if (player.isMoving) {
		updateUnitPosition(player);
	}
	// --- Change drone position ---
	int size = map.nrDrones;
	for (int i = 0; i < size; i++) {
		Drone& drone = map.drones[i];
		if (drone.isMoving) {
			updateUnitPosition(drone);
		}
	}
	// --- Change Patrol position ---
	size = map.nrPatrols;
	for (int i = 0; i < size; i++) {
		Patrol& patrol = map.patrols[i];
		patrol.steps++;
		patrol.w = ((double)(patrol.steps)) / nrMovementSteps;
		// Update Patrol Direction
		double dx = abs(patrol.cx - patrol.getX());
		double dy = abs(patrol.cy - patrol.getY());
		if (dx >= patrolOffset && dy >= patrolOffset) {
			patrol.changeDirection();
		}
	}
	// --- Change Projectiles position ---
	size = (int)(map.projectiles.size());
	int pi = 0;
	while (pi < size) {
		Projectile& projectile = map.projectiles.at(pi);
		projectile.steps++;
		projectile.w = ((double)(projectile.steps)) / nrMovementSteps;
		// Check border
		if (projectile.getX() <= -1 || projectile.getY() <= -1 || projectile.getX() >= map.width || projectile.getY() >= map.height) {
			map.projectiles.erase(map.projectiles.begin() + pi);
			size--;
		}
		else {
			pi++;
		}
	}
}

// === Status Bar Values ===

string getHealthStatus() {
	return to_string(player.health) + "/" + to_string(playerMaxHealth);
}

string getLevelStatus() {
	return to_string(level) + "/" + to_string(maxLevel);
}

string getItemStatus() {
	return to_string(map.nrCollectedKeyItems) + "/" + to_string(map.nrKeyItems);
}

// === Rendering ===

bool inView(double vx, double vy, RenderObject renderObject) {
	if (renderObject == ATTACK_AREA) {
		return (vx > viewMinXA&& vy > viewMinYA&& vx < viewMaxX && vy < viewMaxY);
	}
	else if (renderObject == LIGHT_SOURCE) {
		return (vx > viewMinXL&& vy > viewMinYL&& vx < viewMaxXL && vy < viewMaxYL);
	}
	else {
		return (vx > viewMinX&& vy > viewMinY&& vx < viewMaxX && vy < viewMaxY);
	}
}

void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, int w, int h) {
	SDL_FRect dstrect = { x, y, (float)w, (float)h };
	SDL_RenderCopyF(renderer, texture, NULL, &dstrect);
}

void renderGameView() {
	// View
	double cx = playerOffsetX - player.getX();
	double cy = playerOffsetY - player.getY();
	// Current Position
	double vx = 0;
	double vy = 0;
	float fx = 0;
	float fy = 0;
	// Map
	float mapX = (float)(cx * blockSizeInPixel);
	float mapY = (float)(cy * blockSizeInPixel);
	renderTexture(renderer, mapTexture, mapX, mapY, mapWidthInPixel, mapHeightInPixel);
	// Key Items
	int size = (int)(map.keyItems.size());
	for (int i = 0; i < size; i++) {
		Point& p = map.keyItems.at(i);
		vx = cx + p.x + itemOffset;
		vy = cy + p.y + itemOffset;
		if (inView(vx, vy, BLOCK)) {
			fx = (float)(vx * blockSizeInPixel);
			fy = (float)(vy * blockSizeInPixel);
			renderTexture(renderer, keyItemBlockTexture, fx, fy, itemSizeInPixel, itemSizeInPixel);
		}
	}
	// Bonus Items
	size = (int)(map.bonusItems.size());
	for (int i = 0; i < size; i++) {
		BonusItem& b = map.bonusItems.at(i);
		vx = cx + b.x + itemOffset;
		vy = cy + b.y + itemOffset;
		if (inView(vx, vy, BLOCK)) {
			int BonusIdx = b.value - 1;
			fx = (float)(vx * blockSizeInPixel);
			fy = (float)(vy * blockSizeInPixel);
			renderTexture(renderer, bonusItemBlockTexture[BonusIdx], fx, fy, itemSizeInPixel, itemSizeInPixel);
		}
	}
	// Exit
	if (map.isExitOpen) {
		vx = cx + map.exitX;
		vy = cy + map.exitY;
		if (inView(vx, vy, BLOCK)) {
			fx = (float)(vx * blockSizeInPixel);
			fy = (float)(vy * blockSizeInPixel);
			renderTexture(renderer, exitBlockTexture, fx, fy, blockSizeInPixel, blockSizeInPixel);
		}
	}
	// Drone Attack Areas
	size = map.nrDrones;
	if (map.droneAttack) {
		for (int i = 0; i < size; i++) {
			Drone& d = map.drones[i];
			vx = cx + d.getX() - attackAreaRadius;
			vy = cy + d.getY() - attackAreaRadius;
			if (inView(vx, vy, ATTACK_AREA)) {
				fx = (float)(vx * blockSizeInPixel);
				fy = (float)(vy * blockSizeInPixel);
				renderTexture(renderer, droneAttackAreaTexture, fx, fy, attackAreaSizeInPixel, attackAreaSizeInPixel);
			}
		}
	}
	// Switch Attack Areas
	size = map.nrSwitches;
	for (int i = 0; i < size; i++) {
		Switch& s = map.switches[i];
		vx = cx + s.getX() - attackAreaRadius;
		vy = cy + s.getY() - attackAreaRadius;
		if (inView(vx, vy, ATTACK_AREA)) {
			fx = (float)(vx * blockSizeInPixel);
			fy = (float)(vy * blockSizeInPixel);
			renderTexture(renderer, switchAttackAreaTexture[map.switchState], fx, fy, attackAreaSizeInPixel, attackAreaSizeInPixel);
		}
	}
	// Drones
	size = map.nrDrones;
	for (int i = 0; i < size; i++) {
		Drone& d = map.drones[i];
		vx = cx + d.getX();
		vy = cy + d.getY();
		if (inView(vx, vy, BLOCK)) {
			fx = (float)(vx * blockSizeInPixel);
			fy = (float)(vy * blockSizeInPixel);
			renderTexture(renderer, droneBlockTexture, fx, fy, blockSizeInPixel, blockSizeInPixel);
		}
	}
	// Switches
	size = map.nrSwitches;
	for (int i = 0; i < size; i++) {
		Switch& s = map.switches[i];
		vx = cx + s.getX();
		vy = cy + s.getY();
		if (inView(vx, vy, BLOCK)) {
			fx = (float)(vx * blockSizeInPixel);
			fy = (float)(vy * blockSizeInPixel);
			renderTexture(renderer, switchBlockTexture, fx, fy, blockSizeInPixel, blockSizeInPixel);
		}
	}
	// Patrols
	size = map.nrPatrols;
	for (int i = 0; i < size; i++) {
		Patrol& p = map.patrols[i];
		vx = cx + p.getX() + projectileRadius;
		vy = cy + p.getY() + projectileRadius;
		if (inView(vx, vy, BLOCK)) {
			fx = (float)(vx * blockSizeInPixel);
			fy = (float)(vy * blockSizeInPixel);
			renderTexture(renderer, patrolBlockTexture, fx, fy, projectileSizeInPixel, projectileSizeInPixel);
		}
	}
	// Bosses
	size = map.nrBosses;
	if (map.isBossMap && map.nrBosses > 0) {
		for (int i = 0; i < size; i++) {
			Boss& b = map.bosses[i];
			vx = cx + b.getX();
			vy = cy + b.getY();
			if (inView(vx, vy, BLOCK)) {
				fx = (float)(vx * blockSizeInPixel);
				fy = (float)(vy * blockSizeInPixel);
				renderTexture(renderer, bossBlockTexture, fx, fy, blockSizeInPixel, blockSizeInPixel);
			}
		}
	}
	// Projectiles
	size = (int)(map.projectiles.size());
	for (int i = 0; i < size; i++) {
		Projectile& p = map.projectiles.at(i);
		vx = cx + p.getX() + projectileRadius;
		vy = cy + p.getY() + projectileRadius;
		if (inView(vx, vy, BLOCK)) {
			fx = (float)(vx * blockSizeInPixel);
			fy = (float)(vy * blockSizeInPixel);
			renderTexture(renderer, projBlockTexture, fx, fy, projectileSizeInPixel, projectileSizeInPixel);
		}
	}
	// Shooters
	size = map.nrShooters;
	for (int i = 0; i < size; i++) {
		Shooter& s = map.shooters[i];
		vx = cx + s.getX();
		vy = cy + s.getY();
		if (inView(vx, vy, BLOCK)) {
			fx = (float)(vx * blockSizeInPixel);
			fy = (float)(vy * blockSizeInPixel);
			renderTexture(renderer, shooterBlockTexture, fx, fy, blockSizeInPixel, blockSizeInPixel);
		}
	}
	// Rotate and render player
	SDL_RenderCopyEx(renderer, playerBlockTexture, NULL, &playerPos, playerRotation, NULL, SDL_FLIP_NONE);
	// Draw Lights
	renderLights(cx, cy);
	// Status Bar
	SDL_RenderCopy(renderer, statusBarTexture, NULL, &statusBarPos);
	// Texts
	healthText->showText();
	levelText->showText();
	keyItemsText->showText();
	pointsText->showText();
	highScoreText->showText();
}

void renderLights(double cx, double cy) {
	// Black Screen
	SDL_RenderCopy(renderer, blackScreenTexture, NULL, NULL);
	// Current Position
	double vx = 0;
	double vy = 0;
	float fx = 0;
	float fy = 0;
	// Key Items
	int size = (int)(map.keyItems.size());
	if (size > 0) {
		SDL_SetTextureColorMod(lightTexture[1], courseColorsBright[course].r, courseColorsBright[course].g, courseColorsBright[course].b);
		for (int i = 0; i < size; i++) {
			Point& p = map.keyItems.at(i);
			vx = cx + p.x + itemOffset;
			vy = cy + p.y + itemOffset;
			if (inView(vx, vy, LIGHT_SOURCE)) {
				fx = (float)((vx + lightRenderOffset[1]) * blockSizeInPixel);
				fy = (float)((vy + lightRenderOffset[1]) * blockSizeInPixel);
				renderTexture(renderer, lightTexture[1], fx, fy, lightTextureSizes[1][0], lightTextureSizes[1][1]);
			}
		}
	}
	// Bonus Items
	size = (int)(map.bonusItems.size());
	if (size > 0) {
		SDL_SetTextureColorMod(lightTexture[1], redColor.r, redColor.g, redColor.b);
		for (int i = 0; i < size; i++) {
			BonusItem& b = map.bonusItems.at(i);
			vx = cx + b.x + itemOffset;
			vy = cy + b.y + itemOffset;
			if (inView(vx, vy, LIGHT_SOURCE)) {
				fx = (float)((vx + lightRenderOffset[1]) * blockSizeInPixel);
				fy = (float)((vy + lightRenderOffset[1]) * blockSizeInPixel);
				renderTexture(renderer, lightTexture[1], fx, fy, lightTextureSizes[1][0], lightTextureSizes[1][1]);
			}
		}
	}
	// Exit
	if (map.isExitOpen) {
		SDL_SetTextureColorMod(lightTexture[2], courseColorsBright[course].r, courseColorsBright[course].g, courseColorsBright[course].b);
		vx = cx + map.exitX;
		vy = cy + map.exitY;
		if (inView(vx, vy, LIGHT_SOURCE)) {
			fx = (float)((vx + lightRenderOffset[2]) * blockSizeInPixel);
			fy = (float)((vy + lightRenderOffset[2]) * blockSizeInPixel);
			renderTexture(renderer, lightTexture[2], fx, fy, lightTextureSizes[2][0], lightTextureSizes[2][1]);
		}
	}
	// Drone Attack Areas
	size = map.nrDrones;
	if (map.droneAttack) {
		for (int i = 0; i < size; i++) {
			Drone& d = map.drones[i];
			vx = cx + d.getX() - attackAreaRadius;
			vy = cy + d.getY() - attackAreaRadius;
			if (inView(vx, vy, LIGHT_SOURCE)) {
				fx = (float)((vx + lightRenderOffset[5]) * blockSizeInPixel);
				fy = (float)((vy + lightRenderOffset[5]) * blockSizeInPixel);
				renderTexture(renderer, lightTexture[5], fx, fy, lightTextureSizes[5][0], lightTextureSizes[5][1]);
			}
		}
	}
	// Switch Attack Areas
	size = map.nrSwitches;
	for (int i = 0; i < size; i++) {
		Switch& s = map.switches[i];
		vx = cx + s.getX() - attackAreaRadius;
		vy = cy + s.getY() - attackAreaRadius;
		if (inView(vx, vy, LIGHT_SOURCE)) {
			fx = (float)((vx + lightRenderOffset[3 + map.switchState]) * blockSizeInPixel);
			fy = (float)((vy + lightRenderOffset[3 + map.switchState]) * blockSizeInPixel);
			renderTexture(renderer, lightTexture[3 + map.switchState], fx, fy, lightTextureSizes[3 + map.switchState][0], lightTextureSizes[3 + map.switchState][1]);
		}
	}
	// Drones
	size = map.nrDrones;
	if (map.isDroneRed) {
		SDL_SetTextureColorMod(lightTexture[2], redColor.r, redColor.g, redColor.b);
		for (int i = 0; i < size; i++) {
			Drone& d = map.drones[i];
			vx = cx + d.getX();
			vy = cy + d.getY();
			if (inView(vx, vy, LIGHT_SOURCE)) {
				fx = (float)((vx + lightRenderOffset[2]) * blockSizeInPixel);
				fy = (float)((vy + lightRenderOffset[2]) * blockSizeInPixel);
				renderTexture(renderer, lightTexture[2], fx, fy, lightTextureSizes[2][0], lightTextureSizes[2][1]);
			}
		}
	}
	// Patrols
	size = map.nrPatrols;
	if (size > 0) {
		SDL_SetTextureColorMod(lightTexture[0], patrolLightColor.r, patrolLightColor.g, patrolLightColor.b);
		for (int i = 0; i < size; i++) {
			Patrol& p = map.patrols[i];
			vx = cx + p.getX() + projectileRadius;
			vy = cy + p.getY() + projectileRadius;
			if (inView(vx, vy, LIGHT_SOURCE)) {
				fx = (float)((vx + lightRenderOffset[0]) * blockSizeInPixel);
				fy = (float)((vy + lightRenderOffset[0]) * blockSizeInPixel);
				renderTexture(renderer, lightTexture[0], fx, fy, lightTextureSizes[0][0], lightTextureSizes[0][1]);
			}
		}
	}
	// Projectiles
	size = (int)(map.projectiles.size());
	if (size > 0) {
		SDL_SetTextureColorMod(lightTexture[0], projectileLightColor.r, projectileLightColor.g, projectileLightColor.b);
		for (int i = 0; i < size; i++) {
			Projectile& p = map.projectiles.at(i);
			vx = cx + p.getX() + projectileRadius;
			vy = cy + p.getY() + projectileRadius;
			if (inView(vx, vy, LIGHT_SOURCE)) {
				fx = (float)((vx + lightRenderOffset[0]) * blockSizeInPixel);
				fy = (float)((vy + lightRenderOffset[0]) * blockSizeInPixel);
				renderTexture(renderer, lightTexture[0], fx, fy, lightTextureSizes[0][0], lightTextureSizes[0][1]);
			}
		}
	}
}

void renderTitleScreenLights() {
	for (int i = 0; i < titleScreenNumLights; i++) {
		// Set Color
		Uint8 r = 0;
		if (currTitleScreenLightColor.r > 0) { r = (Uint8)(titleScreenLightsVal[i] * 255); }
		Uint8 g = 0;
		if (currTitleScreenLightColor.g > 0) { g = (Uint8)(titleScreenLightsVal[i] * 255); }
		Uint8 b = 0;
		if (currTitleScreenLightColor.b > 0) { b = (Uint8)(titleScreenLightsVal[i] * 255); }
		SDL_SetTextureColorMod(lightTexture[6], r, g, b);
		// Render Light
		SDL_Rect currPos = { titleScreenLightsPos[i][0], titleScreenLightsPos[i][1], lightTextureSizes[6][0], lightTextureSizes[6][1] };
		SDL_RenderCopy(renderer, lightTexture[6], NULL, &currPos);
		// Update Brightness
		titleScreenLightsVal[i] += (lightChangeStepSize * titleScreenLightsVec[i]);
		bool changeDirect = false;
		if (titleScreenLightsVal[i] <= 0.0) {
			titleScreenLightsVal[i] = 0.0;
			changeDirect = true;
		}
		else if (titleScreenLightsVal[i] >= 1.0) {
			titleScreenLightsVal[i] = 1.0;
			changeDirect = true;
		}
		// Change Direction
		if (changeDirect) {
			titleScreenLightsVec[i] *= -1;
		}
	}
}

void renderFinishedScreenText() {
	// Show End Screen
	SDL_RenderCopy(renderer, titleScreenTexture, NULL, NULL);
	renderTitleScreenLights();
	// Status Bar
	SDL_RenderCopy(renderer, statusBarTexture, NULL, &statusBarPos);
	// Texts
	gameStatusText[0]->showText();
	gameStatusText[1]->showText();
	healthText->showText();
	levelText->showText();
	keyItemsText->showText();
	pointsText->showText();
	highScoreText->showText();
}