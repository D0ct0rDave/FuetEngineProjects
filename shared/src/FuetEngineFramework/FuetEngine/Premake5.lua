local ProjectRelativeSDKSRoot = "$(ProjectDir)../../../SDKS"
local ProjectRelativeFuetEngineRoot = "$(ProjectDir)../../FuetEngine"
local SDKSRoot = "$(FuetEngineProjectsDev)/shared/src/sdks"
local FuetEngineRoot = "$(FuetEngineProjectsDev)/shared/src/FuetEngineFramework/FuetEngine"
	
workspace "FuetEngine"
    configurations { "DEV_Debug", "DEV_Release", "DEV_WITH_SOUND_Debug", "DS_PC_Debug", "PLAYTEST_Debug", "PLAYTEST_Release", "WIN32_Debug", "WIN32_Release"}
    platforms { "Win32", "x64" }
	location "build" -- Where generated files (like Visual Studio solutions) will be stored
    
	filter {"platforms:Win32"}
	architecture "x86"
	filter {"platforms:x64"}
	architecture "x86_64"
	filter {}

project "FuetEngine"
    kind "StaticLib" -- Change to "SharedLib" for a shared library
    language "C++"
    cppdialect "C++17"
    targetdir("$(ProjectDir)../lib/%{cfg.platform}/%{cfg.buildcfg}") -- Output directory for binaries
    objdir("$(ProjectDir)../obj/%{cfg.platform}/%{cfg.buildcfg}") -- Output directory for intermediate files
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
		SDKSRoot .. "/externals/freeimage/Dist",
		SDKSRoot .. "/externals/libconfigPortable/src",
		SDKSRoot .. "/externals/OpenAL_1.1_SDK/include",
		SDKSRoot .. "/externals/OpenAL_1.1_SDK/include/al",
		SDKSRoot .. "/externals/freealut/include",
		SDKSRoot .. "/externals/physfs/",
		SDKSRoot .. "/externals/OpenGL",
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

