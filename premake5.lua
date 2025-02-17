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
	IncludeDir["ImGui"] = "Steins/Vendor/imgui"

	include "Steins/Vendor/glfw"
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
		"%{prj.name}/Source/**.cpp"
	}
	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}"
	}
	links
	{
		"ImGui",
		"GLFW",
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"STEINS_PLATFORM_WINDOWS",
			"STEINS_BUILD_DLL",
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
