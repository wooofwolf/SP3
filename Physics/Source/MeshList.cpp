#include "GL\glew.h"

#include "MeshList.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

#include "Vertex.h"

MeshList::MeshList()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshlist[i] = NULL;
	}
	meshlist[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshlist[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 1, 0), 10, 10, 1.f);
	meshlist[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 2.f);
	meshlist[GEO_CUBE_RED] = MeshBuilder::GenerateQuad("player", Color(1, 1, 1), 2.f);
	meshlist[GEO_CUBE_RED]->textureID = LoadTGA("Image//player.tga");	
	
	meshlist[GEO_REVOLVER] = MeshBuilder::GenerateQuad("revolver", Color(1, 1, 1), 2.f);
	meshlist[GEO_REVOLVER]->textureID = LoadTGA("Image//revolver.tga");	
	meshlist[GEO_SHOTGUN] = MeshBuilder::GenerateQuad("shotgun", Color(1, 1, 1), 2.f);
	meshlist[GEO_SHOTGUN]->textureID = LoadTGA("Image//shotgun.tga");	
	meshlist[GEO_MINIGUN] = MeshBuilder::GenerateQuad("minigun", Color(1, 1, 1), 4.f);
	meshlist[GEO_MINIGUN]->textureID = LoadTGA("Image//minigun.tga");


	meshlist[GEO_CUBE_GREEN] = MeshBuilder::GenerateCube("cube3", Color(0, 1, 0), 2.f);
	meshlist[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(1, 1, 1), 8, 1.f, 1.f);
	meshlist[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshlist[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshlist[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshlist[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("background", Color(1, 1, 1), 200.f);
	//meshlist[GEO_BACKGROUND]->textureID = LoadTGA("Image//menu-wallpaper.tga");
	meshlist[GEO_WIN_1] = MeshBuilder::GenerateQuad("win-1", Color(1, 1, 1), 200.f);
	//meshlist[GEO_WIN_1]->textureID = LoadTGA("Image//left-wins.tga");
	meshlist[GEO_WIN_2] = MeshBuilder::GenerateQuad("win-2", Color(1, 1, 1), 200.f);
	//meshlist[GEO_WIN_2]->textureID = LoadTGA("Image//right-wins.tga");

	meshlist[GEO_CAVEMAN] = MeshBuilder::GenerateQuad("BaseEnemy/Caveman", Color(1, 1, 1), 2.f);
	meshlist[GEO_CAVEMAN]->textureID = LoadTGA("Image//cavemanMelee.tga");
	meshlist[GEO_SPEARMAN] = MeshBuilder::GenerateQuad("BaseEnemy/Spearman", Color(1, 1, 1), 2.f);
	meshlist[GEO_SPEARMAN]->textureID = LoadTGA("Image//cavemanRanged.tga");
	
	meshlist[GEO_KNIGHT] = MeshBuilder::GenerateCube("BaseEnemy/Caveman", Color(1, 1, 0), 2.f);
	meshlist[GEO_ARCHER] = MeshBuilder::GenerateCube("BaseEnemy/Archer", Color(0, 1, 0), 2.f);
	
	meshlist[GEO_CYBORG]= MeshBuilder::GenerateCube("BaseEnemy/Caveman", Color(1, 1, 0), 2.f);
	meshlist[GEO_SNIPER] = MeshBuilder::GenerateCube("BaseEnemy/Archer", Color(0, 1, 1), 2.f);
	
	meshlist[GEO_SPHERE_BLUE] = MeshBuilder::GenerateSphere("Explosion", Color(175.f / 255.f, 238.f / 255.f, 238 / 255.f), 10, 10, 1.f);
	
	meshlist[GEO_HAZARD] = MeshBuilder::GenerateCube("hazard", Color(1, 0, 1), 2.f);
	meshlist[GEO_EXIT] = MeshBuilder::GenerateCube("exit", Color(1, 1, 0), 2.f);

}

MeshList::~MeshList()
{
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		delete meshlist[i];
	}
}

Mesh* MeshList::GetMesh(GEOMETRY_TYPE geometryType)
{
	return meshlist[geometryType];
}