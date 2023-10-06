#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>

#include "Level.h"
#include "Collision.h"

using std::stringstream;

Level::Level() {}

// load in the level
Level::Level(string path, string tileset_s, string level) {
	tileset = PlayGraphics::Instance().LoadSpriteSheet(path, tileset_s, 9, 8);
	
	// input location
	std::ifstream input(level);

	// turn level into string
	auto ss = std::ostringstream{};
	ss << input.rdbuf();
	string levels = ss.str();	

	// get tileset file

	size_t layer = levels.find("layer");
	levels.erase(0, layer+5);

	levels.erase(0, levels.find("width=\"") + 7);
	width = stoi(levels.substr(0, levels.find("\"")));
	levels.erase(0, levels.find("height=\"") + 8);
	height = stoi(levels.substr(0, levels.find("\""))) * px;

	bool stop = false;
	// get chunks
	while (!stop)
	{
		Chunk chunk;
		// filthy
		size_t lv = levels.find("chunk");
		if (lv > 999999) {
			stop = true;
			break;
		}
		levels.erase(0, levels.find("chunk") + 5);
		levels.erase(0, levels.find("x=\"") + 3);
		auto x = levels.substr(0, levels.find("\""));
		chunk.x = stoi(x);
		levels.erase(0, levels.find("y=\"") + 3);
		auto y = levels.substr(0, levels.find("\""));
		chunk.y = stoi(y);

		size_t beg = levels.find(">") + 2;

		size_t end = levels.find("</chunk>");
		string schunk = levels.substr(beg, end - beg);

		levels.erase(0, end + 8);

		vector<string> lines;
		stringstream s_stream(schunk);
		while (s_stream.good()) {
			string substr;
			getline(s_stream, substr, '\n');
			lines.push_back(substr);
		}
		//iterate through lines
		for (int i = 0; i < lines.size(); i++) {
			vector<int> vline;
			string line = lines.at(i);

			//iterate through columns
			stringstream s_stream(line);
			while (s_stream.good()) {
				string substr;
				getline(s_stream, substr, ',');
				if (substr != "") {
					vline.push_back(stoi(substr));
				}
			}
			chunk.tiles.push_back(vline);		
		}
		chunks.push_back(chunk);
	}
	
	// get collision layer

	size_t objectgroup = levels.find("objectgroup");
	levels.erase(0, objectgroup+11);


	size_t namep = levels.find("name");
	levels.erase(0, namep+6);
	string name = levels.substr(0, levels.find("\""));
	
	/// FORGIVE ME, GODS OF EFFICIENCY
	/// I KNOW THIS IS NOT THE BEST WAY TO DO THIS
	/// BUT I DO NOT HAVE TIME TO THINK OF THE BEST SOLUTION
	bool stop1 = false;
	while (!stop1) {
		if (name == "collision") {
			stop = false;
			// get chunks
			while (!stop)
			{

				std::cout << "dbg";
				size_t object = levels.find("object");
				levels.erase(0, object + 6);
				//string name = levels.substr(0, levels.find("\""));

				size_t x = levels.find("x=\"") + 3;
				levels.erase(0, x);
				int xi = stoi(levels.substr(0, levels.find("\"")));

				size_t y = levels.find("y=\"") + 3;
				levels.erase(0, y);
				int yi = stoi(levels.substr(0, levels.find("\"")));

				size_t width = levels.find("width=\"") + 7;
				levels.erase(0, width);
				int widthi = stoi(levels.substr(0, levels.find("\"")));

				size_t height = levels.find("height=\"") + 8;
				levels.erase(0, height);
				int heighti = stoi(levels.substr(0, levels.find("\"")));
				levels.erase(0, levels.find("\""));

				collisionObjects.push_back(CollisionBox({ xi, yi }, { xi + widthi, yi + heighti }));

				// disgusting
				if (levels.find("</objectgroup>") <= 5) {
					stop = true;
					break;
				}
			}
		}
		std::cout << "dbg";

		objectgroup = levels.find("objectgroup");
		levels.erase(0, objectgroup + 11);


				levels.erase(0, levels.find("\""));
		namep = levels.find("name");
		levels.erase(0, namep + 6);
		name = levels.substr(0, levels.find("\""));

		std::cout << "dbg";
		if (name == "enemy1") {
			stop = false;
			// get chunks
			while (!stop)
			{
				size_t object = levels.find("object");
				levels.erase(0, object + 6);
				//string name = levels.substr(0, levels.find("\""));

				size_t x = levels.find("x=\"") + 3;
				levels.erase(0, x);
				int xi = floor(stof(levels.substr(0, levels.find("\""))));

				size_t y = levels.find("y=\"") + 3;
				levels.erase(0, y);
				int yi = floor(stof(levels.substr(0, levels.find("\""))));
				levels.erase(0, levels.find("\""));

				//collisionObjects.push_back(CollisionBox({ xi, yi }, { xi + widthi, yi + heighti }));
				enemies.push_back(EnemyData{ xi, yi, 1 });

				// disgusting
				if (levels.find("</objectgroup>") <= 5) {
					stop = true;
					break;
				}
			}
		}

		std::cout << "dbg";
		objectgroup = levels.find("objectgroup");
		levels.erase(0, objectgroup + 11);


		namep = levels.find("name");
		levels.erase(0, namep + 6);
		name = levels.substr(0, levels.find("\""));

		std::cout << "dbg";

		if (name == "enemy2") {
			stop = false;
			// get chunks
			while (!stop)
			{
				size_t object = levels.find("object");
				levels.erase(0, object + 6);
				//string name = levels.substr(0, levels.find("\""));

				size_t x = levels.find("x=\"") + 3;
				levels.erase(0, x);
				int xi = floor(stof(levels.substr(0, levels.find("\""))));

				size_t y = levels.find("y=\"") + 3;
				levels.erase(0, y);
				int yi = floor(stof(levels.substr(0, levels.find("\""))));
				levels.erase(0, levels.find("\""));

				//collisionObjects.push_back(CollisionBox({ xi, yi }, { xi + widthi, yi + heighti }));
				enemies.push_back(EnemyData{ xi, yi, 2 });

				// disgusting
				if (levels.find("</objectgroup>") <= 5) {
					stop = true;
					break;
				}
			}
		}

		std::cout << "dbg";
		objectgroup = levels.find("objectgroup");
		levels.erase(0, objectgroup + 11);


		namep = levels.find("name");
		levels.erase(0, namep + 6);
		name = levels.substr(0, levels.find("\""));

		std::cout << "dbg";

		if (name == "enemy3") {
			stop = false;
			// get chunks
			while (!stop)
			{
				size_t object = levels.find("object");
				levels.erase(0, object + 6);
				//string name = levels.substr(0, levels.find("\""));

				size_t x = levels.find("x=\"") + 3;
				levels.erase(0, x);
				int xi = floor(stof(levels.substr(0, levels.find("\""))));

				size_t y = levels.find("y=\"") + 3;
				levels.erase(0, y);
				int yi = floor(stof(levels.substr(0, levels.find("\""))));
				levels.erase(0, levels.find("\""));

				//collisionObjects.push_back(CollisionBox({ xi, yi }, { xi + widthi, yi + heighti }));
				enemies.push_back(EnemyData{ xi, yi, 3 });

				// disgusting
				if (levels.find("</objectgroup>") <= 5) {
					stop = true;
					break;
				}
			}
		}	
		/// need more efficient way of breaking loop
		stop1 = true;
		break;
	}
}

// display graphics directly to using undocumented PlayBuffer functions
// no gameobjects needed
void Level::display(float x, float y, int dwidth, int dheight) {
	for (auto it = chunks.begin(); it != chunks.end(); ++it) {
		//it->draw();
		auto chunk = *it;
		for (int j = 0; j < chunk.tiles.size(); j++) {
			auto line = chunk.tiles[j];
			for (int k = 0; k < line.size(); k++) {
				int dx = (k + chunk.x) * px + x;
				int dy = (j+chunk.y)*py+y;
				if (dx >= -33 && dx < dwidth && dy >= -33 && dy < dheight) {
					PlayGraphics::Instance().Draw(tileset, { dx,dy }, line[k] + frame);
				}
			}
		}
	}
}

int Level::getWidth() {
	return width;
}

int Level::getHeight() {
	return height;
}

void Level::addCollisionObject(CollisionBox box) {
	collisionObjects.push_back(box);
}
vector<CollisionBox> Level::getCollisionObjects() {
	return collisionObjects;
}

vector<EnemyData> Level::getEnemyData() {
	return enemies;
}

bool Level::isColliding(int x, int y, int r) {
	for (auto it = collisionObjects.begin(); it != collisionObjects.end(); ++it) {
		auto obj = *it;
		if (obj.checkColliding(x, y, r)) {
			return true;
		}
	}
	return false;
}
