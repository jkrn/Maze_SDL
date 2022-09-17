#include <vector>
#include <algorithm>
#include "Map.hpp"
#include "Maze.hpp"
#include "Direct.hpp"

Map::Map() : 
	mazeWidth(0),
	mazeHeight(0),
	width(0),
	height(0),
	nrKeyItems(0),
	nrBonusItems(0),
	nrDrones(0),
	nrShooters(0),
	nrSwitches(0),
	nrPatrols(0),
	startX(0),
	startY(0),
	exitX(0),
	exitY(0),
	nrCollectedKeyItems(0),
	nrBosses(0),
	switchState(0),
	prevSwitchState(0),
	isExitOpen(false),
	isDroneRed(false),
	droneAttack(false),
	isBossMap(false)
{
	std::fill(field, field + mapArraySize, initVal);
}

Map::Map(int course) :
	mazeWidth(bossMapCells),
	mazeHeight(bossMapCells),
	width(bossMapSize),
	height(bossMapSize),
	nrKeyItems(bossNrKeyItems),
	nrBonusItems(bossNrBonusItems[course]),
	nrDrones(0),
	nrShooters(0),
	nrSwitches(0),
	nrPatrols(0),
	nrCollectedKeyItems(0),
	nrBosses(nrBossesStart),
	switchState(0),
	prevSwitchState(0),
	isExitOpen(false),
	isDroneRed(false),
	droneAttack(false),
	isBossMap(true)
{
	std::fill(field, field + mapArraySize, initVal);
	buildBossMap(course);
}

Map::Map(int mazeWidth, int mazeHeight, int nrKeyItems, int nrBonusItems, int nrDrones, int nrShooters, int nrSwitches, int nrPatrols, int bonusValue) :
	mazeWidth(mazeWidth),
	mazeHeight(mazeHeight),
	width(mazeWidth*(mazeCellSize + 1) + 1),
	height(mazeHeight*(mazeCellSize + 1) + 1),
	nrKeyItems(nrKeyItems),
	nrBonusItems(nrBonusItems),
	nrDrones(nrDrones),
	nrShooters(nrShooters),
	nrSwitches(nrSwitches),
	nrPatrols(nrPatrols),
	nrCollectedKeyItems(0),
	nrBosses(0),
	switchState(0),
	prevSwitchState(0),
	isExitOpen(false),
	isDroneRed(false),
	droneAttack(false),
	isBossMap(false)
{
	std::fill(field, field + mapArraySize, initVal);
	buildMazeMap(bonusValue);
}

Map::~Map() {}

void Map::removeBosses() {
	// Remove boss
	nrBosses = 0;
	// Open Center
	set(width / 2, height / 2, freeVal);
}

bool Map::operator()(int x, int y) {
	int pos = width * y + x;
	int fieldIdx = pos / 32;
	Uint32 mask = (1 << ( pos % 32 ));
	return ((field[fieldIdx] & mask) == mask);
}

void Map::set(int x, int y, bool value) {
	int pos = width * y + x;
	int fieldIdx = pos / 32;
	if (value) {
		Uint32 mask = (1 << (pos % 32));
		field[fieldIdx] = (field[fieldIdx] | mask);
	}
	else {
		Uint32 mask = ~(1 << (pos % 32));
		field[fieldIdx] = (field[fieldIdx] & mask);
	}
}

void Map::buildBossMap(int course) {
	// Center
	int cx = width / 2;
	int cy = height / 2;
	// Directions
	int dx[4] = { -1,-1, 1, 1 };
	int dy[4] = { -1, 1,-1, 1 };
	// Line offset
	int lineOffset = mazeCellSize + 1;
	int bonusItemsBound = lineOffset * 2;
	int keyItemsBound = lineOffset * 3;
	// Random Generator
	RandomGenerator rndX(0, 1);
	RandomGenerator rndY(0, 1);
	// Start
	startX = 2 + ((int)width - 5)*rndX.getNextValue();
	startY = 2 + ((int)height - 5)*rndY.getNextValue();
	// Exit
	exitX = (int)cx;
	exitY = (int)cy;
	// Border
	int border = width / 2;
	// Field
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int vx = abs(i - cx);
			int vy = abs(j - cy);
			int v = std::max(vx, vy);
			if ((vx == bossCenterOffset && vy == bossCenterOffset) || (v % lineOffset == 0 && ((v == 0) || (vx > 1 && vy > 1) || v == border))) {
				set(i, j, wallVal);
			}
			else {
				set(i, j, freeVal);
			}
		}
	}
	// Projectiles
	projectiles = std::vector<Projectile>();
	// Boss
	bosses = std::vector<Boss>();
	for (int d = 0; d < 4; d++) {
		bosses.push_back(Boss(cx + dx[d] * bossCenterOffset, cy + dy[d] * bossCenterOffset));
	}
	// Key Items
	keyItems = std::vector<Point>();
	for (int k = lineOffset; k <= keyItemsBound; k = k + lineOffset) {
		keyItems.push_back(Point(cx - k, cy));
		keyItems.push_back(Point(cx + k, cy));
		keyItems.push_back(Point(cx, cy - k));
		keyItems.push_back(Point(cx, cy + k));
	}
	// Bonus Items
	bonusItems = std::vector<BonusItem>();
	for (int k = lineOffset; k <= bonusItemsBound; k = k + lineOffset) {
		for (int d = 0; d < 4; d++) {
			// Course 0
			if (course == 0) {
				// Value 5
				bonusItems.push_back(BonusItem(cx + dx[d] * (bossBonusItemOffset - k), cy + dy[d] * (bossBonusItemOffset - k), 5));
				// Value 4
				bonusItems.push_back(BonusItem(cx + dx[d] * (bossBonusItemOffset - k) - dx[d] * 2, cy + dy[d] * (bossBonusItemOffset - k), 4));
				bonusItems.push_back(BonusItem(cx + dx[d] * (bossBonusItemOffset - k), (cy + dy[d] * (bossBonusItemOffset - k) - dy[d] * 2), 4));
				// Value 3
				if (k == lineOffset) {
					bonusItems.push_back(BonusItem(cx + dx[d] * (bossBonusItemOffset - k) - dx[d] * 4, cy + dy[d] * (bossBonusItemOffset - k), 3));
					bonusItems.push_back(BonusItem(cx + dx[d] * (bossBonusItemOffset - k), cy + dy[d] * (bossBonusItemOffset - k) - dy[d] * 4, 3));
				}
			}
			// Course 1, 2
			else {
				if (k == lineOffset) {
					// Value 4
					bonusItems.push_back(BonusItem(cx + dx[d] * (bossBonusItemOffset - k), cy + dy[d] * (bossBonusItemOffset - k), 4));
					// Value 3
					bonusItems.push_back(BonusItem(cx + dx[d] * (bossBonusItemOffset - k) - dx[d] * 2, cy + dy[d] * (bossBonusItemOffset - k), 3));
					bonusItems.push_back(BonusItem(cx + dx[d] * (bossBonusItemOffset - k), cy + dy[d] * (bossBonusItemOffset - k) - dy[d] * 2, 3));
				}
			}
		}
	}
}

void Map::buildMazeMap(int bonusValue) {
	// Create maze
	Maze maze(mazeWidth, mazeHeight);
	// Convert to maze to map
	std::vector<Point> freePoints, A, B;
	for (int x = 0; x < mazeWidth; x++) {
		for (int y = 0; y < mazeHeight; y++) {
			int cx = x * (mazeCellSize + 1) + 1;
			int cy = y * (mazeCellSize + 1) + 1;
			// A, B
			if (maze.isDeadEnd(x, y)) {
				A.push_back(Point(cx, cy));
			}
			else {
				B.push_back(Point(cx, cy));
			}
			// Free cells
			for (int dx = 0; dx < mazeCellSize; dx++) {
				for (int dy = 0; dy < mazeCellSize; dy++) {
					set(cx + dx, cy + dy, freeVal);
				}
			}
			// Free Walls
			if ((maze.maze[x][y] & SOUTH_VAL) == SOUTH_VAL) {
				for (int dx = 0; dx < mazeCellSize; dx++) {
					set(cx + dx, cy + mazeCellSize, freeVal);
				}
			}
			if ((maze.maze[x][y] & EAST_VAL) == EAST_VAL) {
				for (int dy = 0; dy < mazeCellSize; dy++) {
					set(cx + mazeCellSize, cy + dy, freeVal);
				}
			}
		}
	}
	// Free Points
	std::random_shuffle(std::begin(A), std::end(A));
	std::random_shuffle(std::begin(B), std::end(B));
	freePoints.reserve(A.size() + B.size());
	freePoints.insert(freePoints.end(), B.begin(), B.end());
	freePoints.insert(freePoints.end(), A.begin(), A.end());
	// Random Generators
	RandomGenerator rndX(0, 2);
	RandomGenerator rndY(0, 2);
	RandomGenerator rndD(0, 1);
	// Set start
	startX = freePoints.back().x + rndX.getNextValue();
	startY = freePoints.back().y + rndY.getNextValue();
	freePoints.pop_back();
	// Set exit
	exitX = freePoints.back().x + rndX.getNextValue();
	exitY = freePoints.back().y + rndY.getNextValue();
	freePoints.pop_back();
	// Set key items
	keyItems = std::vector<Point>();
	for (int i = 0; i < nrKeyItems; i++) {
		keyItems.push_back(Point(freePoints.back().x + rndX.getNextValue(), freePoints.back().y + rndY.getNextValue()));
		freePoints.pop_back();
	}
	// Set bonus items
	bonusItems = std::vector<BonusItem>();
	for (int i = 0; i < nrBonusItems; i++) {
		bonusItems.push_back(BonusItem(freePoints.back().x + rndX.getNextValue(), freePoints.back().y + rndY.getNextValue(), bonusValue));
		freePoints.pop_back();
	}
	// Set drones
	drones = std::vector<Drone>();
	for (int i = 0; i < nrDrones; i++) {
		drones.push_back(Drone(freePoints.back().x + enemyOffset, freePoints.back().y + enemyOffset));
		freePoints.pop_back();
	}
	// Set shooters
	shooters = std::vector<Shooter>();
	for (int i = 0; i < nrShooters; i++) {
		int x = freePoints.back().x + enemyOffset;
		int y = freePoints.back().y + enemyOffset;
		shooters.push_back(Shooter(x, y));
		freePoints.pop_back();
		// Insert wall at shooter
		set(x,y,wallVal);
	}
	// Set switches
	switches = std::vector<Switch>();
	for (int i = 0; i < nrSwitches; i++) {
		int x = freePoints.back().x + enemyOffset;
		int y = freePoints.back().y + enemyOffset;
		switches.push_back(Switch(x, y));
		freePoints.pop_back();
		// Insert wall at switch
		set(x, y, wallVal);
	}
	// Set patrols
	patrols = std::vector<Patrol>();
	for (int i = 0; i < nrPatrols; i++) {
		patrols.push_back(Patrol(freePoints.back().x + enemyOffset, freePoints.back().y + enemyOffset, rndD.getNextValue() * 2 - 1));
		freePoints.pop_back();
	}
	// Projectiles
	projectiles = std::vector<Projectile>();
}