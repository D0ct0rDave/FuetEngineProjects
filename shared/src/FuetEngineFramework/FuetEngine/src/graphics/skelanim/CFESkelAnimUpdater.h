// ----------------------------------------------------------------------------
/*! \class CFESkelAnimUpdater
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimUpdaterH
#define CFESkelAnimUpdaterH
//-----------------------------------------------------------------------------
class CFESkelAnimInst;
//-----------------------------------------------------------------------------
class CFESkelAnimUpdater
{
    public:

        static void Update(CFESkelAnimInst* _poInstance);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------