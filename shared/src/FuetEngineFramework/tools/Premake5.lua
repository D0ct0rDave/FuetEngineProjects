------------------------------------------------------------------------------
-- Define the mapping of platforms to project file extensions
local systemProjectExtensions = 
{
	windows = "vcxproj",
	linux = "makefile",
	macosx = "xcodeproj"
}

-- Determine the current platform's project file extension
local currentSystem = os.target():lower() -- e.g., "windows", "linux", "macosx"
local projectExtension = systemProjectExtensions[currentSystem]

if not projectExtension then
	error("Unsupported system: " .. currentSystem)
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

local function addToolProject(_projectName, _useWxWidgets)

	local ProjectRelativeFinalDataRoot = "$(ProjectDir)../../../../toolchain/FuetEngine"
	local ProjectRelativeSDKSRoot = "$(ProjectDir)../../../SDKS"
	local ProjectRelativeFuetEngineRoot = "$(ProjectDir)../../FuetEngine"

	local currentPlatform = "Win32"
	filter { "architecture:x86_64" }
		currentPlatform = "x64"
	filter {} -- Reset filter

	project(_projectName)
		kind "WindowedApp" -- Change to "SharedLib" for a shared library
		language "C++"
		cppdialect "C++17"
		targetdir("$(ProjectDir)exe/" .. _projectName .. "/" .. currentPlatform .. "/%{cfg.buildcfg}") -- Output directory for binaries
		objdir("$(ProjectDir)obj/" .. _projectName .. "/" .. currentPlatform .. "/%{cfg.buildcfg}") -- Output directory for intermediate files
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
			"NOPCH",
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
			"$(ProjectDir)../../../common/components",
			
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
			ProjectRelativeSDKSRoot .. "/externals/FreeImage/Dist/" .. currentPlatform .. "/%{cfg.buildcfg}",
			ProjectRelativeSDKSRoot .. "/externals/physfs/build/%{cfg.buildcfg}",
			ProjectRelativeSDKSRoot .. "/externals/libConfigPortable/lib/" .. currentPlatform .. "/%{cfg.buildcfg}",
			ProjectRelativeSDKSRoot .. "/externals/OpenAL_1.1_SDK/libs/Win64/",
			ProjectRelativeSDKSRoot .. "/externals/freealut/build/src/%{cfg.buildcfg}",	
			ProjectRelativeFuetEngineRoot .. "/lib/" .. currentPlatform .. "/DEV_%{cfg.buildcfg}"			
		}

		if _useWxWidgets then
			libdirs
			{
				ProjectRelativeSDKSRoot .. "/Externals/wxWidgets2.8/lib/vc_lib/" .. currentPlatform
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
			"comctl32.lib",
			"rpcrt4.lib",
			"odbc32.lib",
		}

		-- Configuration-specific settings
		filter "configurations:Debug"
			defines { "DEBUG" }
			symbols "On" -- Generate debug symbols

			if _useWxWidgets then

				defines {
					"__WXMSW__",
					"__WXDEBUG__",
				}

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

			staticruntime "off"
			runtime "Debug"


		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On" -- Enable optimizations			

			if _useWxWidgets then
				defines {
					"__WXMSW__",
				}

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

			staticruntime "off"
			runtime "Release"

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
workspace "FuetEngine_Tools"
    configurations { "Debug", "Release" }
    location "build" -- Where generated files (like Visual Studio solutions) will be stored
    architecture "x86_64"

------------------------------------------------------------------------------
group("Tools")
	addToolProject("Autoversion", 	false)
	addToolProject("SpriteEditor", 	true)
	addToolProject("FontEditor", 		true)
	addToolProject("ConfigBinarizer", false)
	addToolProject("ActorEditor", 	true)
	
--[[
group("FuetEngine")	
	includeExternalProjects(frameworkRoot .. "/build/vs2022")
]]--
