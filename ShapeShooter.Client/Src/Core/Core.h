#ifndef GAME_CORE_H
#define GAME_CORE_H


#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <chrono>

#include <array>
#include <vector>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>

#include "Camera.h"
#include "Input.h"


#include "Graphics/Texture.h"
#include "Graphics/Renderer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Shader.h"
#include "Graphics/Frustum.h"

#include <Util/Timer.h>

#include <imgui/imgui.h>

#endif