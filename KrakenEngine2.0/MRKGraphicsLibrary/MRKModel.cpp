#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader-master/tiny_obj_loader.h>
#include <glm/gtx/hash.hpp>

#include "MRKVulkanTools.h"
#include "MRKModel.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

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
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			throw_line(importer.GetErrorString())
		}

		processNode(scene->mRootNode, scene);
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
		aiString path("Assets/textures/");
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			path.Append(str.C_Str());

			bool skip = false;

			for (unsigned int j = 0; j < loadedTextures.size(); ++j)
			{
				if (std::strcmp(loadedTextures[j].path.C_Str(), path.C_Str()) == 0)
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
				texture.path = path;
				textures.push_back(texture);
				loadedTextures.push_back(texture); // add to loaded textures
			}
		}

		return textures;
	}
}

