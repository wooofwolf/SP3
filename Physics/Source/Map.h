#pragma once

#include <string>
#include <vector>

#include "MatrixStack.h"
#include "SingletonPattern.h"
#include "MeshList.h"
#include "Player.h"
#include "BaseEnemy.h"
#include <functional>
#include <queue>
struct Grid {
	unsigned int value;

	Grid()
		:value(0), pos(0, 0,0), parent(-1, -1,0), f(0), g(0), h(0) {}

	Grid(const Vector3 pos, unsigned int f)
		:value(0), pos(pos), parent(-1, 1,0), f(f), g(0), h(0) {}

	Grid(Vector3 pos, Vector3 parent, unsigned int f, unsigned int g, unsigned int h)
		:value(0), pos(pos), parent(parent), f(f), g(g), h(h) {}

	Vector3 pos;
	Vector3 parent;
	unsigned int f;
	unsigned int g;
	unsigned int h;


};
using HeuristicFunction = std::function<unsigned int(const Vector3&, const Vector3& , int)>;
//reverse std::priority_queue to get the largest f value on top
inline bool operator <(const Grid& a, const Grid& b) { return b.f < a.f; }
namespace heuristic
{
	unsigned int manhattan(const Vector3 v1, const Vector3& v2, int);
	unsigned int euclidean(const Vector3 v1, const Vector3& v2, int);
}

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

		TILE_PICKUP,

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

	//for Astar Pathfinding
	std::vector<Vector3>PathFind(const Vector3& startPos, const Vector3& targetPos, HeuristicFunction heuristicFunc);
	//set if Astar PathFinding will consider diagonal movements
	void SetDiagonalMovement(const bool bEnable);
	//print out details about this class instance in the console window
	void PrintSelf(void) const;
	BaseEnemy* baseEnemy;
	std::vector<CavemenMelee*> CaveMeleeList;
private:
	// Variables
	TILES mapGrid[200][200];
	std::vector <Vector3> activeTileList;

	MS modelStack;
protected:

	Grid** arrMapInfo;
	// For A-Star PathFinding
// Build a path from m_cameFromList after calling PathFind()
	std::vector<Vector3> BuildPath() const;
	// Check if a grid is valid
	bool isValid(const Vector3& pos) const;
	// Check if a grid is blocked
	bool isBlocked(const unsigned int uiRow,
		const unsigned int uiCol,
		const bool bInvert = true) const;
	// Convert a position to a 1D position in the array
	int ConvertTo1D(const Vector3& pos) const;
	// Delete AStar lists
	bool DeleteAStarLists(void);
	// Reset AStar lists
	bool ResetAStarLists(void);



	//int m_size;
	unsigned int m_nrOfDirections;
	//glm::i32vec2 m_dimensions;
	Vector3 m_startPos;
	Vector3 m_targetPos;
	std::priority_queue<Grid> m_openList;
	std::vector<bool> m_closedList;
	std::vector<Grid> m_cameFromList;
	//std::vector<int> m_grid;
	std::vector<Vector3> m_directions;
	HeuristicFunction m_heuristic;
};

