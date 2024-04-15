// -----------------------------------------------------------------------------
/*! \class CFEScriptLoader
 *  \brief A class to load scripts.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEScriptLoader.h"
#include "support/misc/CFEStringUtils.h"
// -----------------------------------------------------------------------------
CFEActionLoader* CFEScriptLoader::m_poActionLoader =  NULL;
// -----------------------------------------------------------------------------
CFEScript* CFEScriptLoader::poLoad(const CFEString& _sFilename)
{
	CFEString sFilename = _sFilename + ".scr";

	CFEConfigFile oCfg(sFilename);
	if (! oCfg.bInitialized()) return(NULL);

	// Retrieve the sequence of actions.
	CFEString sActionSequence = oCfg.sGetString("Script.Sequence","");
	if (sActionSequence == "") return(NULL);
	
	// remove '\n','\r','\t',' '...
	CFEString sAux = sActionSequence;
	char* pAux = (char*)sAux.szString();

	sActionSequence = "";
	while (*pAux)
	{
		char c = *pAux;
		if (
			(c != '\n')
			&& (c != '\r')
			&& (c != ' ')
			&& (c != '\t')
			)
			sActionSequence += c;

		pAux++;
	}

	CFEScript* poScript = new CFEScript;
	CFEActionLoader* poActionLoader = m_poActionLoader;
	// create a default action loader 
	if (poActionLoader == NULL) poActionLoader = new CFEActionLoader;


		CFEString sAction;
		do{

			// Retrieve the action to parse
			int iComaPos = sActionSequence.iPos(",");
			if (iComaPos > -1)
			{
				sAction = sActionSequence.SubString(0,iComaPos);
				sActionSequence = sActionSequence.SubString(iComaPos+1,sActionSequence.uiLen() - iComaPos);
			}
			else
				sAction = sActionSequence;

			if (sAction != "")
			{
				// are there any newlines?
				int iNewLinePos = sAction.iPos('\n');
				if (iNewLinePos > -1)
					sAction = sAction.SubString( iNewLinePos,sAction.uiLen() );

				CFEString sPrefix = CFEString("Script.") + sAction;
				CFEAction* poAction = poActionLoader->poLoad(oCfg,sPrefix);

				if (poAction != NULL)
					poScript->AddAction( poAction );
			}

		} while (sAction != sActionSequence);


	if (m_poActionLoader == NULL)
		delete poActionLoader;

	return(poScript);		
}
// -----------------------------------------------------------------------------
