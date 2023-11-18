#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
	public:
		unsigned int ID;

		Shader(const char* vertexCode, const char* fragmentCode);

		void use();

		void SetBool(const std::string &name, bool value)const;
		void SetInt(const std::string &name, int value)const;
		void SetFloat(const std::string &name, float value)const;
	
};

