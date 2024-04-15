// ----------------------------------------------------------------------------
/*! \class Fuet Engine Configuration File
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (PROFILE == USE_STDPROFILE) || (PROFILE == USE_PHYSIXFSPROFILE)
// ----------------------------------------------------------------------------
#include "FECStdMessages.h"
#include "FEEnums.h"
#include "FEConsts.h" // for NULL
#include "../CFECore.h"
// ----------------------------------------------------------------------------
char* GENERIC_LOAD_MESSAGE			= NULL;
char* GENERIC_LOAD_MESSAGE_OPTIONS	= NULL;

char* GENERIC_SAVE_MESSAGE 			= NULL;
char* GENERIC_SAVE_MESSAGE_OPTIONS 	= NULL;

char* RECOVER_SAVE_MESSAGE 			= NULL;
char* RECOVER_SAVE_MESSAGE_OPTIONS 	= NULL;

char* CREATE_SAVE_MESSAGE 			= NULL;
char* CREATE_SAVE_MESSAGE_OPTIONS 	= NULL;

char* FATAL_ERROR_MESSAGE 			= NULL;
char* FATAL_ERROR_MESSAGE_OPTIONS 	= NULL;

char* BADFSFORMAT_ERROR_MESSAGE 		= NULL;
char* BADFSFORMAT_ERROR_MESSAGE_OPTIONS = NULL;
// ----------------------------------------------------------------------------
// we need to separate every language into a separate file
// since some languages will use ANSI character sets and other 
// will use UTF8. The problem comes when a language with ANSI character set
// uses characters from the extended ascii part, like accents etc,
// on a single UTF8 file, these characters become UTF8 characters...
// ----------------------------------------------------------------------------
typedef struct{

	char* m_szGenericLoad;
	char* m_szGenericLoadOptions;
	char* m_szGenericSave;
	char* m_szGenericSaveOptions;
	char* m_szCreateSave;
	char* m_szCreateSaveOptions;
	char* m_szFatalError;
	char* m_szFatalErrorOptions;
	char* m_szBadFSFormat;
	char* m_szBadFSFormatOptions;
	char* m_szRecoverSave;
	char* m_szRecoverSaveOptions;

}TLanguageMessage;
// ----------------------------------------------------------------------------
/*
	LID_COMMON = 0,

	LID_ES,						// spanish
	LID_EN,						// english
	LID_FR,						// french
	LID_IT,						// italian
	LID_DE,						// deutch (german)
	LID_DU,						// dutch
	LID_PT,						// portuguese

	LID_US_ES,					// us spanish
	LID_US_EN,					// us english
	LID_US_FR,					// us french
	LID_US_PT,					// us portuguese

	LID_JP,						// japanish
*/
// ----------------------------------------------------------------------------
TLanguageMessage LANGUAGEMESSAGES[LID_NUM_IDs] = 
{
	// common / es / en / fr / it / de /du / pt / us_es / us_en / us_fr /us_pt / jp

	// LID_COMMON,							// common	
	// #include "FECStdMessages_en.inc"
	{"","","","","","","","","","","",""},

	// LID_ES,								// spanish
	// #include "FECStdMessages_es.inc"
	{"","","","","","","","","","","",""},

	// LID_EN,								// english
	// #include "FECStdMessages_en.inc"
	{"","","","","","","","","","","",""},

	// LID_FR,								// french
	// #include "FECStdMessages_fr.inc"
	{"","","","","","","","","","","",""},

	// LID_IT,								// italian
	// #include "FECStdMessages_it.inc"
	{"","","","","","","","","","","",""},

	// LID_DE,								// deutch (german)
	// #include "FECStdMessages_de.inc"
	{"","","","","","","","","","","",""},

	// LID_DU,								// dutch
	// #include "FECStdMessages_du.inc"
	// #include "FECStdMessages_en.inc"
	{"","","","","","","","","","","",""},

	// LID_PT,								// portuguese
	// #include "FECStdMessages_pt.inc"
	// #include "FECStdMessages_en.inc"
	{"","","","","","","","","","","",""},

	// LID_US_ES,							// us spanish
	// #include "FECStdMessages_es.inc"
	{"","","","","","","","","","","",""},

	// LID_US_EN,							// us english
	// #include "FECStdMessages_en.inc"
	{"","","","","","","","","","","",""},

	// LID_US_FR,							// us french
	// #include "FECStdMessages_us_fr.inc"
	// #include "FECStdMessages_fr.inc"
	{"","","","","","","","","","","",""},

	// LID_US_PT,							// us portuguese
	// #include "FECStdMessages_us_pt.inc"
	// #include "FECStdMessages_pt.inc"
	// #include "FECStdMessages_en.inc"
	{"","","","","","","","","","","",""},

	// LID_JP,								// japanese
	// #include "FECStdMessages_jp.inc"
	{"","","","","","","","","","","",""}
};
// ----------------------------------------------------------------------------
namespace FECStdMessages{
// ----------------------------------------------------------------------------
void Init()
{
	EFELocalID eID = CFECore::Local::eGetLocalID();

	// depending on the language set message pointers
	GENERIC_LOAD_MESSAGE 				= LANGUAGEMESSAGES[eID].m_szGenericLoad;
	GENERIC_LOAD_MESSAGE_OPTIONS 		= NULL;

	GENERIC_SAVE_MESSAGE 				= LANGUAGEMESSAGES[eID].m_szGenericSave;
	GENERIC_SAVE_MESSAGE_OPTIONS 		= NULL;

	CREATE_SAVE_MESSAGE  				= LANGUAGEMESSAGES[eID].m_szCreateSave;
	CREATE_SAVE_MESSAGE_OPTIONS 		= NULL;

	FATAL_ERROR_MESSAGE	 				= LANGUAGEMESSAGES[eID].m_szFatalError;
	FATAL_ERROR_MESSAGE_OPTIONS 		= NULL;

	BADFSFORMAT_ERROR_MESSAGE 			= LANGUAGEMESSAGES[eID].m_szBadFSFormat;
	BADFSFORMAT_ERROR_MESSAGE_OPTIONS 	= NULL;

	RECOVER_SAVE_MESSAGE 				= LANGUAGEMESSAGES[eID].m_szRecoverSave;
	RECOVER_SAVE_MESSAGE_OPTIONS 		= LANGUAGEMESSAGES[eID].m_szRecoverSaveOptions;
}
// ----------------------------------------------------------------------------
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
