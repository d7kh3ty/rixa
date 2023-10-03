#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>

#include "Level.h"

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
	width = stoi(levels.substr(0, levels.find("\""))) * px;
	levels.erase(0, levels.find("height=\"") + 8);
	std::cout << "levels";
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
					std::cout << substr;
					vline.push_back(stoi(substr));
				}
			}
			chunk.tiles.push_back(vline);		
		}
		chunks.push_back(chunk);
	}
	
	// get collision layer
}

// display graphics directly to using undocumented PlayBuffer functions
// no gameobjects needed
void Level::display(float x, float y) {
	for (auto it = chunks.begin(); it != chunks.end(); ++it) {
		//it->draw();
		auto chunk = *it;
		for (int j = 0; j < chunk.tiles.size(); j++) {
			auto line = chunk.tiles[j];
			for (int k = 0; k < line.size(); k++) {
				//auto idk = *kt;

				PlayGraphics::Instance().Draw(tileset, {(k+chunk.x)*px+x,(j+chunk.y)*py+y}, line[k]+frame);
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
