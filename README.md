This is the root directory where the projects build with FuetEngine are located.
Create a directory here where your game project will be located.

All game projects share some functionality and tools. Inside the share directory you'll find anything that can be shared among all the projects.

#FuetEngine Notes:

- From windows Environment, set environment variable **FuetEngineProjectsDev** to the root directory where all the development projects and shared directory is located.

- Some features may require external libraries. Those are expected to be found under 
**$(FuetEngineProjectsDev)/shared/sdks** directory. For instance, almost all tools / projects use images, so if we're using a third party library to load images, it most probably will be found here.

- Although the engine allows plugin any other image loading library, I've been using FreeImage since the begining of the project. It should be found under **$(FuetEngineProjectsDev)/sdks/FreeImage**, and its related files should be located under "Dist" directory inside the FreeImage folder. Before compiling any FuetEngine sample / tool or applicaton we should have a binary version of this library, so we'll have to build it. To ensure compatibility with the latest FuetEngine development state, a copy of the source code at that moment is provided, so you can build it by yourself.

- Quick overview. Go to the samples directory and open the solution found under build directory. Compile and test each sample.

Im the following weeks I'll be adding all the tools built using FuetEngine and for FuetEngine and eventually a sample complex project.

### build instructions
In most cases you will need to use premake (included in $(FuetEngineProjectDev)/shared/toolchain/general) to create the project files so you can build the binaries.

- Build FreeImage:
  - Premake5 vs2022 (or any other)
  - Generate the Debug or Release libraries for x64 or Win32
  - Premake5 install. 
    
- Build freealut.
  - Premake5 vs2022
  - Generate the Debug or Release libraries for x64 or Win32

- Build libConfigPortable
  - Premake5 vs2022
  - Generate the Debug or Release libraries for x64 or Win32

- Build wxWidgets2.8
  - Generate the Debug, Release, Unicode Debug, or Unicode Releaes libraries for x64 or Win32

Now you can build FuetEngine things:

- Build FuetEngine
  - Premake5 vs2022
  - Generate the Debug or Release libraries for x64 or Win32

- Build Samples
  - Premake5 vs2022
  - Generate the Debug or Release libraries for x64 or Win32

- Build Tools
  - Premake5 vs2022
  - Generate the Debug or Release libraries for x64 or Win32

Or you can switch to a specific branch and build one of the projects.
