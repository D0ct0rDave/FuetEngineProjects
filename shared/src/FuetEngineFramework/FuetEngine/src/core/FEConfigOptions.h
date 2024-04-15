// ----------------------------------------------------------------------------
/*! \class Fuet Engine Configuration Options
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FEConfigOptionsH
#define FEConfigOptionsH
// ----------------------------------------------------------------------------

// Target OS Platform
#define USE_DEVELOPMENT		0
#define USE_WINDOWS         1
#define USE_LINUX           2
#define USE_MACOS           3
#define USE_DS              4
#define USE_DSi             5
#define USE_WII             6
#define USE_N3DS            7
#define USE_COCOS2D_X		8
#define USE_ANDROID			9
#define USE_CAFE			10

// #define TARGETPLATFORM: This variable will hold the target platform where the engine is built.

// Graphics API
#define USE_NATIVE_SUPPORT	0
#define USE_OPENGL		    1
#define USE_OPENGL_STEREO	2
#define USE_DIRECT3D        3
#define USE_OPENGLES	    4
#define USE_OPENGLES2	    5
#define USE_PICAONDESKTOP	6
#define USE_GD				7

// #define GRAPHICS: This variable will hold the graphics API to implement the engine functionality.

// Sound System
#define USE_NATIVE_SUPPORT	0
#define USE_OPENAL          1
#define USE_BASS            2
#define USE_FMOD            3
#define USE_DIRECTSOUND     4
#define USE_DUMMYAUDIO		5

// #define SOUND: This variable will hold the sound API to implement the engine functionality.

// Sound Sample loading support
#define USE_NATIVE_SUPPORT	0
#define USE_OPENAL	        1
#define USE_LIBSND          2

// #define SAMPLELIB: This variable will hold the sound sample API to implement the engine functionality.

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
#define USE_STDIMGLIB       1
#define USE_FREEIMAGE       2
#define USE_DEVIL           3
#define USE_IMAGEMAGIC      4

// USE_STDIMGLIB is based on TGA files.

// #define IMAGELIB: This variable will hold the image API to implement the engine functionality.

// Basic Memory management
#define USE_NATIVE_SUPPORT	0
#define USE_STDMEM          1

// #define MEMORY: This variable will hold the memory management system to implement the engine functionality.

// File System Management
#define USE_NATIVE_SUPPORT	0
#define USE_STDFILESYS      1
#define USE_PHYSIXFSFILESYS 2
// #define FILESYS: This variable will hold the file system functionality to implement the engine functionality.

// Input
#define USE_NATIVE_SUPPORT			0
#define USE_DIRECTINPUT				1
#define USE_NATIVE_AND_DIRECTINPUT	2

// #define INPUTSYS: This variable will hold the input API to implement the engine functionality.

// Math
#define USE_NATIVE_SUPPORT	0
#define USE_STDMATH         1

// #define MATH: This variable will hold the math system to implement the engine functionality.

// String
#define USE_NATIVE_SUPPORT	0
#define USE_STDSTRING		1

// #define STRING: This variable will hold the input API to implement the engine functionality.

// Profile
#define USE_NATIVE_SUPPORT	0
#define USE_STDPROFILE		1
#define USE_DS_EEPROM		2
#define USE_DSi_NAND		3
#define USE_PHYSIXFSPROFILE 4

// Localization and region support.
#define USE_NATIVE_SUPPORT	0
#define USE_STDLOCAL		1
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
