#include "Map.h"

#include <iostream>
#include <string>
#include <fstream>
#include <set>

#include "Application.h"
#include "GL/glew.h"
#include "Camera.h"
#include "SceneBase.h"
#include "Player.h"
//#include "SceneTest.h"
//#include "SceneLevel3.h"

typedef std::pair<int, int> pair;
typedef std::pair<double, pair> pPair;

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
}

Map::~Map()
{
}

void Map::Init(std::string fileName)
{
	Map::GetInstance()->ReadFile(fileName);
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
		char c;
		while (fin.get(c))
		{
			if (c == '2')
			{
				Player::GetInstance()->SetPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
				c = '0';
			}
			if (c == '4')
			{
				CavemenMelee* CavemanMelee = new CavemenMelee(baseEnemy);
				CavemanMelee->Init();
				CavemanMelee->SetEnemyPos(Vector3(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1));
				SceneManager::GetInstance()->GetActiveScene()->enemyList.push_back(CavemanMelee);
				c = '0';
			}
			if (c == '\n')
			{
				++y;
				x = 0;
			}
			else if (c - '0' >= static_cast<int>(Tile::TILE_TYPE::TILE_NONE) && c - '0' <= static_cast<int>(Tile::TILE_TYPE::TILE_TOTAL))
			{
				Tile newTile = Tile();
				newTile.pos.Set(x, y, 1);
				newTile.type = static_cast<Tile::TILE_TYPE>(c - '0');
				mapGrid[x][y] = newTile;
				++x;
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
	return sqrt(2 * (pos.x - dest.x) + (pos.y - dest.y) * 2);
}

std::stack<Vector3> Map::TracePath(Vector3 dest)
{
	int x = dest.x;
	int y = dest.y;

	std::stack<Vector3> path;
	while (!(mapGrid[x][y].parent_i == x && mapGrid[x][y].parent_j == y))
	{
		path.push(Vector3(x, y, 1));
		int temp_x = mapGrid[x][y].parent_i;
		int temp_y = mapGrid[x][y].parent_j;
		x = temp_x;
		y = temp_y;
	}

	path.push(Vector3(x, y, 1));

	return path;
}

std::stack<Vector3> Map::AStarSearch(Vector3 source, Vector3 destination)
{
	Vector3 src = Vector3((source.x - 2) / 4, (source.y - m_worldHeight + 2) / -4);
	Vector3 dest = Vector3((destination.x - 2) / 4, (destination.y - m_worldHeight + 2) / -4);

	if (IsValid(src) == false)
	{
		std::cout << "Source is invalid" << std::endl;
		return std::stack<Vector3>();
	}
	if (IsValid(dest) == false)
	{
		std::cout << "Destination is invalid" << std::endl;
		return std::stack<Vector3>();
	}

	if (IsUnblocked(src) == false || IsUnblocked(dest) == false)
	{
		std::cout << "Source or destination is blocked" << std::endl;
		return std::stack<Vector3>();
	}

	if (IsDestination(src, dest))
	{
		std::cout << "Already at destination" << std::endl;
		return std::stack<Vector3>();
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
				return TracePath(dest);
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
				return TracePath(dest);
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
				return TracePath(dest);
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
				return TracePath(dest);
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
				return TracePath(dest);
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
				return TracePath(dest);
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
				return TracePath(dest);
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
				return TracePath(dest);
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
	if (foundDest == false)
		std::cout << "Failed to find the Destination Cell" << std::endl;

	return std::stack<Vector3>();
}
