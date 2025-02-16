// ----------------------------------------------------------------------------
/*! \class Fuet Engine Configuration Options
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FEConfigOptionsH
#define FEConfigOptionsH
// ----------------------------------------------------------------------------

// Target OS Platform
#define USE_WINDOWS         1
#define USE_LINUX           2
#define USE_MACOS           3
#define USE_DS              4

// #define TARGETPLATFORM: This variable will hold the target platform where the engine is built.

// Graphics API
#define USE_NATIVE_SUPPORT	0
#define USE_OPENGL		    1
#define USE_DIRECT3D        2

// #define GRAPHICS: This variable will hold the graphics API to implement the engine functionality.

// Sound System
#define USE_NATIVE_SUPPORT	0
#define USE_OPENAL          1
#define USE_BASS            2
#define USE_FMOD            3
#define USE_DIRECTSOUND     4

// #define SOUND: This variable will hold the sound API to implement the engine functionality.

// Config files
#define USE_NATIVE_SUPPORT		0
#define USE_INIFILE				1
#define USE_XML					2
#define USE_LIBCONFIG			3
#define USE_BINCONFIG			4
#define USE_BIN_AND_LIBCONFIG	5

// #define CONFIG: This variable will hold the config file system to implement the engine functionality.

// Image loading support
#define USE_NATIVE_SUPPORT	0
#define USE_FREEIMAGE       1
#define USE_DEVIL           2
#define USE_IMAGEMAGIC      3

// #define IMAGELIB: This variable will hold the image API to implement the engine functionality.

// Basic Memory management
#define USE_NATIVE_SUPPORT	0
#define USE_STDMEM          1

// #define MEMORY: This variable will hold the memory management system to implement the engine functionality.

// File System Management
#define USE_NATIVE_SUPPORT	0
#define USE_STDFILESYS      1

// #define FILESYS: This variable will hold the file system functionality to implement the engine functionality.

// Input
#define USE_NATIVE_SUPPORT	0
#define USE_DIRECTINPUT     1

// #define INPUTSYS: This variable will hold the input API to implement the engine functionality.

// Math
#define USE_NATIVE_SUPPORT	0
#define USE_STDMATH         1


// #define MATH: This variable will hold the math system to implement the engine functionality.

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
