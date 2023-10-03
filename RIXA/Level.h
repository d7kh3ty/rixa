#pragma once
#include <string>
#include <vector>
#include "Play.h"

using std::string, std::vector;
struct Chunk {
	int x;
	int y;
	vector<vector<int>> tiles;
};

class Level {
public:
	Level();
	Level(string path, string tileset_s, string level);
	void display(float x, float y);

	// required to sync frames with file format
	int frame = 71;

private:
	int tileset;
	vector<Chunk> chunks;
	// number of pixels for each tile
	int px = 24;
	int py = 24;
};

