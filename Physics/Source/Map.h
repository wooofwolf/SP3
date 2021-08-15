#pragma once

#include <string>

class Map
{
private:
	// Tileset
	enum class TILES
	{
		TILE_NONE = 0,
		TILE_WALL,
		TILE_TOTAL
	};

	// Variables
	int mapGrid[200][200];

public:
	// Constructor
	Map();
	// Destructor
	~Map();

	// Accessors
	int getGrid(int x, int y);
	// Mutators
	void readFile(std::string fileName);
};

