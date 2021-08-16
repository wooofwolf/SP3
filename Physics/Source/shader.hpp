//#ifndef SHADER_HPP
//#define SHADER_HPP
//
//GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
//
//#endif

#pragma once
#include "SingletonPattern.h"

unsigned LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

class Shader : public SingletonPattern<Shader>
{
public:
	unsigned shaderdata;
	Shader() { shaderdata = LoadShaders("Shader//comg.vertexshader", "Shader//comg.fragmentshader"); };
	~Shader() {};
private:
};