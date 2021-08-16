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
		GEO_CYLINDER,
		GEO_BACKGROUND,
		GEO_BASEENEMY,
		GEO_WIN_1,
		GEO_WIN_2,

		NUM_GEOMETRY,
	};

	MeshList();
	~MeshList();

	Mesh* GetMesh(GEOMETRY_TYPE geometryType);

private:
	Mesh* meshlist[NUM_GEOMETRY];
};

#endif