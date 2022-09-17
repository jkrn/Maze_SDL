#include "Maze.hpp"
#include "Direct.hpp"
#include "RandomGenerator.hpp"
#include <algorithm>

Maze::Maze(int w, int h) : w(w), h(h) {
	maze = new int*[w];
	for (int i = 0; i < w; i++) {
		maze[i] = new int[h];
		std::fill(maze[i], maze[i] + h, 0);
	}
	RandomGenerator rndX(0, w - 1);
	RandomGenerator rndY(0, h - 1);
	generateMaze(rndX.getNextValue(), rndY.getNextValue());
}

Maze::~Maze() {
	for (int i = 0; i < w; i++) {
		delete[] maze[i];
	}
	delete[] maze;
}

bool Maze::isDeadEnd(int cx, int cy) {
	if (maze[cx][cy] == NORTH_VAL || maze[cx][cy] == SOUTH_VAL || maze[cx][cy] == EAST_VAL || maze[cx][cy] == WEST_VAL) {
		return true;
	}
	return false;
}

void Maze::generateMaze(int cx, int cy) {
	DIR dirs[4] = { NORTH, WEST, SOUTH, EAST };
	std::random_shuffle(std::begin(dirs), std::end(dirs));
	for (int i = 0; i < 4; i++) {
		Direct dir(dirs[i]);
		int nx = cx + dir.dx;
		int ny = cy + dir.dy;
		if (between(nx, w) && between(ny, h)
			&& (maze[nx][ny] == 0)) {
			maze[cx][cy] |= dir.bit;
			maze[nx][ny] |= dir.oppBit;
			generateMaze(nx, ny);
		}
	}
}

bool Maze::between(int v, int upper) {
	return (v >= 0) && (v < upper);
}