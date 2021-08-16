#include "Map.h"

#include <iostream>
#include <string>
#include <fstream>
#include "Application.h"
#include "GL/glew.h"

Map::Map() : mapGrid{0}
{
	SceneBase::Init();
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
}

Map::~Map()
{
}

void Map::RenderMap(std::string fileName)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	ReadFile(fileName);
	// Model matrix : an identity matrix (model will be at the origin)
	//modelStack.LoadIdentity();
	for (int y = 0; y < 200; ++y)
	{
		for (int x = 0; x < 200; ++x)
		{
			if(GetGrid(x,y) == 1)
			{
				modelStack.PushMatrix();
				modelStack.Translate(2 + 4 * x, m_worldHeight - 2 - 4 * y, 1);
				modelStack.Scale(2, 2, 2);
				RenderMesh(meshList[GEO_CUBE], true);
				modelStack.PopMatrix();
			}
		}
	}
}

int Map::GetGrid(int x, int y)
{
	return mapGrid[x][y];
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
			if (c == '\n')
			{
				++y;
				x = 0;
			}
			else if (c - '0' >= static_cast<int>(TILES::TILE_NONE) && c - '0' <= static_cast<int>(TILES::TILE_TOTAL))
			{
				mapGrid[x][y] = c - '0';
				++x;
			}
		}
	fin.close();
}
