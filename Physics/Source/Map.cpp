#include "Map.h"

#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <sstream>
#include <mutex>

#include "Application.h"
#include "GL/glew.h"
#include "Camera.h"
#include "SceneBase.h"
#include "Player.h"

#include "Boss.h"

typedef std::pair<int, int> pair;
typedef std::pair<double, pair> pPair;

static std::mutex mutex;

Map::Map()
{
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	activeTileList.clear();

	for (int i = 0; i < 200; ++i)
	{
		for (int j = 0; j < 200; ++j)
		{
			mapGrid[i][j] = Tile();
		}
	}

	totalEnemyCount = 0;
}

Map::~Map()
{
}

void Map::Init(std::string fileName)
{
	ReadFile(fileName);
}

void Map::RenderMap(std::string fileName)
{
	activeTileList.clear();
	ReadFile(fileName);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	for (int y = 0; y < 200; ++y)
	{
		for (int x = 0; x < 200; ++x)
		{
			switch(GetTile(x,y).type)
			{
			case Tile::TILE_TYPE::TILE_WALL:
				Vector3 tilePos = Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
				float width = (tilePos - Camera::GetInstance()->position).Length();
				float height = Camera::GetInstance()->position.z - tilePos.z;
				float hypotenuse = sqrtf(width * width + height * height);
				float maxWidth = (Vector3(m_worldWidth / 2 + 2, 0, 1).Dot(Vector3(m_worldHeight / 2 + 2, 0, 1)) - Camera::GetInstance()->position.Length());
				float maxHypotenuse = sqrtf(maxWidth * maxWidth + height * height);

				if (hypotenuse <= maxHypotenuse)
				{
					activeTileList.push_back(tilePos);
					modelStack.PushMatrix();
					modelStack.Translate(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
					modelStack.Scale(2, 2, 2);
					//RenderMesh(MeshList::GetInstance()->GetMesh(MeshList::GEO_CUBE), true);
					modelStack.PopMatrix();
					break;
				}
			}
		}
	}

	Player::GetInstance()->Collisioncheck(activeTileList);
}

Tile Map::GetTile(int x, int y)
{
	return mapGrid[x][y];
}

void Map::SetTile(int x, int y, Tile tile)
{
	this->mapGrid[x][y] = tile;
}

void Map::ReadFile(std::string fileName)
{
	std::fstream fin;

	fin.open(fileName, std::ios::in);
		int x, y;
		x = y = 0;
		std::string str;
		int val;
		while (std::getline(fin, str))
		{
			std::stringstream ss(str);
				
			while (ss >> val)
			{
				if (val == 2)
				{
					Player::GetInstance()->SetPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
					val = 0;
				}
				if (val == 4)
				{
					CavemenMelee* CavemanMelee = new CavemenMelee(baseEnemy);
					CavemanMelee->Init();
					CavemanMelee->SetEnemyPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
					SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(CavemanMelee);
					val = 0;
					++totalEnemyCount;
				}
				if (val == 6)
				{
					CavemenRanged* CavemanRanged = new CavemenRanged(baseEnemy);
					CavemanRanged->Init();
					CavemanRanged->SetEnemyPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
					SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(CavemanRanged);
					val = 0;
					++totalEnemyCount;
				}
				if (val == 7)
				{
					Knights* knight = new Knights(baseEnemy);
					knight->Init();
					knight->SetEnemyPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
					SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(knight);
					val = 0;
					++totalEnemyCount;
				}
				if (val == 8)
				{
					Archer* archers = new Archer(baseEnemy);
					archers->Init();
					archers->SetEnemyPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
					SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(archers);
					val = 0;
					++totalEnemyCount;
				}
				if (val == 9)
				{
					Cyborg* cyborg = new Cyborg(baseEnemy);
					cyborg->Init();
					cyborg->SetEnemyPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
					SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(cyborg);
					val = 0;
					++totalEnemyCount;
				}
				if (val == 10)
				{
					Sniper* snipers = new Sniper(baseEnemy);
					snipers->Init();
					snipers->SetEnemyPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
					SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(snipers);
					val = 0;
					++totalEnemyCount;
				}
				if (val == 100)
				{
					Boss* boss = Boss::GetInstance();
					boss->SetPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
					val = 0;
					++totalEnemyCount;
				}

				if (val >= static_cast<int>(Tile::TILE_TYPE::TILE_NONE) && val <= static_cast<int>(Tile::TILE_TYPE::TILE_TOTAL))
				{
					Tile newTile = Tile();
					newTile.pos.Set(x, y, 1);
					newTile.type = static_cast<Tile::TILE_TYPE>(val);
					mapGrid[x][y] = newTile;
					++x;
					if (x > 199)
					{
						++y;
						x = 0;
					}
				}
				if (ss.peek() == ',')
				{
					ss.ignore();
				}
			}
		}
	fin.close();
}

bool Map::IsValid(Vector3 pos)
{
	// enemy pos to grid pos
	//Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1)
	return (pos.x >= 0 && pos.x < 200) && (pos.y >= 0 && pos.y < 200);
}

bool Map::IsUnblocked(Vector3 pos)
{
	// Check position of tiles and see if its blocking the grid
	return mapGrid[(int)(pos.x)][(int)(pos.y)].type == Tile::TILE_TYPE::TILE_NONE;
}

bool Map::IsDestination(Vector3 pos, Vector3 dest)
{
	return (int)pos.x == (int)dest.x && (int)pos.y == (int)dest.y;
}

double Map::CalculateHValue(Vector3 pos, Vector3 dest)
{
	return  abs(pos.x - dest.x) +
		abs(pos.y - dest.y);
}

void Map::TracePath(Vector3 dest, std::vector<Vector3>* path)
{
	path->clear();
	int x = dest.x;
	int y = dest.y;

	while (!(mapGrid[x][y].parent_i == x && mapGrid[x][y].parent_j == y))
	{
		path->push_back(Vector3(x, y, 1));
		int temp_x = mapGrid[x][y].parent_i;
		int temp_y = mapGrid[x][y].parent_j;
		x = temp_x;
		y = temp_y;
	}

	return;
}

void Map::AStarSearch(Vector3 source, Vector3 destination, std::vector<Vector3>* path)
{
	std::lock_guard<std::mutex> lock(mutex);

	Vector3 src = Vector3((source.x - 2) / 4, (source.y - m_worldHeight + 2) / -4);
	Vector3 dest = Vector3((destination.x - 2) / 4, (destination.y - m_worldHeight + 2) / -4);

	if (IsValid(src) == false)
	{
		return;
	}
	if (IsValid(dest) == false)
	{
		return;
	}

	if (IsDestination(src, dest))
	{
		return;
	}

	bool closedList[200][200];
	memset(closedList, false, sizeof(closedList));

	int i, j;
	for (i = 0; i < 200; i++)
	{
		for (j = 0; j < 200; j++)
		{
			mapGrid[i][j].f = FLT_MAX;
			mapGrid[i][j].g = FLT_MAX;
			mapGrid[i][j].h = FLT_MAX;

			mapGrid[i][j].parent_i = -1;
			mapGrid[i][j].parent_i = -1;
		}
	}

	i = src.x, j = src.y;
	mapGrid[i][j].f = 0.0;
	mapGrid[i][j].g = 0.0;
	mapGrid[i][j].h = 0.0;

	mapGrid[i][j].parent_i = i;
	mapGrid[i][j].parent_j = j;

	std::set<pPair> openList;
	openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

	bool foundDest = false;
	while (!openList.empty())
	{
		pPair p = *openList.begin();
		openList.erase(openList.begin());

		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		double gNew, hNew, fNew;
		// North
		if (IsValid(Vector3(i - 1, j)) == true)
		{
			if (IsDestination(Vector3(i - 1, j), dest) == true)
			{
				mapGrid[i - 1][j].parent_i = i;
				mapGrid[i - 1][j].parent_j = j;
				foundDest = true;
				TracePath(dest, path);
				return;
			}
			else if (closedList[i - 1][j] == false && IsUnblocked(Vector3(i - 1, j)) == true)
			{
				gNew = mapGrid[i][j].g + 1.0;
				hNew = CalculateHValue(Vector3(i - 1, j), dest);
				fNew = gNew + hNew;

				if (mapGrid[i - 1][j].f == FLT_MAX || mapGrid[i - 1][j].f > fNew)
				{
					openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j)));

					mapGrid[i - 1][j].f = fNew;
					mapGrid[i - 1][j].g = gNew;
					mapGrid[i - 1][j].h = hNew;
					mapGrid[i - 1][j].parent_i = i;
					mapGrid[i - 1][j].parent_j = j;
				}
			}
		}
		// South
		if (IsValid(Vector3(i + 1, j)) == true)
		{
			if (IsDestination(Vector3(i + 1, j), dest) == true)
			{
				mapGrid[i + 1][j].parent_i = i;
				mapGrid[i + 1][j].parent_j = j;
				foundDest = true;
				TracePath(dest, path);
				return;
			}
			else if (closedList[i + 1][j] == false && IsUnblocked(Vector3(i + 1, j)) == true)
			{
				gNew = mapGrid[i][j].g + 1.0;
				hNew = CalculateHValue(Vector3(i + 1, j), dest);
				fNew = gNew + hNew;

				if (mapGrid[i + 1][j].f == FLT_MAX || mapGrid[i + 1][j].f > fNew)
				{
					openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j)));

					mapGrid[i + 1][j].f = fNew;
					mapGrid[i + 1][j].g = gNew;
					mapGrid[i + 1][j].h = hNew;
					mapGrid[i + 1][j].parent_i = i;
					mapGrid[i + 1][j].parent_j = j;
				}
			}
		}
		// East
		if (IsValid(Vector3(i, j + 1)) == true)
		{
			if (IsDestination(Vector3(i, j + 1), dest) == true)
			{
				mapGrid[i][j + 1].parent_i = i;
				mapGrid[i][j + 1].parent_j = j;
				foundDest = true;
				TracePath(dest, path);
				return;
			}
			else if (closedList[i][j + 1] == false && IsUnblocked(Vector3(i, j + 1)) == true)
			{
				gNew = mapGrid[i][j].g + 1.0;
				hNew = CalculateHValue(Vector3(i, j + 1), dest);
				fNew = gNew + hNew;

				if (mapGrid[i][j + 1].f == FLT_MAX || mapGrid[i][j + 1].f > fNew)
				{
					openList.insert(std::make_pair(fNew, std::make_pair(i, j + 1)));

					mapGrid[i][j + 1].f = fNew;
					mapGrid[i][j + 1].g = gNew;
					mapGrid[i][j + 1].h = hNew;
					mapGrid[i][j + 1].parent_i = i;
					mapGrid[i][j + 1].parent_j = j;
				}
			}
		}
		// West
		if (IsValid(Vector3(i, j - 1)) == true)
		{
			if (IsDestination(Vector3(i, j - 1), dest) == true)
			{
				mapGrid[i][j - 1].parent_i = i;
				mapGrid[i][j - 1].parent_j = j;
				foundDest = true;
				TracePath(dest, path);
				return;
			}
			else if (closedList[i][j - 1] == false && IsUnblocked(Vector3(i, j - 1)) == true)
			{
				gNew = mapGrid[i][j].g + 1.0;
				hNew = CalculateHValue(Vector3(i, j - 1), dest);
				fNew = gNew + hNew;

				if (mapGrid[i][j - 1].f == FLT_MAX || mapGrid[i][j - 1].f > fNew)
				{
					openList.insert(std::make_pair(fNew, std::make_pair(i, j - 1)));

					mapGrid[i][j - 1].f = fNew;
					mapGrid[i][j - 1].g = gNew;
					mapGrid[i][j - 1].h = hNew;
					mapGrid[i][j - 1].parent_i = i;
					mapGrid[i][j - 1].parent_j = j;
				}
			}
		}
		// North-East
		if (IsValid(Vector3(i - 1, j + 1)) == true)
		{
			if (IsDestination(Vector3(i - 1, j + 1), dest) == true)
			{
				mapGrid[i - 1][j + 1].parent_i = i;
				mapGrid[i - 1][j + 1].parent_j = j;
				foundDest = true;
				TracePath(dest, path);
				return;
			}
			else if (closedList[i - 1][j + 1] == false && IsUnblocked(Vector3(i - 1, j + 1)) == true)
			{
				gNew = mapGrid[i][j].g + 1.414;
				hNew = CalculateHValue(Vector3(i - 1, j + 1), dest);
				fNew = gNew + hNew;

				if (mapGrid[i - 1][j + 1].f == FLT_MAX || mapGrid[i - 1][j + 1].f > fNew)
				{
					openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j + 1)));

					mapGrid[i - 1][j + 1].f = fNew;
					mapGrid[i - 1][j + 1].g = gNew;
					mapGrid[i - 1][j + 1].h = hNew;
					mapGrid[i - 1][j + 1].parent_i = i;
					mapGrid[i - 1][j + 1].parent_j = j;
				}
			}
		}
		// North-West
		if (IsValid(Vector3(i - 1, j - 1)) == true)
		{
			if (IsDestination(Vector3(i - 1, j - 1), dest) == true)
			{
				mapGrid[i - 1][j - 1].parent_i = i;
				mapGrid[i - 1][j - 1].parent_j = j;
				foundDest = true;
				TracePath(dest, path);
				return;
			}
			else if (closedList[i - 1][j - 1] == false && IsUnblocked(Vector3(i - 1, j - 1)) == true)
			{
				gNew = mapGrid[i][j].g + 1.414;
				hNew = CalculateHValue(Vector3(i - 1, j - 1), dest);
				fNew = gNew + hNew;

				if (mapGrid[i - 1][j - 1].f == FLT_MAX || mapGrid[i - 1][j - 1].f > fNew)
				{
					openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j - 1)));

					mapGrid[i - 1][j - 1].f = fNew;
					mapGrid[i - 1][j - 1].g = gNew;
					mapGrid[i - 1][j - 1].h = hNew;
					mapGrid[i - 1][j - 1].parent_i = i;
					mapGrid[i - 1][j - 1].parent_j = j;
				}
			}
		}
		// South-East
		if (IsValid(Vector3(i + 1, j + 1)) == true)
		{
			if (IsDestination(Vector3(i + 1, j + 1), dest) == true)
			{
				mapGrid[i + 1][j + 1].parent_i = i;
				mapGrid[i + 1][j + 1].parent_j = j;
				foundDest = true;
				TracePath(dest, path);
				return;
			}
			else if (closedList[i + 1][j + 1] == false && IsUnblocked(Vector3(i + 1, j + 1)) == true)
			{
				gNew = mapGrid[i][j].g + 1.414;
				hNew = CalculateHValue(Vector3(i + 1, j + 1), dest);
				fNew = gNew + hNew;

				if (mapGrid[i + 1][j + 1].f == FLT_MAX || mapGrid[i + 1][j + 1].f > fNew)
				{
					openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j + 1)));

					mapGrid[i + 1][j + 1].f = fNew;
					mapGrid[i + 1][j + 1].g = gNew;
					mapGrid[i + 1][j + 1].h = hNew;
					mapGrid[i + 1][j + 1].parent_i = i;
					mapGrid[i + 1][j + 1].parent_j = j;
				}
			}
		}
		// South-West
		if (IsValid(Vector3(i + 1, j - 1)) == true)
		{
			if (IsDestination(Vector3(i + 1, j - 1), dest) == true)
			{
				mapGrid[i + 1][j - 1].parent_i = i;
				mapGrid[i + 1][j - 1].parent_j = j;
				foundDest = true;
				TracePath(dest, path);
				return;
			}
			else if (closedList[i + 1][j - 1] == false && IsUnblocked(Vector3(i + 1, j - 1)) == true)
			{
				gNew = mapGrid[i][j].g + 1.414;
				hNew = CalculateHValue(Vector3(i + 1, j - 1), dest);
				fNew = gNew + hNew;

				if (mapGrid[i + 1][j - 1].f == FLT_MAX || mapGrid[i + 1][j - 1].f > fNew)
				{
					openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j - 1)));

					mapGrid[i + 1][j - 1].f = fNew;
					mapGrid[i + 1][j - 1].g = gNew;
					mapGrid[i + 1][j - 1].h = hNew;
					mapGrid[i + 1][j - 1].parent_i = i;
					mapGrid[i + 1][j - 1].parent_j = j;
				}
			}
		}
	}

	return;
}
