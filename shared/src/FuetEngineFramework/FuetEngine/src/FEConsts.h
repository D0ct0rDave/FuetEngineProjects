// ----------------------------------------------------------------------------
/*! \class FEConsts
 *  \brief Constants used by the engine exposed to the external application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FEConstsH
#define FEConstsH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
#ifndef NULL
#define NULL 0L
#endif

/// Maximum size of the logger buffer
const uint LOGGER_MAX_SIZE   = 65536;

// ----------------------------------------------------------------------------
const FEReal EPSILON = 0.001f;  // for insertions or key frame retrievement

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
