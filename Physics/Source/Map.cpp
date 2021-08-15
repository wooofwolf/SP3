#include "Map.h"

#include <iostream>
#include <string>
#include <fstream>

#include "GL/glew.h"

Map::Map() : mapGrid{0}
{
}

Map::~Map()
{
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
