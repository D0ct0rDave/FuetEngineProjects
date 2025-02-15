// ----------------------------------------------------------------------------
/*! \class CFEHUDTranslator
 *  \brief A generic class to translate the text of a given HUD instance using a dictionary.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDTranslatorH
#define CFEHUDTranslatorH
//-----------------------------------------------------------------------------
class CFEHUD;
class CFEDictionary;
//-----------------------------------------------------------------------------
class CFEHUDTranslator
{
	public:

        /// Translates the given HUD using the given dictionary.
        static void Translate(CFEHUD* _poHUD,CFEDictionary* _poDict);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------