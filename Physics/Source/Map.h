#pragma once

#include <string>

#include "MatrixStack.h"
#include "SceneBase.h"
#include "SingletonPattern.h"
#include "MeshList.h"

class Map: public SceneBase, public SingletonPattern<Map>
{
	friend SingletonPattern<Map>;
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
	float m_worldWidth;
	float m_worldHeight;
	
	// Constructor
	Map();
	// Destructor
	~Map();
	void RenderMap(std::string fileName);
	// Accessors
	int GetGrid(int x, int y);
	// Mutators
	void ReadFile(std::string fileName);

	Mesh* mesh;
};

