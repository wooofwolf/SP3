#ifndef MESH_LIST_H
#define MESH_LIST_H

#include <string>
#include "Material.h"
#include "Mesh.h"
#include "SingletonPattern.h"

class MeshList : public SingletonPattern<MeshList>
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_BALL,
		GEO_CUBE,
		GEO_CUBE_RED,

		GEO_LEVEL1_WALL,
		GEO_LEVEL2_WALL,
		GEO_LEVEL3_WALL,
		
		GEO_LEVEL1_HAZARD,
		GEO_LEVEL2_HAZARD,
		GEO_LEVEL3_HAZARD,

		GEO_REVOLVER,
		GEO_SHOTGUN,
		GEO_MINIGUN,

		GEO_CUBE_GREEN,
		GEO_CYLINDER,
		GEO_BACKGROUND,
		GEO_CAVEMAN,
		GEO_SPEARMAN,
		GEO_KNIGHT,
		GEO_ARCHER,
		GEO_CYBORG,
		GEO_SNIPER,
		GEO_SPHERE_BLUE,
		GEO_WIN_1,
		GEO_WIN_2,
		GEO_HAZARD,
		GEO_EXIT,

		NUM_GEOMETRY,
	};

	MeshList();
	~MeshList();

	Mesh* GetMesh(GEOMETRY_TYPE geometryType);

private:
	Mesh* meshlist[NUM_GEOMETRY];
};
#endif