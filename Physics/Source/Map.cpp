#include "Map.h"

#include <iostream>
#include <string>
#include <fstream>

#include "Application.h"
#include "GL/glew.h"
#include "Camera.h"
#include "SceneBase.h"
#include "Player.h"
using namespace std::placeholders;
Map::Map() : mapGrid{TILES::TILE_NONE}
{
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	activeTileList.clear();
}

Map::~Map()
{
	// Delete AStar lists
	DeleteAStarLists();
}

void Map::Init(std::string fileName)
{
	Map::GetInstance()->ReadFile(fileName);
	m_startPos = Vector3(0, 0,0);
	m_targetPos = Vector3(0, 0,0);
	//m_size = cSettings->NUM_TILES_YAXIS* cSettings->NUM_TILES_XAXIS;

	m_nrOfDirections = 4;
	m_directions = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 },
						{ -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 } };

	// Resize these 2 lists
	m_cameFromList.resize(200*200);
	m_closedList.resize(200*200, false);
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

std::vector<Vector3> Map::PathFind(const Vector3& startPos, const Vector3& targetPos, HeuristicFunction heuristicFunc)
{
	// Check if the startPos and targetPost are blocked
	if (isBlocked(startPos.y, startPos.x) ||
		(isBlocked(targetPos.y, targetPos.x)))
	{
		std::cout << "Invalid start or target position." << std::endl;
		// Return an empty path
		std::vector<Vector3> path;
		return path;
	}

	// Set up the variables and lists
	m_startPos = startPos;
	m_targetPos = targetPos;

	m_heuristic = std::bind(heuristicFunc, _1, _2, _3);

	// Reset AStar lists
	ResetAStarLists();

	// Add the start pos to 2 lists
	m_cameFromList[ConvertTo1D(m_startPos)].parent = m_startPos;
	m_openList.push(Grid(m_startPos, 0));

	unsigned int fNew, gNew, hNew;
	Vector3 currentPos;

	// Start the path finding...
	while (!m_openList.empty())
	{
		// Get the node with the least f value
		currentPos = m_openList.top().pos;
		//cout << endl << "*** New position to check: " << currentPos.x << ", " << currentPos.y << endl;
		//cout << "*** targetPos: " << m_targetPos.x << ", " << m_targetPos.y << endl;

		// If the targetPos was reached, then quit this loop
		if (currentPos == m_targetPos)
		{
			//cout << "=== Found the targetPos: " << m_targetPos.x << ", " << m_targetPos.y << endl;
			while (m_openList.size() != 0)
				m_openList.pop();
			break;
		}

		m_openList.pop();
		m_closedList[ConvertTo1D(currentPos)] = true;

		// Check the neighbors of the current node
		for (unsigned int i = 0; i < m_nrOfDirections; ++i)
		{
			const auto neighborPos = currentPos + m_directions[i];
			const auto neighborIndex = ConvertTo1D(neighborPos);

			//cout << "\t#" << i << ": Check this: " << neighborPos.x << ", " << neighborPos.y << ":\t";
			if (!isValid(neighborPos) ||
				isBlocked(neighborPos.y, neighborPos.x) ||
				m_closedList[neighborIndex] == true)
			{
				//cout << "This position is not valid. Going to next neighbour." << endl;
				continue;
			}

			gNew = m_cameFromList[ConvertTo1D(currentPos)].g + 1;
			hNew = m_heuristic(neighborPos, m_targetPos , 10);
			fNew = gNew + hNew;

			if (m_cameFromList[neighborIndex].f == 0 || fNew < m_cameFromList[neighborIndex].f)
			{
				//cout << "Adding to Open List: " << neighborPos.x << ", " << neighborPos.y;
			/*	std::cout << ". [ f : " << fNew << ", g : " << gNew << ", h : " << hNew << "]" << std::endl;*/
				m_openList.push(Grid(neighborPos, fNew));
				m_cameFromList[neighborIndex] = { neighborPos, currentPos, fNew, gNew, hNew };
			}
			else
			{
				//cout << "Not adding this" << endl;
			}
		}
		//system("pause");
	}
	return BuildPath();
}

void Map::SetDiagonalMovement(const bool bEnable)
{
	m_nrOfDirections = (bEnable) ? 8 : 4;
}


void Map::PrintSelf(void) const
{
	//std::cout << std::endl << "AStar::PrintSelf()" << std::endl;
	for (unsigned uiRow = 0; uiRow < 200; uiRow++)
	{
		for (unsigned uiCol = 0; uiCol < 200; uiCol++)
		{
			std::cout.fill('0');
			std::cout.width(3);	
			std::cout << arrMapInfo[uiRow][uiCol].value;
			if (uiCol != 200 - 1)
				std::cout << ", ";
			else
				std::cout << std::endl;
		}
	}
	

	std::cout << "m_openList: " << m_openList.size() << std::endl;
	std::cout << "m_cameFromList: " << m_cameFromList.size() << std::endl;
	std::cout << "m_closedList: " << m_closedList.size() << std::endl;

	std::cout << "===== AStar::PrintSelf() =====" << std::endl;
}

std::vector<Vector3> Map::BuildPath() const
{
	std::vector<Vector3> path;
	auto currentPos = m_targetPos;
	auto currentIndex = ConvertTo1D(currentPos);

	while (!(m_cameFromList[currentIndex].parent == currentPos))
	{
		path.push_back(currentPos);
		currentPos = m_cameFromList[currentIndex].parent;
		currentIndex = ConvertTo1D(currentPos);
	}

	// If the path has only 1 entry, then it is the the target position
	if (path.size() == 1)
	{
		// if m_startPos is next to m_targetPos, then having 1 path point is OK
		if (m_nrOfDirections == 4)
		{
			if (abs(m_targetPos.y - m_startPos.y) + abs(m_targetPos.x - m_startPos.x) > 1)
				path.clear();
		}
		else
		{
			if (abs(m_targetPos.y - m_startPos.y) + abs(m_targetPos.x - m_startPos.x) > 2)
				path.clear();
			else if (abs(m_targetPos.y - m_startPos.y) + abs(m_targetPos.x - m_startPos.x) > 1)
				path.clear();
		}
	}
	else
		std::reverse(path.begin(), path.end());

	return path;
}

bool Map::isValid(const Vector3& pos) const
{
	return (pos.x >= 0) && (pos.x < 200) &&
		(pos.y >= 0) && (pos.y < 200);
}

bool Map::isBlocked(const unsigned int uiRow, const unsigned int uiCol, const bool bInvert) const
{
	if (bInvert == true)
	{
		if ((static_cast<int>(mapGrid[200 - uiRow - 1][uiCol]) == 1))
			return true;
		else
			return false;
	}
	else
	{
		if ((static_cast<int>(mapGrid[uiRow][uiCol]) == 1))
			return true;
		else
			return false;
	}
}

int Map::ConvertTo1D(const Vector3& pos) const
{
	//return (pos.y * m_dimensions.x) + pos.x;
	return (pos.y * 200) + pos.x;
}

bool Map::DeleteAStarLists(void)
{
	// Delete m_openList
	while (m_openList.size() != 0)
		m_openList.pop();
	// Delete m_cameFromList
	m_cameFromList.clear();
	// Delete m_closedList
	m_closedList.clear();

	return true;
}

bool Map::ResetAStarLists(void)
{
	// Delete m_openList
	while (m_openList.size() != 0)
		m_openList.pop();
	// Reset m_cameFromList
	for (int i = 0; i < m_cameFromList.size(); i++)
	{
		m_cameFromList[i].pos = Vector3(0, 0 , 0);
		m_cameFromList[i].parent = Vector3(0, 0 , 0);
		m_cameFromList[i].f = 0;
		m_cameFromList[i].g = 0;
		m_cameFromList[i].h = 0;
	}
	// Reset m_closedList
	for (int i = 0; i < m_closedList.size(); i++)
	{
		m_closedList[i] = false;
	}

	return true;
}

unsigned int heuristic::manhattan(const Vector3 v1, const Vector3& v2,int)
{
	Vector3 delta = v2 - v1;
	return static_cast<unsigned int>(delta.x + delta.y);
}

unsigned int heuristic::euclidean(const Vector3 v1, const Vector3& v2,int)
{
	Vector3 delta = v2 - v1;
	return static_cast<unsigned int>(sqrt((delta.x * delta.x) + (delta.y * delta.y)));
}
