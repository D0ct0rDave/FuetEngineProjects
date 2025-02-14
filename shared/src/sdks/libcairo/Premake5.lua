-- Premake5.lua

workspace "libcairo"
    configurations { "Debug", "Release" }
    location "build" -- Where generated files (like Visual Studio solutions) will be stored
    architecture "x86_64"

project "libcairo"
    kind "StaticLib" -- Change to "SharedLib" for a shared library
    language "C++"
    cppdialect "C++17"
    targetdir "$(ProjectDir)/../lib/x64/%{cfg.buildcfg}" -- Output directory for binaries
    objdir "$(ProjectDir)obj/x64/%{cfg.buildcfg}" -- Output directory for intermediate files
	characterset("ASCII")
	sourceDir = "$(ProjectDir)../src"

	-- Specify the root directory of the library
    local sourceRoot = os.getcwd() .. "/src"

    -- Recursively include all .cpp and .h files from the sourceRoot directory
    files {
        "src/**.c",
        "src/**.h",
		"libpng/**.c",
		"libpng/**.h",
		"pixman/**.c",
		"pixman/**.h",
		"zlib/**.c",
		"zlib/**.h",
    }

	-- specific defines for this project
	defines {
		"LIBCAIRO_EXPORTS__"
	}
	
	filter { "system:windows" }
		defines { 
			"_MBCS" ,
			"WIN32",
			"WINDOWS",
			"_USRDLL__",
		}
		excludes {
			"src/cairo-atsui*",			
			"src/cairo-directfb*",
			"src/cairo-ft*",
			"src/cairo-glitz*",
			"src/cairo-os2*",
			"src/cairo-quartz*",
			"src/cairo-type1-subset*",
			
			"src/cairo-xcb*",
			"src/cairo-xlib*",
		}
	filter {} -- Reset filter

    -- Add include directories (sourceRoot is included by default)
    includedirs {
		"$(ProjectDir)../src",
		"$(ProjectDir)../pixman/src",
		"$(FuetEngineProjectsDev)/shared/src/sdks/externals/FreeImage/Source/zlib",
		"$(FuetEngineProjectsDev)/shared/src/sdks/externals/FreeImage/Source/LibPng",
    }

    -- Configuration-specific settings
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On" -- Generate debug symbols

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On" -- Enable optimizations

	filter {} -- Clear filter for general settings
