#pragma once

#include <string>

#include "MatrixStack.h"
#include "SingletonPattern.h"
#include "MeshList.h"

class Map: public SingletonPattern<Map>
{
	friend SingletonPattern<Map>;
public:
	// Tileset
	enum class TILES
	{
		TILE_NONE = 0,
		TILE_WALL,
		TILE_TOTAL
	};

	float m_worldWidth;
	float m_worldHeight;
	
	// Constructor
	Map();
	// Destructor
	~Map();
	void RenderMap(std::string fileName);
	// Accessors
	TILES GetTile(int x, int y);
	// Mutators
	void ReadFile(std::string fileName);

private:
	// Variables
	TILES mapGrid[200][200];

	MS modelStack;
};

