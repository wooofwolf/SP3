#pragma once

#include <string>
#include <vector>

#include "MatrixStack.h"
#include "SingletonPattern.h"
#include "MeshList.h"
#include "Player.h"
//#include "BaseEnemy.h"

struct Tile {
	// Tileset
	enum class TILE_TYPE
	{
		TILE_NONE = 0,
		TILE_WALL,

		TILE_PLAYER,

		TILE_PICKUP,

		TILE_HAZARD = 5,

		TILE_CAVEMAN,
		TILE_SPEARMAN,
		TILE_KNIGHT,
		TILE_ARCHER,
		TILE_CYBORG,
		TILE_SNIPER,

		TILE_EXIT = 99,
		
		TILE_TOTAL
	};

	TILE_TYPE type;

	Tile() :type(TILE_TYPE::TILE_NONE), pos(Vector3()), parent_i(-1), parent_j(-1), f(0), g(0), h(0) {}

	Tile(const Vector3 pos, unsigned int f)
		:type(TILE_TYPE::TILE_NONE), pos(pos), parent_i(-1), parent_j(-1), f(f), g(0), h(0) {}

	Tile(Vector3 pos, int i, int j, unsigned int f, unsigned int g, unsigned int h)
		:type(TILE_TYPE::TILE_NONE), pos(pos), parent_i(i), parent_j(j), f(f), g(g), h(h) {}

	Vector3 pos;
	int parent_i, parent_j;
	double f, g, h;
};

class Map: public SingletonPattern<Map>
{
	friend SingletonPattern<Map>;
public:
	float m_worldWidth;
	float m_worldHeight;
	
	// Constructor
	Map();
	// Destructor
	~Map();

	void Init(std::string fileName);

	void RenderMap(std::string fileName);
	// Accessors
	Tile GetTile(int x, int y);
	void SetTile(int x, int y, Tile tile);
	// Mutators
	void ReadFile(std::string fileName);

	BaseEnemy* baseEnemy;
	std::vector<CavemenMelee*> CaveMeleeList;

	// A* Pathfinding
	bool IsValid(Vector3 pos);
	bool IsUnblocked(Vector3 pos);
	bool IsDestination(Vector3 pos, Vector3 dest);
	double CalculateHValue(Vector3 pos, Vector3 dest);
	std::stack<Vector3> TracePath(Vector3 dest);
	std::stack<Vector3> AStarSearch(Vector3 src, Vector3 dest);

private:
	// Variables
	Tile mapGrid[200][200];
	std::vector <Vector3> activeTileList;

	MS modelStack;
};

