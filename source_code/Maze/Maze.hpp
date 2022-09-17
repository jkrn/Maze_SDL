#pragma once

class Maze {
public:
	const int w;
	const int h;
	int** maze;
	Maze(int w, int h);
	~Maze();
	bool isDeadEnd(int cx, int cy);
private:
	void generateMaze(int cx, int cy);
	bool between(int v, int upper);
};