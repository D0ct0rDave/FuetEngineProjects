// ----------------------------------------------------------------------------
/*! \class Fuet Engine Configuration File (Linux)
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FECConfigH
#define FECConfigH
// ----------------------------------------------------------------------------
#include "../FEConfigOptions.h"
// ----------------------------------------------------------------------------

// Graphics API
#define GRAPHICS        USE_OPENGL

// Sound System
#define SOUND           USE_OPENAL

// Config files
#define CONFIG          USE_BIN_AND_LIBCONFIG

// Image loading support
#define IMAGELIB        USE_FREEIMAGE

// Basic Memory management
#define MEMORY          USE_STDMEM

// File System Management
#define FILESYS         USE_STDFILESYS

// Input (polled through X11, see CFECLinuxInput.cpp)
#define INPUTSYS        USE_NATIVE_SUPPORT

// Math
#define MATH            USE_STDMATH

// Strings
#define STRING          USE_STDSTRING

// Localization
#define LOCAL           USE_STDLOCAL

// Profile
#define PROFILE         USE_STDPROFILE

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
