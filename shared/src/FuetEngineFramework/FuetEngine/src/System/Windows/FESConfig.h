// ----------------------------------------------------------------------------
/*! \class Fuet Engine Configuration File
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FESConfigH
#define FESConfigH
// ----------------------------------------------------------------------------
#include "../FEConfigOptions.h"
// ----------------------------------------------------------------------------

// Graphics API
#ifndef GRAPHICS
#define GRAPHICS        USE_OPENGL
// #define GRAPHICS        USE_DIRECT3D
#endif

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

// Input
// #define INPUTSYS        USE_DIRECTINPUT
#define INPUTSYS        USE_NATIVE_SUPPORT

// Math
#define MATH            USE_STDMATH

// String functionality
#define STRING          USE_STDSTRING

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
