#pragma once

#include "Constants.hpp"
#include "Drone.hpp"
#include "Shooter.hpp"
#include "Switch.hpp"
#include "Patrol.hpp"
#include "Boss.hpp"
#include "BonusItem.hpp"
#include "RandomGenerator.hpp"
#include <vector>

class Map {
public:
	int mazeWidth;
	int mazeHeight;
	int width;
	int height;
	int nrKeyItems;
	int nrBonusItems;
	int nrDrones;
	int nrShooters;
	int nrSwitches;
	int nrPatrols;
	int startX;
	int startY;
	int exitX;
	int exitY;
	int nrCollectedKeyItems;
	int nrBosses;
	int switchState;
	int prevSwitchState;
	bool isExitOpen;
	bool isDroneRed;
	bool droneAttack;
	bool isBossMap;
	std::vector<Point> keyItems;
	std::vector<BonusItem> bonusItems;
	std::vector<Drone> drones;
	std::vector<Shooter> shooters;
	std::vector<Switch> switches;
	std::vector<Boss> bosses;
	std::vector<Patrol> patrols;
	std::vector<Projectile> projectiles;
	Map();
	Map(int course);
	Map(int mazeWidth, int mazeHeight, int nrKeyItems, int nrBonusItems, int nrDrones, int nrShooters, int nrSwitches, int nrPatrols, int bonusValue);
	~Map();
	void removeBosses();
	bool operator()(int x, int y);
	void set(int x, int y, bool value);
private:
	Uint32 field[mapArraySize];
	void buildBossMap(int course);
	void buildMazeMap(int bonusValue);
};