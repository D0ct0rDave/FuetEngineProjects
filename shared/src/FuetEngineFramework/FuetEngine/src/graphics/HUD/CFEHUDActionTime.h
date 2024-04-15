// ----------------------------------------------------------------------------
/*! \class CFEHUDActionTime
 *  \brief Helper class to retrieve the time of an element action.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDActionTimeH
#define CFEHUDActionTimeH
//-----------------------------------------------------------------------------
class CFEHUDElementAction;
//-----------------------------------------------------------------------------
class CFEHUDActionTime
{
    public:

        /// Retrieves the maximum time of the element action not taking into account the wrap mode.
        static FEReal rGetMaxActionTime(CFEHUDElementAction* _poAction);

		/// Retrieves the time of the element action or -1 if wrap mode is loop.
		static FEReal rGetActionTime(CFEHUDElementAction* _poAction);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
