#pragma once

#include <SDL.h>
#include <string>

// Colors
const SDL_Color whiteColor = { 255, 255, 255, 255 };
const SDL_Color lightGreyColor = { 191, 191, 191, 255 };
const SDL_Color greyColor = { 128, 128, 128, 255 };
const SDL_Color darkGrayColor = { 64, 64, 64, 255 };
const SDL_Color blackColor = { 0, 0, 0, 255 };
const SDL_Color redColor = { 255, 0, 0, 255 };
const SDL_Color greenColor = { 0, 255, 0, 255 };
const SDL_Color blueColor = { 0, 0, 255, 255 };
const SDL_Color attackAreaColor = { 255, 191, 0, 191 };
const SDL_Color emptyColor = { 0, 0, 0, 0 };
const SDL_Color statusColor = blackColor;
const SDL_Color statusTextColor = whiteColor;
const SDL_Color statusBarColor = lightGreyColor;
const SDL_Color droneIdleColor = greyColor;
const SDL_Color blackScreenColor = { 0, 0, 0, 64 };
const SDL_Color playerTimeOutColor = { 255, 255, 255, 128 };
const SDL_Color courseColorsDark[] = { SDL_Color{ 0, 191, 191, 255 }, SDL_Color{ 0, 191, 0, 255 }, SDL_Color{ 191, 191, 0, 255 } };
const SDL_Color courseColorsBright[] = { SDL_Color{ 0, 255, 255, 255 }, SDL_Color{ 0, 255, 0, 255 }, SDL_Color{ 255, 255, 0, 255 } };
const SDL_Color projectileLightColor = { 0, 255, 255, 255 };
const SDL_Color patrolLightColor = { 255, 0, 255, 255 };
const SDL_Color attackAreaLightColor = { 255, 191, 0, 255 };
const SDL_Color backgroundColor[] = { whiteColor, lightGreyColor, greyColor, darkGrayColor };
// Strings
const std::string windowName = "Maze";
const std::string saveFileName = "Maze.save";
const std::string beforeStartText = "SELECT A COURSE";
const std::string exitText[2] = { "[Esc]","Exit" };
const std::string infoText[4] = { "[F1]","Manual", "[F2]", "Credits" };
const std::string pauseText[2] = { "PAUSED\n[Space]\n[Esc]", "\nResume\nChange Course" };
const std::string gameOverText[2] = { "GAME OVER\n[Space]\n[Esc]", "\nRestart\nChange Course" };
const std::string finishedText[2] = { "YOU WIN\n[Space]\n[Esc]", "\nRestart\nChange Course" };
const std::string selectionItemTexts[3] = { "Easy" , "Normal" , "Hard" };
const std::string informationText[6] = { "\n\nIn-Game Controls:\n\n\n\nGoal for each level:\n\n\n\nPoints:",
											"[Esc]\n\n\n[W,A,S,D]\n[Esc]\n\n\nCollect all keys and go to the exit.\n"
											"Avoid the traps on the way.\n\n\nYou will get points if you collect a key, enter an exit or find a bonus item. "
											"The maximum amount of points for each course is 400.",
											"Back\n\n\nMove\nPause",
											"\n\nMade by:\n\n\n\nThird-party libraries:",
											"[Esc]\n\n\nJens Krenzin\n(jens_krenzin@t-online.de)\n\n\nSDL\nSDL_image\nSDL_ttf\nFreeType\nlibpng\nzlib",
											"Back" };
// Map
const bool freeVal = true;
const bool wallVal = false;
const Uint32 initVal = 0;
const int maxMapSize = 33;
const int maxNumFields = maxMapSize * maxMapSize;
const int mapArraySize = (maxNumFields + 31) / 32;
// Game Field
const int mazeCellSize = 3;
const int blockSizeInPixel = 32;
const int gameFieldSize = 19;
const int gameFieldSizeInPixel = gameFieldSize * blockSizeInPixel;
// Status Bar
const int statusHeight = 1;
const int statusWidth = gameFieldSize;
const int statusHeightInPixel = statusHeight * blockSizeInPixel;
const int statusWidthInPixel = statusWidth * blockSizeInPixel;
// Player Offset
const int playerOffsetX = gameFieldSize / 2;
const int playerOffsetY = gameFieldSize / 2 + statusHeight;
const int playerOffsetXInPixel = playerOffsetX * blockSizeInPixel;
const int playerOffsetYInPixel = playerOffsetY * blockSizeInPixel;
const SDL_Rect playerPos = { playerOffsetXInPixel, playerOffsetYInPixel, blockSizeInPixel, blockSizeInPixel };
// Face
const int faceOffsetXInPixel = 7;
const int faceOffsetYInPixel = 24;
const int faceWidthInPixel = 18;
const int faceHeightInPixel = 8;
// Window
const int windowWidth = gameFieldSize;
const int windowHeight = gameFieldSize + statusHeight;
const int windowWidthInPixel = windowWidth * blockSizeInPixel;
const int windowHeightInPixel = windowHeight * blockSizeInPixel;
// Status Text
const int healthTextOffsetInPixel = 32;
const int levelTextOffsetInPixel = 154;
const int keyItemsTextOffsetInPixel = 275;
const int pointsTextOffsetInPixel = 397;
const int highScoreTextOffsetInPixel = 518;
const int statusTextWidth = 3;
const int statusValueOffsetX = blockSizeInPixel / 4;
const int statusValueOffsetY = 6;
const int statusFontSize = 24;
const int statusTextFontSize = 30;
const int statusOffsetY = windowHeight / 4;
const int statusOffsetYInPixel = statusOffsetY * blockSizeInPixel;
const SDL_Rect statusBarPos = { 0, 0, statusWidthInPixel, statusHeightInPixel };
// Courses
const int numCourses = 3;
// Selection Text
const int numTextsPerCourse = 4;
const int numSelectionTexts = numCourses * numTextsPerCourse;
const int selectionTextOffsetX = 4;
const int selectionTextOffsetY = 9;
const int selectionTitleOffsetY = selectionTextOffsetY - 5;
const int selectionInfoTextOffsetY = selectionTextOffsetY + 4;
const int selectionExitTextOffsetY = selectionInfoTextOffsetY + 2;
const int selectionInformationTextOffsetX = 2;
const int selectionInformationTextOffsetY = 2;
const int selectionItemTextWidth = windowWidth / 2;
const int selectionOffsetA = 3;
const int selectionOffsetB = 7;
const int selectionOffsetC = 9;
const int selectionOffsetD = 4;
const int selectionOffsetE = 6;
const SDL_Rect selectionHighScorePos = { (selectionTextOffsetX + selectionOffsetB)*blockSizeInPixel - 5, (selectionTextOffsetY - 2)*blockSizeInPixel + blockSizeInPixel / 2 + 1, blockSizeInPixel - 2, blockSizeInPixel - 2 };
// Enemy types
const int numEnemyTypes = 4;
// Max Level
const int maxLevel = 12;
// Maze Sizes
const int mazeWidths[maxLevel - 1] = { 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7 };
const int mazeHeights[maxLevel - 1] = { 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7 };
// Enemy / Bonus ratio
const double enemyRatios[numCourses] = { 2.0 / 3.0, 1.0, 1.0 };
// Player health
const int playerMaxHealth = 3;
// Key Item Value
const int keyItemValues[numCourses] = { 1, 4, 4 };
// Max Points
const int maxPoints = 400;
// Enemy offset
const int enemyOffset = mazeCellSize / 2;
// Projectiles
const double projectileRadius = 0.25;
const double projectileRadiusInPixel = blockSizeInPixel / 4;
const int projectileSizeInPixel = blockSizeInPixel / 2;
// Attack Area
const int attackAreaRadius = 1;
const int attackAreaSize = attackAreaRadius * 2 + 1;
const int attackAreaSizeInPixel = blockSizeInPixel * attackAreaSize;
// Items
const double itemOffset = 0.25F;
const int itemSizeInPixel = blockSizeInPixel / 2;
// Drone Range
const int droneRange = 1;
// Game Over check
const double attackAreaGameOverThresholdSquare = attackAreaRadius + 0.5;
const double attackAreaGameOverThresholdLine = 0.5;
const double projectileGameOverThreshold = 0.5;
// Patrol
const int patrolOffset = (mazeCellSize / 2 + 1);
// Movement
const int maxPlayerDirections = 4;
const double movementSpeed = 7.5;
const double alphaSpeed = 2.5;
const double lightChangeSpeed = 0.1;
const int lightChangeSpeedLevels = 3;
// Game Step Status
const int gameStepStatusA = 1;
// Blink
const int nrBlinks = 4;
// Boss
const int nrBossesStart = 4;
const int bossCenterOffset = 1;
const int bossMapCells = 7;
const int bossMapSize = maxMapSize;
const int bossNrKeyItems = 12;
const int bossNrBonusItems[] = { 32, 12, 12 };
const int bossBonusItemOffset = 14;
// Light
const int lightValue = 64;
const int numLightTypes = 7;
const int lightRadius = 2;
const int titleScreenLightRadius = 3;
const int titleScreenNumLightsInRow = gameFieldSize / titleScreenLightRadius + 1;
const int titleScreenNumLights = titleScreenNumLightsInRow * titleScreenNumLightsInRow;
const int lightRadiusInPixel = blockSizeInPixel * lightRadius;
const int titleScreenLightRadiusInPixel = blockSizeInPixel * titleScreenLightRadius;
const int lightAreaSizes[numLightTypes][2] = {	{ 0, 0 },
												{ itemSizeInPixel,itemSizeInPixel },
												{ blockSizeInPixel, blockSizeInPixel },
												{ attackAreaSizeInPixel, blockSizeInPixel },
												{ blockSizeInPixel, attackAreaSizeInPixel },
												{ attackAreaSizeInPixel, attackAreaSizeInPixel },
												{ 0, 0 } };
const int lightTextureSizes[numLightTypes][2] = {	{ lightRadiusInPixel * 2, lightRadiusInPixel * 2 },
													{ lightRadiusInPixel * 2 + itemSizeInPixel, lightRadiusInPixel * 2 + itemSizeInPixel },
													{ lightRadiusInPixel * 2 + blockSizeInPixel, lightRadiusInPixel * 2 + blockSizeInPixel },
													{ lightRadiusInPixel * 2 + attackAreaSizeInPixel, lightRadiusInPixel * 2 + attackAreaSizeInPixel },
													{ lightRadiusInPixel * 2 + attackAreaSizeInPixel, lightRadiusInPixel * 2 + attackAreaSizeInPixel },
													{ lightRadiusInPixel * 2 + attackAreaSizeInPixel, lightRadiusInPixel * 2 + attackAreaSizeInPixel },
													{ titleScreenLightRadiusInPixel * 2, titleScreenLightRadiusInPixel * 2 } };
const int lightTextureOffsets[numLightTypes][2] = { { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, blockSizeInPixel },{ blockSizeInPixel, 0 },{ 0, 0 },{ 0, 0 } };
const double lightRenderOffset[numLightTypes - 1] = { -projectileRadius - lightRadius + 0.5, -lightRadius, -lightRadius, -lightRadius, -lightRadius, -lightRadius };
// In View Borders
const double viewMinX = -1;
const double viewMinY = -1 + statusHeight;
const double viewMinXA = viewMinX - attackAreaRadius * 2;
const double viewMinYA = viewMinY - attackAreaRadius * 2;
const double viewMinXL = viewMinX - lightRadius * 2 - attackAreaSize;
const double viewMinYL = viewMinY - lightRadius * 2 - attackAreaSize;
const double viewMaxX = gameFieldSize;
const double viewMaxY = gameFieldSize + statusHeight;
const double viewMaxXL = viewMaxX + lightRadius;
const double viewMaxYL = viewMaxY + lightRadius;