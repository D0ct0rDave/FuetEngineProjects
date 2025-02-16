-- Premake5.lua

workspace "tinyxml"
    configurations { "Debug", "Release" }
    platforms { "Win32", "x64" }	
    location "build" -- Where generated files (like Visual Studio solutions) will be stored

	filter {"platforms:Win32"}
	architecture "x86"
	filter {"platforms:x64"}
	architecture "x86_64"
	filter {}

project "tinyxml"
    kind "StaticLib" -- Change to "SharedLib" for a shared library
    language "C++"
    cppdialect "C++17"
    targetdir "$(ProjectDir)/../lib/%{cfg.platform}/%{cfg.buildcfg}" -- Output directory for binaries
    objdir "$(ProjectDir)obj/%{cfg.platform}/%{cfg.buildcfg}" -- Output directory for intermediate files
	characterset("ASCII")
	sourceDir = "$(ProjectDir)../src"

	-- Specify the root directory of the library
    local sourceRoot = os.getcwd() .. "/src"

    -- Recursively include all .cpp and .h files from the sourceRoot directory
    files {
        sourceRoot .. "/**.cpp",
        sourceRoot .. "/**.h"
    }

	-- specific defines for this project
	defines {
		"_MBCS" ,
		"_LIB"
	}
	
	filter { "system:windows" }
		defines { "WIN32" }
	filter {} -- Reset filter

    -- Add include directories (sourceRoot is included by default)
    includedirs {
		"$(ProjectDir)../src",
    }

    -- Configuration-specific settings
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On" -- Generate debug symbols

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On" -- Enable optimizations

	filter {} -- Clear filter for general settings

    -- List of directories to exclude from recursion
    local excludeDirs = { 
        "build",
    }
	
	includeDir = sourceDir .. "/../include"
