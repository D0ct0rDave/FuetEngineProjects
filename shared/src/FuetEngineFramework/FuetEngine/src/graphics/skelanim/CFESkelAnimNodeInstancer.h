// ----------------------------------------------------------------------------
/*! \class CFESkelAnimNodeInstancer
 *  \brief Creates an instance of the given skeleton anim node.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimNodeInstancerH
#define CFESkelAnimNodeInstancerH
//-----------------------------------------------------------------------------
#include "CFESkelAnim.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNodeInstancer
{
    public:        

        static CFESkelAnimNode* poCreateInstance(CFESkelAnimNode* _poSkelAnimNode);
 };
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------