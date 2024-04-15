// ----------------------------------------------------------------------------
/*! \class CFEHUDActionDefaultValues
 *  \brief A generic class to translate the text of a given HUD instance using a dictionary.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDActionDefaultValuesH
#define CFEHUDActionDefaultValuesH
//-----------------------------------------------------------------------------
class CFEHUD;
class CFEDictionary;
class CFEHUDObject;
//-----------------------------------------------------------------------------
class CFEHUDActionDefaultValues
{
	public:

        /// Sets the default values for all the objects inside the element.
        static void Setup(CFEHUDObject* _poObj);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
