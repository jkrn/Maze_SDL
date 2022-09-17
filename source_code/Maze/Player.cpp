#include "Player.hpp"
#include "Constants.hpp"

Player::Player() : Player(0, 0) {}

Player::Player(int x, int y) :
	Unit(x,y),
	health(playerMaxHealth),
	timeOutFrames(0)
{
	isPlayer = true;
}

Player::~Player() {}