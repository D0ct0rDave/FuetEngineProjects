// ----------------------------------------------------------------------------
/*! \class Fuet Engine Configuration File
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
// #define GRAPHICS        USE_DIRECT3D

// Sound System
#define SOUND           USE_OPENAL

// Config files
#define CONFIG          USE_BIN_AND_LIBCONFIG

// Image loading support
#define IMAGELIB        USE_FREEIMAGE

// Basic Memory management
#define MEMORY          USE_STDMEM

// File System Management
#define FILESYS         USE_PHYSIXFSFILESYS

// Input
#define INPUTSYS        USE_NATIVE_SUPPORT

// Math
#define MATH            USE_STDMATH

// String functionality
#define STRING          USE_STDSTRING

// Localization functionality
#define LOCAL			USE_STDLOCAL

// Profile functionality
#define PROFILE			USE_PHYSIXFSPROFILE
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
