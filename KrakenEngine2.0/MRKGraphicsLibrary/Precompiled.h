#pragma once

/* Precompiled header. */

//#pragma warning(disable : 4996)


//#define STB_DEFINE 
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <cassert>

#include <string>
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <bitset>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iterator>
#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

#define GLFW_INCLUDE_VULKAN
#define GLFW_DLL
#include "GLFW/glfw3.h"

#define VULKAN_HPP_TYPESAFE_CONVERSION
#include "vulkan/vulkan.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/hash.hpp"

#include "assimp/material.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
