workspace "Gear"
	architecture "x64"
	startproject "Worms"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--include directories relative to root folder(solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Gear/vendor/GLFW/include"
IncludeDir["Glad"] = "Gear/vendor/Glad/include"
IncludeDir["ImGui"] = "Gear/vendor/imgui"
IncludeDir["glm"] = "Gear/vendor/glm"
IncludeDir["stb_image"] = "Gear/vendor/stb_image"
IncludeDir["fmod"] = "Gear/vendor/fmod/include"
IncludeDir["Json"] = "Gear/vendor/Json/include"

include "Gear/vendor/Glad"
include "Gear/vendor/GLFW"
include "Gear/vendor/ImGui"

project "Gear"
	location "Gear"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "grpch.h"
	pchsource "Gear/src/grpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/fmod/include/**.h",
		"%{prj.name}/vendor/fmod/include/**.h",
		"%{prj.name}/vendor/Json/include/json.h"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/KSNetwork/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.fmod}",
		"%{IncludeDir.Json}"
	}
	libdirs
	{
		"%{prj.name}/vendor/fmod/lib"
	}
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"fmod64_vc.lib",
		"jsoncpp.lib",
		"KSNetwork.lib",
		"ws2_32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"GR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "GR_DEBUG"
		runtime "Debug"
		symbols "on"
		libdirs
		{
			"%{prj.name}/vendor/Json/lib/Debug",
			"%{prj.name}/vendor/KSNetwork/lib/Debug"
		}
	
	filter "configurations:Release"
		defines "GR_RELEASE"
		runtime "Release"
		optimize "on"
		libdirs
		{
			"%{prj.name}/vendor/Json/lib/Release",
			"%{prj.name}/vendor/KSNetwork/lib/Release"
		}
	
	filter "configurations:Dist"
		defines "GR_DIST"
		runtime "Release"
		optimize "on"
		libdirs
		{
			"%{prj.name}/vendor/Json/lib/Release",
			"%{prj.name}/vendor/KSNetwork/lib/Release"
		}



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Gear/vendor/spdlog/include",
		"Gear/src",
		"Gear/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.fmod}"
	}
	links
	{
		"Gear"
	}
	

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "GR_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "GR_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "GR_DIST"
		runtime "Release"
		optimize "on"

project "Worms"
	location "Worms"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "wmpch.h"
	pchsource "Worms/src/wmpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Gear/vendor/spdlog/include",
		"Gear/src",
		"%{prj.name}/src",
		"Gear/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.fmod}",
		"%{IncludeDir.Json}"
	}
	links
	{
		"Gear"
	}
	

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "GR_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "GR_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "GR_DIST"
		runtime "Release"
		optimize "on"

project "WormsServer"
	location "WormsServer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "wsvpch.h"
	pchsource "WormsSever/src/wsvpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	links
	{
		"KSNetwork.lib",
		"ws2_32.lib"
	}
	

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		libdirs
		{
			"Gear/vendor/KSNetwork/lib/Debug"
		}
	
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		libdirs
		{
			"Gear/vendor/KSNetwork/lib/Release"
		}