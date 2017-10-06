#pragma once
#include "Precompiled.h"

namespace mrk
{

class Model
{
public:
    class Vertex
    {
    public:
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static vk::VertexInputBindingDescription getBindingDescription();
        static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions();

        bool operator==(const Vertex& other) const;
    };

    void load(char const * modelPath, char const * texturePath);
    const char * mTexturePath;

    // JUAN IDEALLY THESE SHOULD BE ARRAYS IN THE FUTURE NOT VECTORS
    // It is important to contain all the data in only 2 arrays even when we use different models we can create our own
    // indice system to differentiate them. That will be the actual data that merits a model class
    static std::vector<Vertex> vertices;
    static std::vector<uint32_t> indices; // The "uint32_t" limits or raises the number of total possible indices
};

}
