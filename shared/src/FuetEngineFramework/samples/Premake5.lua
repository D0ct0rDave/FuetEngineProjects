------------------------------------------------------------------------------
-- Define the mapping of platforms to project file extensions
local platformProjectExtensions = 
{
	windows = "vcxproj",
	linux = "makefile",
	macosx = "xcodeproj"
}

-- Determine the current platform's project file extension
local currentPlatform = os.target():lower() -- e.g., "windows", "linux", "macosx"
local projectExtension = platformProjectExtensions[currentPlatform]

if not projectExtension then
	error("Unsupported platform: " .. currentPlatform)
end

------------------------------------------------------------------------------
local function includeLibraries(rootDir)
	local projectFiles = os.matchfiles(rootDir .. "/*." .. projectExtension)
	for _, projectFile in ipairs(projectFiles) do
		local projectName = projectFile:match("([^/\\]+)%..+$") -- Extract project name

		print("Adding library: " .. projectFile)
		links { projectName .. ".lib" }
		links { projectName }
	end
end

------------------------------------------------------------------------------
-- Step 2: Include external projects based on generated project files
local libraries= {} -- Collect all library names
local function includeExternalProjects(rootDir)

	local projectFiles = os.matchfiles(rootDir .. "/*." .. projectExtension)
	for _, projectFile in ipairs(projectFiles) do
		local projectName = projectFile:match("([^/\\]+)%..+$") -- Extract project name

		print("Adding external project: " .. projectFile)
		externalproject(projectName)
			location(rootDir)
			kind "StaticLib" -- Modify as needed
			language "C++"
	end
end

------------------------------------------------------------------------------

scriptRoot = os.getcwd()	
frameworkRoot = scriptRoot .. "/../FuetEngine"

------------------------------------------------------------------------------

local function addSampleProject(_projectName, _useWxWidgets)

	local ProjectRelativeFinalDataRoot = "$(ProjectDir)../data/PC"
	local ProjectRelativeSDKSRoot = "$(ProjectDir)../../../SDKS"
	local ProjectRelativeFuetEngineRoot = "$(ProjectDir)../../FuetEngine"

	project(_projectName)
		kind "WindowedApp" -- Change to "SharedLib" for a shared library
		language "C++"
		cppdialect "C++17"
		targetdir("$(ProjectDir)exe/" .. _projectName .. "$(Platform)/%{cfg.buildcfg}") -- Output directory for binaries
		objdir("$(ProjectDir)obj/" .. _projectName .. "$(Platform)/%{cfg.buildcfg}") -- Output directory for intermediate files
		characterset("ASCII")
		debugdir(ProjectRelativeFinalDataRoot)

		-- Recursively include all .cpp and .h files from the sourceRoot directory		
		sourceRoot = scriptRoot .. "/" .. _projectName
		
		-- Recursively include all .cpp and .h files from the sourceRoot directory
		files {
			sourceRoot .. "/src/**.cpp",
			sourceRoot .. "/src/**.h",
			sourceRoot .. "/src/**.txt",
		}

		-- specific defines for this project
		defines {
			"_MBCS",
			"_OGL_",
			"_OPENAL_",
			"FREEIMAGE_LIB",
		}
		
		filter { "system:windows" }
			defines { "WIN32" }
		filter {} -- Reset filter

		disablewarnings
		{
			"4091",
		}

		includedirs 
		{
			-- Add include directories (sourceRoot is included by default)
			sourceRoot,
			ProjectRelativeFuetEngineRoot .. "/src",
		}

		if _useWxWidgets then
			includedirs 
			{
				ProjectRelativeSDKSRoot .. "/Externals/wxWidgets2.8/include",
				ProjectRelativeSDKSRoot .. "/Externals/wxWidgets2.8/include/msvc",
			}			
		end

		-- Library directories common for all configurations
		libdirs
		{
			ProjectRelativeSDKSRoot .. "/externals/FreeImage/Dist/$(Platform)/%{cfg.buildcfg}",
			ProjectRelativeSDKSRoot .. "/externals/physfs/build/%{cfg.buildcfg}",
			ProjectRelativeSDKSRoot .. "/externals/libConfigPortable/lib/$(Platform)/%{cfg.buildcfg}",
			ProjectRelativeSDKSRoot .. "/externals/OpenAL_1.1_SDK/libs/Win64/",
			ProjectRelativeSDKSRoot .. "/externals/OpenAL_1.1_SDK/libs",			
			ProjectRelativeFuetEngineRoot .. "/lib/" .. "x64" .. "/WIN32_%{cfg.buildcfg}",
		}
		
		if _useWxWidgets then
			libdirs
			{
				ProjectRelativeSDKSRoot .. "/Externals/wxWidgets2.8/lib/vc_lib/$(Platform)",
			}
		end		

		links 
		{
			"ddraw.lib",
			"dxguid.lib",
			"opengl32.lib",
			"glu32.lib",
			"freeimage.lib",
			"libconfigPortable.lib",
			"OpenAL32.lib",
			"alut.lib",
			"physfs.lib",
		}

		-- Configuration-specific settings
		filter "configurations:Debug"
			defines { "DEBUG" }
			symbols "On" -- Generate debug symbols

			if _useWxWidgets then
				links 
				{
					"wxbase28d.lib",
					"wxbase28d_net.lib",
					"wxbase28d_odbc.lib",
					"wxbase28d_xml.lib",
					"wxmsw28d_adv.lib",
					"wxmsw28d_aui.lib",
					"wxmsw28d_core.lib",
					"wxmsw28d_dbgrid.lib",
					"wxmsw28d_gl.lib",
					"wxmsw28d_html.lib",
					"wxmsw28d_media.lib",
					"wxmsw28d_qa.lib",
					"wxmsw28d_richtext.lib",
					"wxmsw28d_xrc.lib",
					"wxexpatd.lib",
					"wxjpegd.lib",
					"wxpngd.lib",
					"wxregexd.lib",
					"wxtiffd.lib",
					"wxzlibd.lib",
				}
			end


		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On" -- Enable optimizations			

			if _useWxWidgets then
				links 
				{
					"wxmsw28_xrc.lib",
					"wxmsw28_dbgrid.lib",
					"wxmsw28_core.lib",
					"wxbase28.lib",
					"wxbase28_net.lib",
					"wxmsw28_adv.lib",
					"wxmsw28_html.lib",
					"wxbase28_odbc.lib",
					"wxmsw28_richtext.lib",
					"wxbase28_xml.lib",
					"wxmsw28_aui.lib",
					"wxmsw28_media.lib",
					"wxmsw28_qa.lib",
					"wxmsw28_gl.lib",
					"wxtiff.lib",
					"wxjpeg.lib",
					"wxregex.lib",
					"wxpng.lib",
					"wxzlib.lib",
					"wxexpat.lib",
				}
			end

		filter {} -- Clear filter for general settings

		-- Add the external project to the solution
		includeLibraries(frameworkRoot .. "/build/vs2022")
				
	
		links { "FuetEngine.lib" }
		
		--[[	
		links { scriptRoot .. "../FuetEngine/build/vs2022/FuetEngine.vcxproj" }
		]]--

	-- Install rules (using a post-build step for example purposes)
	project(_projectName) -- for some reason this is reset, so we need to setup it again
		print("Final data:" .. ProjectRelativeFinalDataRoot)
		postbuildcommands
		{
			-- "{MKDIR} %{wks.location}/dist/lib", -- Create output directory
			-- "{COPYFILE} " .. ProjectRelativeSDKSRoot .. "/Externals/FreeImage/Dist/$(Platform)/FreeImage.dll " .. ProjectRelativeFinalDataRoot
			-- "{COPYFILE} " .. ProjectRelativeSDKSRoot .. "/externals/OpenAL_1.1_SDK/libs/Win64/alut.dll " .. ProjectRelativeFinalDataRoot
		}
end

------------------------------------------------------------------------------
-- Premake5.lua
------------------------------------------------------------------------------
workspace "FuetEngine_Samples"
    configurations { "Debug", "Release" }
    location "build" -- Where generated files (like Visual Studio solutions) will be stored
    architecture "x86_64"

------------------------------------------------------------------------------
group("Samples")
	addSampleProject("2DisplaysSample", true)
	addSampleProject("fontsample", false)
	addSampleProject("FPS", false)
	addSampleProject("helloworld", false)
	addSampleProject("intro", false)
	addSampleProject("MeshSample", false)
	addSampleProject("padinput", false)
	addSampleProject("SoundSample", false)
	addSampleProject("sprites", false)
	addSampleProject("SpriteWithInput", false)

--[[
group("FuetEngine")	
	includeExternalProjects(frameworkRoot .. "/build/vs2022")
]]--
