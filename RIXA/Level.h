#pragma once
#include <string>
#include <vector>
#include "Play.h"
#include "Collision.h"

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
	int getWidth();
	int getHeight();
	void addCollisionObject(CollisionBox box);
	vector<CollisionBox> getCollisionObjects();


private:
	int tileset;
	vector<Chunk> chunks;
	int width;
	int height;
	// number of pixels for each tile
	int px = 24;
	int py = 24;
	// required to sync frames with file format
	int frame = 71;

	vector<CollisionBox> collisionObjects;
};

