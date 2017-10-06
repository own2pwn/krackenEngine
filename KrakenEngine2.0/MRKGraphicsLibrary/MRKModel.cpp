#include "Precompiled.h"
#include "MRKVulkanTools.h"
#include "MRKModel.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#define ASSIM

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader-master/tiny_obj_loader.h>

// Needed for hashing vertices parsing for unique vertices using std::unordered_map, this looks like it should be replaced with a lamda
namespace std
{
	template<>
	struct hash<mrk::Vertex>
	{
		size_t operator()(mrk::Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

namespace mrk
{
	void Model::load(char const* modelPath)
	{
		std::unordered_map<mrk::Vertex, uint32_t> uniqueVertices = {};

#ifdef ASSIM
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(modelPath, aiProcessPreset_TargetRealtime_Fast);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			throw_line(importer.GetErrorString())
		}

		processNode(scene->mRootNode, scene);

#else
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
#endif
	}

	void Model::processNode(aiNode const * node, aiScene const * scene)
	{
		for (unsigned i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh const* mesh, aiScene const* scene)
	{
		Mesh processedMesh;

		for (unsigned i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex = {};

			// process vertex positions
			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;

			// normals
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			//textures
			if(mesh->mTextureCoords[0])
			{
				vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoord.y = 1.0f - mesh->mTextureCoords[0][i].y;
			}

			processedMesh.vertices.push_back(vertex);
		}

		// process indices
		for (unsigned i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace const face = mesh->mFaces[i];
			for (unsigned j = 0; j < face.mNumIndices; ++j)
			{
				processedMesh.indices.push_back(face.mIndices[j]);
			}
		}

		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			processedMesh.textures.insert(processedMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			processedMesh.textures.insert(processedMesh.textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return processedMesh;
	}

	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		aiString dir("Assets/textures/");
		std::vector<Texture> textures;
bool Model::Vertex::operator==(const Vertex& other) const
{
    return (pos == other.pos) && (color == other.color) && (texCoord == other.texCoord);
}

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			dir.Append(str.C_Str());

			bool skip = false;

			for (unsigned int j = 0; j < loadedTextures.size(); j++)
			{
				if (std::strcmp(loadedTextures[j].path.C_Str(), dir.C_Str()) == 0)
				{
					textures.push_back(loadedTextures[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Texture texture;
				texture.type = typeName;
				texture.path = dir;
				textures.push_back(texture);
				loadedTextures.push_back(texture); // add to loaded textures
			}
		}

		return textures;
	}
}

