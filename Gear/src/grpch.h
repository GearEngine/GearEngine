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
#include <unordered_map>
#include <unordered_set>

#include "Gear/Core/Log.h"
#include "Gear/Debug/Instrumentor.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "box2d.h"

#ifdef GR_PLATFORM_WINDOWS
	#include <Windows.h>
#endif