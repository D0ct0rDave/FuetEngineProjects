-- Linux build of the 2014 FuetEngine (premake5 gmake).  Windows/consoles keep using build/vs20xx.
-- Usage:  premake5 --file=Premake5.lua gmake && make -C build_linux config=debug_x64 -j$(nproc)
local DepsRoot = os.getcwd() .. "/../../../toolchain/FuetEngine/linux-deps"

workspace "FuetEngine"
    configurations { "Debug", "Release", "Asan" }
    platforms { "x64" }
    location "build_linux"
    architecture "x86_64"

project "FuetEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "gnu++14"
    targetdir(os.getcwd() .. "/lib/%{cfg.platform}/%{cfg.buildcfg}")
    objdir(os.getcwd() .. "/obj/%{cfg.platform}/%{cfg.buildcfg}")
    local sourceRoot = os.getcwd() .. "/src"

    files { sourceRoot .. "/**.cpp", sourceRoot .. "/**.h", sourceRoot .. "/**.inl" }
    excludes {
        "**/CFECPlatformCore_Template.cpp",
        "**/CFERenderer_VBO.cpp",
        "**/CFEMem_SelfManagement.cpp",
        "**/CFEString_.cpp",
        "**/CFECWindowsOpenGL_VX=3floats.cpp",
        "**/Windows/**", "**/Cocos2D_X/**", "**/Android/**",
        "**/MacOS/**",
    }

    defines { "LINUX", "ALUT_BUILD_STATIC" }
    buildoptions { "-fPIC", "-fpermissive", "-w" }
    -- MSVC secure CRT functions (sprintf_s, fopen_s...) missing in glibc
    forceincludes { sourceRoot .. "/core/Linux/FECLinuxCompat.h" }
    includedirs { sourceRoot, DepsRoot .. "/include", DepsRoot .. "/include/AL" }

    filter "configurations:Debug or configurations:Asan"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Asan"
        -- AddressSanitizer build to hunt memory errors (link the application with -fsanitize=address too)
        buildoptions { "-fsanitize=address", "-fno-omit-frame-pointer" }
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
