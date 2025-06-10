workspace "Steins"
	architecture "x64"
	startproject "Sandbox"
	
	configurations
	{
		"Debug",
		"Release",
	}

	IncludeDir = {}
	IncludeDir["GLFW"] = "Steins/Vendor/glfw/include"
	IncludeDir["glad"] = "Steins/Vendor/glad/include"
	IncludeDir["ImGui"] = "Steins/Vendor/imgui"
	IncludeDir["glm"] = "Steins/Vendor/glm"
	IncludeDir["stb"] = "Steins/Vendor/stb"

	include "Steins/Vendor/glfw"
	include "Steins/Vendor/glad"
	include "Steins/Vendor/imgui"

project "Steins"
	location "Steins"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
	objdir ("Build/Intermediate/%{cfg.buildcfg}/%{prj.name}")

	pchheader "SteinsPCH.h"
	pchsource "%{prj.name}/Source/SteinsPCH.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Vendor/stb/**.h",
		"%{prj.name}/Vendor/stb/**.cpp",
		"%{prj.name}/Vendor/glm/glm/**.hpp",
 		"%{prj.name}/Vendor/glm/glm/**.inl"
	}
	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"$(VULKAN_SDK)/Include"
	}
	libdirs 
	{
        "$(VULKAN_SDK)/Lib"
    }
	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib",
		"vulkan-1.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"STEINS_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "STEINS_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "STEINS_RELEASE"
		symbols "On"
	
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
	objdir ("Build/Intermediate/%{cfg.buildcfg}/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}
	includedirs
	{
		"Steins/Vendor/spdlog/include",
		"Steins/Source",
		"Steins/Vendor",
		"%{IncludeDir.glm}"
	}
	links
	{
		"Steins"
	}
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"STEINS_PLATFORM_WINDOWS",
		}
	filter "configurations:Debug"
		defines "STEINS_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "STEINS_RELEASE"
		symbols "On"

project "SteinsGate"
	location "SteinsGate"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
	objdir ("Build/Intermediate/%{cfg.buildcfg}/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}
	includedirs
	{
		"Steins/Vendor/spdlog/include",
		"Steins/Source",
		"Steins/Vendor",
		"%{IncludeDir.glm}"
	}
	links
	{
		"Steins"
	}
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"STEINS_PLATFORM_WINDOWS",
		}
	filter "configurations:Debug"
		defines "STEINS_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "STEINS_RELEASE"
		symbols "On"
