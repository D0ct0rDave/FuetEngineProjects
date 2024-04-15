This is the root directory where the projects build with FuetEngine are located.
Create a directory here where your game project will be located.

All game projects share some functionality and tools. Inside the share directory you'll find anything that can be shared among all the projects.

#FuetEngine Notes:

- From windows Environment, set environment variable "**FuetEngineProjectsDev**" to the root directory where all the development projects and shared directory is located.

- Some features may require external libraries. Those are expected to be found under 
**"$(FuetEngineProjectsDev)/shared/sdks"** directory. For instance, almost all tools / projects use images, so if we're using a third party library to load images, it most probably will be found here.

- Although the engine allows plugin any other image loading library, I've been using FreeImage since the begining of the project. It should be found under "$(FuetEngineProjectsDev)/sdks/FreeImage", and its related files should be located under "Dist" directory inside the FreeImage folder. Before compiling any FuetEngine sample / tool or applicaton we should have a binary version of this library, so we'll have to build it. To ensure compatibility with the latest FuetEngine development state, a copy of the source code at that moment is provided, so you can build it by yourself.

- Quick overview. Go to the samples directory and open the solution found under build directory. Compile and test each sample.
