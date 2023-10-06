#pragma once
#include <string>
#include <vector>
#include "Play.h"
#include "Collision.h"


struct EnemyData {
	int x;
	int y;
	int type;
};

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
	void display(float x, float y, int dwidth, int dheight);
	int getWidth();
	int getHeight();
	void addCollisionObject(CollisionBox box);
	bool isColliding(int x, int y, int r);
	void changeFrame();
	vector<CollisionBox> getCollisionObjects();
	vector<EnemyData> getEnemyData();


private:
	int tileset;
	vector<Chunk> chunks;
	int width;
	int height;
	// number of pixels for each tile
	int px = 32;
	int py = 32;
	// required to sync frames with file format
	int frame = 1422;

	vector<CollisionBox> collisionObjects;
	vector<EnemyData> enemies;
};

