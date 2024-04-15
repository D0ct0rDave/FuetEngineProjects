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
#define SOUND           USE_OPENAL

// Config files
#define CONFIG          USE_LIBCONFIG

// Image loading support
#define IMAGELIB        USE_FREEIMAGE

// Basic Memory management
#define MEMORY          USE_STDMEM

// File System Management
#define FILESYS         USE_STDFILESYS

// Input
#define INPUTSYS        USE_OSINPUT

// Math
#define MATH            USE_STDMATH

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
