#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader-master/tiny_obj_loader.h>
#include <glm/gtx/hash.hpp>

#include <unordered_map>

#include "MRKVulkanTools.h"
#include "MRKModel.h"

// Needed for hashing vertices parsing for unique vertices using std::unordered_map, this looks like it should be replaced with a lamda
namespace std {
    template<> 
    struct hash<mrk::Model::Vertex> 
    {
        size_t operator()(mrk::Model::Vertex const& vertex) const 
        {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

namespace mrk
{

std::vector<Model::Vertex> Model::vertices;
std::vector<uint32_t> Model::indices;

void Model::load(char const* modelPath, char const* texturePath) 
{
    mTexturePath = texturePath;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    /*
    * An OBJ file consists of positions, normals, texture coordinates and faces.
    *
    * Faces consist of an arbitrary amount of vertices, where each vertex refers to a position, normal and/or texture coordinate
    * by index. This makes it possible to not just reuse entire vertices, but also individual attributes.
    *
    * The attrib container holds all of the positions, normals and texture coordinates in its attrib.vertices,
    * attrib.normals and attrib.texcoords vectors. The shapes container contains all of the separate objects and their faces.
    * Each face consists of an array of vertices, and each vertex contains the indices of the position,
    * normal and texture coordinate attributes. OBJ models can also define a material and texture per face, but we
    * will be ignoring those.
    *
    * The err string contains errors and warnings that occurred while loading the file, like a missing material definition.
    * Loading only really failed if the LoadObj function returns false. As mentioned above, faces in OBJ files can
    * actually contain an arbitrary number of vertices, whereas our application can only render triangles. Luckily
    * the LoadObj has an optional parameter to automatically triangulate such faces, which is enabled by default.
    */
    if (tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath) == false)
    {
        throw_line(err.c_str())
    }

    std::unordered_map<Model::Vertex, uint32_t> uniqueVertices = {};
#ifdef _DEBUG
    uniqueVertices.reserve(1000000);
    vertices.reserve(500000);
    indices.reserve(500000);
#endif

    for (auto const & shape : shapes)
    {
        for (const auto & index : shape.mesh.indices)
        {
            Vertex vertex = {};

            vertex.pos =
            {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                // The problem is that the origin of texture coordinates in Vulkan is the 
                // top-left corner, whereas the OBJ format assumes the bottom-left corner. 
                // Solve this by flipping the vertical component of the texture coordinates:
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

	vk::VertexInputBindingDescription Model::Vertex::getBindingDescription()
	{
		auto bindingDescription = vk::VertexInputBindingDescription()
			.setBinding(0)
			.setStride(sizeof(Vertex))
			.setInputRate(vk::VertexInputRate::eVertex);

		return bindingDescription;
	}

	std::array<vk::VertexInputAttributeDescription, 3> Model::Vertex::getAttributeDescriptions()
	{
		std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = vk::Format::eR32G32B32Sfloat;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}

	bool Model::Vertex::operator==(const Vertex& other) const
{
    return pos == other.pos && color == other.color && texCoord == other.texCoord;
}

}

