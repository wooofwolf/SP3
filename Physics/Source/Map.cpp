#include "Map.h"

#include <iostream>
#include <string>
#include <fstream>

#include "Application.h"
#include "GL/glew.h"
#include "Camera.h"
#include "SceneBase.h"
#include "Player.h"

Map::Map() : mapGrid{TILES::TILE_NONE}
{
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	activeTileList.clear();
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
			switch(GetTile(x,y))
			{
			case TILES::TILE_WALL:
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

Map::TILES Map::GetTile(int x, int y)
{
	return mapGrid[x][y];
}

void Map::SetTile(int x, int y, Map::TILES tile)
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
			if (c == '\n')
			{
				++y;
				x = 0;
			}
			else if (c - '0' >= static_cast<int>(TILES::TILE_NONE) && c - '0' <= static_cast<int>(TILES::TILE_TOTAL))
			{
				mapGrid[x][y] = static_cast<Map::TILES>(c - '0');
				++x;
			}
		}
	fin.close();
}
