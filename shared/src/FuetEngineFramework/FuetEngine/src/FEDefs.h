// ----------------------------------------------------------------------------
/*! \class FEDefs
 *  \brief Useful macros and definitions for the engine and applications.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FEDefsH
#define FEDefsH
// ----------------------------------------------------------------------------

/// Cast value
#define CASTVALUE(type,value) reinterpret_cast<type>(value)

/// Cast variable
#define CASTVAR(type,value) (* (reinterpret_cast<type*>(reinterpret_cast<int*>(&value))) )

/// Get pointer value
#define GETPVALUE(type,value) CASTVALUE(type,value)

/// Get property value
#define GETPROPVALUE(type,value) CASTVALUE(type,value)

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
