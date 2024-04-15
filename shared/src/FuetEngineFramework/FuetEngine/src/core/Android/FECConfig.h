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
#ifndef FECConfigH
#define FECConfigH
// ----------------------------------------------------------------------------
#include "../FEConfigOptions.h"
// ----------------------------------------------------------------------------

// Graphics API
#define GRAPHICS        USE_OPENGL

// Sound System
#define SOUND           USE_NATIVE_SUPPORT

// Config files
#define CONFIG          USE_BINCONFIG

// Image loading support
#define IMAGELIB        USE_NATIVE_SUPPORT

// Basic Memory management
#define MEMORY          USE_STDMEM

// File System Management
#define FILESYS         USE_NATIVE_SUPPORT

// Input
#define INPUTSYS        USE_NATIVE_SUPPORT

// Math
#define MATH            USE_STDMATH

// String functionality
#define STRING          USE_STDSTRING

// Localization functionality
#define LOCAL			USE_STDLOCAL

// Profile functionality
#define PROFILE			USE_NATIVE_SUPPORT
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
