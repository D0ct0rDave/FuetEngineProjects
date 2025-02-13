local ProjectRelativeSDKSRoot = "$(ProjectDir)../../../SDKS"
local ProjectRelativeFuetEngineRoot = "$(ProjectDir)../../FuetEngine"
	
workspace "FuetEngine"
    configurations { "DEV_Debug", "DEV_Release", "DEV_WITH_SOUND_Debug", "DS_PC_Debug", "PLAYTEST_Debug", "PLAYTEST_Release", "WIN32_Debug", "WIN32_Release"}
    location "build" -- Where generated files (like Visual Studio solutions) will be stored
    architecture "x86_64"
	
	currentPlatform = "Win32"
	filter { "architecture:x86_64" }
		currentPlatform = "x64"
	filter {} -- Reset filter

project "FuetEngine"
    kind "StaticLib" -- Change to "SharedLib" for a shared library
    language "C++"
    cppdialect "C++17"
    targetdir("$(ProjectDir)../lib/" .. currentPlatform .. "/%{cfg.buildcfg}") -- Output directory for binaries
    objdir("$(ProjectDir)../obj/" .. currentPlatform .. "/%{cfg.buildcfg}") -- Output directory for intermediate files
	characterset("ASCII")
	sourceDir = "$(ProjectDir)../src"

	-- Specify the root directory of the library
    local sourceRoot = os.getcwd() .. "/src"

    -- Recursively include all .cpp and .h files from the sourceRoot directory
    files {
        sourceRoot .. "/**.cpp",
        sourceRoot .. "/**.inl",		
        sourceRoot .. "/**.h"
    }
	
	excludes {
		sourceRoot .. "/**CFECPlatformCore_Template.cpp",
		sourceRoot .. "/**CFERenderer_VBO.cpp",
		sourceRoot .. "/**CFEMem_SelfManagement.cpp",
		sourceRoot .. "/**CFECWindowsOpenGL_VX=3floats.cpp",
	}
	
	filter { "system:windows" }
		excludes { 
			"**/Linux/**",
			"**/Cocos2D_X/**",
			"**/Android/**",
		}

	filter {} -- Reset filter


	-- specific defines for this project
	defines {
		"_MBCS" ,
		"NULL=0",
		"ALUT_BUILD_STATIC",		
	}
	
	filter { "system:windows" }
		defines { 
			"WIN32", 
			"_WIN32",
			"_WINDOWS",
		}
	filter {} -- Reset filter

    -- Add include directories (sourceRoot is included by default)
    includedirs {
		"$(ProjectDir)../src",
		ProjectRelativeSDKSRoot .. "/externals/freeimage/Dist",
		ProjectRelativeSDKSRoot .. "/externals/libconfigPortable/src",
		ProjectRelativeSDKSRoot .. "/externals/OpenAL_1.1_SDK/include",
		ProjectRelativeSDKSRoot .. "/externals/OpenAL_1.1_SDK/include/al",
		ProjectRelativeSDKSRoot .. "/externals/freealut/include",
		ProjectRelativeSDKSRoot .. "/externals/physfs/",
		ProjectRelativeSDKSRoot .. "/externals/OpenGL",
    }

    -- Configuration-specific settings
    filter "configurations:*Debug"
        defines { "DEBUG" }
        symbols "On" -- Generate debug symbols
		staticruntime "off"
		runtime "Debug"
	filter {}
	
    filter "configurations:*Release"
        defines { "NDEBUG" }
        optimize "On" -- Enable optimizations
		staticruntime "off"
		runtime "Release"
	filter {}
	
	filter "configurations:DEV_Debug or configurations:DEV_Release or configurations:DS_PC_Debug"
        defines { "DEVELOPMENT" }
	filter {}
	
	filter "configurations:DEV_WITH_SOUND_Debug"
        defines { "DEVELOPMENT_WITH_SOUND" }
	filter {}
	
	filter "configurations:PLAYTEST_*"
        defines { "PLAYTEST" }
	filter {}

