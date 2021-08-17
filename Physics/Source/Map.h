#pragma once

#include <string>
#include <vector>

#include "MatrixStack.h"
#include "SingletonPattern.h"
#include "MeshList.h"
#include "Player.h"

class Map: public SingletonPattern<Map>
{
	friend SingletonPattern<Map>;
public:
	// Tileset
	enum class TILES
	{
		TILE_NONE = 0,
		TILE_WALL,

		TILE_PLAYER,

		TILE_TOTAL
	};

	float m_worldWidth;
	float m_worldHeight;
	
	// Constructor
	Map();
	// Destructor
	~Map();

	void Init(std::string fileName);

	void RenderMap(std::string fileName);
	// Accessors
	TILES GetTile(int x, int y);
	void SetTile(int x, int y, TILES tile);
	// Mutators
	void ReadFile(std::string fileName);

private:
	// Variables
	TILES mapGrid[200][200];
	std::vector <Vector3> activeTileList;

	MS modelStack;
};

