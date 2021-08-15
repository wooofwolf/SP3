#pragma once

#include <string>

#include "MatrixStack.h"

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
	int GetGrid(int x, int y);
	// Mutators
	void ReadFile(std::string fileName);
};

