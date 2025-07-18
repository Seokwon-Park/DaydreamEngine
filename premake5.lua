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
	IncludeDir["assimp"] = "Steins/Vendor/assimp/include"

	include "Steins/Vendor/glfw"
	include "Steins/Vendor/glad"
	include "Steins/Vendor/imgui"
	
project "Steins"
	location "Steins"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
 		"%{prj.name}/Vendor/glm/glm/**.inl",
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
		"%{IncludeDir.assimp}",
		"$(VULKAN_SDK)/Include"
	}
	libdirs 
	{
        "$(VULKAN_SDK)/Lib",
		"Steins/Vendor/assimp/bin/%{cfg.buildcfg}"
    }
	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib",
		"vulkan-1.lib",
	}

	defines
	{
		"GLM_FORCE_LEFT_HANDED",
		"GLM_FORCE_DEPTH_ZERO_TO_ONE"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		defines
		{
			"STEINS_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}
		flags { "MultiProcessorCompile" }


	filter "configurations:Debug"
		defines "STEINS_DEBUG"
		symbols "On"
		runtime "Debug"
		links 
		{
			"spirv-cross-cd.lib",
			"spirv-cross-cored.lib",
			"spirv-cross-cppd.lib",
			"spirv-cross-glsld.lib",
			"spirv-cross-hlsld.lib",
			"spirv-cross-msld.lib",
			"spirv-cross-reflectd.lib",
			"spirv-cross-utild.lib",
            "assimp-vc143-mtd.lib" -- Assimp Debug 버전 라이브러리 이름 (필요 시 주석 해제)
        }
	filter "configurations:Release"
		defines "STEINS_RELEASE"
		symbols "On"
		runtime "Release"
		links 
		{
			"spirv-cross-c.lib",
			"spirv-cross-core.lib",
			"spirv-cross-cpp.lib",
			"spirv-cross-glsl.lib",
			"spirv-cross-hlsl.lib",
			"spirv-cross-msl.lib",
			"spirv-cross-reflect.lib",
			"spirv-cross-util.lib",
            "assimp-vc143-mt.lib" -- Assimp Debug 버전 라이브러리 이름 (필요 시 주석 해제)
        }
	
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		systemversion "latest"
		defines
		{
			"STEINS_PLATFORM_WINDOWS",
		}
		flags { "MultiProcessorCompile" }
	filter "configurations:Debug"
		defines "STEINS_DEBUG"
		symbols "On"
		runtime "Debug"
	filter "configurations:Release"
		defines "STEINS_RELEASE"
		symbols "On"
		runtime "Release"

project "SteinsGate"
	location "SteinsGate"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		systemversion "latest"
		defines
		{
			"STEINS_PLATFORM_WINDOWS",
		}
		flags { "MultiProcessorCompile" }
	filter "configurations:Debug"
		defines "STEINS_DEBUG"
		symbols "On"
		runtime "Debug"
	filter "configurations:Release"
		defines "STEINS_RELEASE"
		symbols "On"
		runtime "Release"
