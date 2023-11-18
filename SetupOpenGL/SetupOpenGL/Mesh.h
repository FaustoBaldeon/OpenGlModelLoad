#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
};

class Mesh
{
	public:
		//mesh data
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, 
			const std::vector<Texture> &textures);

		void Draw(const Shader &shader);

	private:
		//render data
		unsigned int vao, vbo, ebo;

		void setupMesh();
};

