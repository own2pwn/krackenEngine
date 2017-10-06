#pragma once
#include "Precompiled.h"

//predeclared
struct aiScene;
struct aiNode;
struct aiMesh;
struct aiString;

	

namespace mrk
{
	// vertex
	class Vertex
	{
	public:
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;

		static vk::VertexInputBindingDescription getBindingDescription()
		{
			auto bindingDescription = vk::VertexInputBindingDescription()
				.setBinding(0)
				.setStride(sizeof(Vertex))
				.setInputRate(vk::VertexInputRate::eVertex);

			return bindingDescription;
		}

		static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions()
		{
			std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			//attributeDescriptions[1].binding = 0;
			//attributeDescriptions[1].location = 1;
			//attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
			//attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
			attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}

		bool operator==(const Vertex& other) const
		{
			return pos == other.pos && texCoord == other.texCoord;
		}
	};

	// texture
	class Texture
	{
	public:
		aiString path;
		std::string type;
	};

	// mesh
	class Mesh
	{
	public:
		Mesh()
		{
		}

		Mesh(std::vector<Vertex> const & verts, std::vector<uint32_t> const & inds)
		{
			vertices = verts;
			indices = inds;
		}

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices; // The "uint32_t" limits or raises the number of total possible indices
		std::vector<Texture> textures;
	};

	// model
class Model
{
public:
	std::vector<Mesh> meshes;

    void load(char const * modelPath);
    const char * mTexturePath;

    // JUAN IDEALLY THESE SHOULD BE ARRAYS IN THE FUTURE NOT VECTORS
    // It is important to contain all the data in only 2 arrays even when we use different models we can create our own
    // indice system to differentiate them. That will be the actual data that merits a model class
private:
	void processNode(aiNode const * node, aiScene const * scene);
	Mesh processMesh(aiMesh const * mesh, aiScene const * scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
public:
	std::vector<Texture> loadedTextures;
};

}
