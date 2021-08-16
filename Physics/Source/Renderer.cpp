#include "Renderer.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "Vertex.h"
#include <sstream>

Renderer::Renderer()
{
	//Load Vertex and fragment shaders
	Mtx44 projection;
	projection.SetToPerspective(45.f, 16.f / 9.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); //Set BG colour
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Shaders are loaded in shader.hpp

	//Parameters
	parameters[U_MVP] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "MVP");
	parameters[U_MODELVIEW] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "MV");
	parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "MV_inverse_transpose");
	parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "material.kAmbient");
	parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "material.kDiffuse");
	parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "material.kSpecular");
	parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "material.kShininess");

	//Mesh::SetMaterialLoc(Parameters[U_MATERIAL_AMBIENT], Parameters[U_MATERIAL_DIFFUSE], Parameters[U_MATERIAL_SPECULAR], Parameters[U_MATERIAL_SHININESS], Parameters[U_MATERIAL_MAPKD], Parameters[U_MATERIAL_MAPKD_ENABLED]);
	parameters[U_LIGHTENABLED] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "lightEnabled");
	parameters[U_NUMLIGHTS] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "numLights");

	// Get a handle for our "colorTexture" uniform
	//for (int i = 0; i < MAX_TEXTURES; i++)
	//{
	//	std::stringstream ss;
	//	ss.str("");
	//	ss << "colorTextureEnabled[" << i << "]";
	//	Parameters[U_COLOR_TEXTURE_ENABLED + i] = glGetUniformLocation(Shader::GetInstance()->shaderdata, ss.str().c_str());

	//	ss.str("");
	//	ss << "colorTexture[" << i << "]";
	//	Parameters[U_COLOR_TEXTURE + i] = glGetUniformLocation(Shader::GetInstance()->shaderdata, ss.str().c_str());
	//}

	// Get a handle for our "colorTexture" uniform
	parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "colorTextureEnabled");
	parameters[U_COLOR_TEXTURE] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "colorTexture");
	// Get a handle for our "textColor" uniform
	parameters[U_TEXT_ENABLED] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "textEnabled");
	parameters[U_TEXT_COLOR] = glGetUniformLocation(Shader::GetInstance()->shaderdata, "textColor");

	glUseProgram(Shader::GetInstance()->shaderdata);
	// Make sure you pass uniform parameters after glUseProgram()
	//glUniform1i(Parameters[U_NUMLIGHTS], numlight);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(Shader::GetInstance()->shaderdata);
}

void Renderer::Reset()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight)
	{
		glUniform1i(parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	//for (int i = 0; i < MAX_TEXTURES; ++i)
	//{
	//	if (mesh->textureArr[i] > 0)
	//	{
	//		glUniform1i(Parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
	//		glActiveTexture(GL_TEXTURE0 + i);
	//		glBindTexture(GL_TEXTURE_2D, mesh->textureArr[i]);
	//		glUniform1i(Parameters[U_COLOR_TEXTURE + i], i);
	//	}
	//	else
	//	{
	//		glUniform1i(Parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
	//	}
	//}

	mesh->Render();

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Renderer::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(parameters[U_LIGHTENABLED], 0);
	glUniform1i(parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(parameters[U_COLOR_TEXTURE], 0);

	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;

		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	glDisable(GL_DEPTH_TEST);

	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 128, 0, 72, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(parameters[U_LIGHTENABLED], 0);
	glUniform1i(parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(parameters[U_COLOR_TEXTURE], 0);

	//float accumulator = 0.f;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;

		//Change this line inside for loop
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); // Change 1.0f to change spacing between each character
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(parameters[U_TEXT_ENABLED], 0);

	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, float angle)
{
	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 128, 0, 72, -10, 10); //size of screen UI

	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly remove rotate for something else lul later
	modelStack.Translate(x, y, 0);
	modelStack.Rotate(angle, 0, 0, 1);
	modelStack.Scale(sizex, sizey, 0);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}