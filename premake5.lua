workspace "Steins"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
	}
project "Steins"
	location "Steins"
	kind "StaticLib"
	language "C++"
	targetdir ("Build/Bin/%{prj.name}")
	objdir ("Build/Intermediate/%{prj.name}")
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}
	includedirs
	{
	}
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"STEINS_PLATFORM_WINDOWS",
			"STEINS_BUILD_DLL",
		}
		postbuildcommands
		{
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
	targetdir ("Build/Bin/%{prj.name}")
	objdir ("Build/Intermediate/%{prj.name}")
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}
	includedirs
	{
		"Steins/Source"
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
