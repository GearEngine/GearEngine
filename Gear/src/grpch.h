#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <any>
#include <set>
#include <bitset>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <random>

#include "Gear/Core/Log.h"
#include "Gear/Core/Util.h"
#include "Gear/Debug/Instrumentor.h"
#include "Gear/Core/Timestep.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "../vendor/Json/include/json.h"

#ifdef GR_PLATFORM_WINDOWS
	#include <Windows.h>
#endif