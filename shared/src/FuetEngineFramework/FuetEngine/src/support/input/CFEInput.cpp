// ----------------------------------------------------------------------------
/*! \class CFEInput
 *  \brief High Level Input Management Class
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEInput.h"
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
#include <string.h>
// ----------------------------------------------------------------------------
class CFEInputData
{
	public:

        CFEInputData() :
            m_uiScreenVWidth(640),
            m_uiScreenVHeight(480),
            m_hHandler(NULL)
        {
			memset(&m_oCurInput,0,sizeof(TFEInputStruct));
			memset(&m_oOldInput,0,sizeof(TFEInputStruct));
        };

		TFEInputStruct  m_oCurInput;
		TFEInputStruct  m_oOldInput;
		FEHandler       m_hHandler;
		uint            m_uiScreenVWidth;
		uint            m_uiScreenVHeight;
};
// ----------------------------------------------------------------------------
CFEInput::CFEInput()
{
	m_poData = new CFEInputData;
}
// ----------------------------------------------------------------------------
CFEInput::~CFEInput()
{
	CFESystem::Input::Finish(m_poData->m_hHandler);
	delete m_poData;
}
// ----------------------------------------------------------------------------
void CFEInput::Init(FEHandler _hParam)
{
    m_poData->m_hHandler = CFESystem::Input::hInit(_hParam);
    memset(&m_poData->m_oCurInput,0,sizeof(TFEInputStruct));
    memset(&m_poData->m_oOldInput,0,sizeof(TFEInputStruct));
}
// ----------------------------------------------------------------------------
void CFEInput::Finish()
{
}
// ----------------------------------------------------------------------------
void CFEInput::Update(FEReal _rDeltaT)
{
    // copy last data to old structure
    memcpy(&m_poData->m_oOldInput,&m_poData->m_oCurInput,sizeof(TFEInputStruct));

    // retrieve new data into current structure
    CFESystem::Input::Update(m_poData->m_hHandler,&m_poData->m_oCurInput);
}
// ----------------------------------------------------------------------------
bool CFEInput::bDown(EFEInputButton _eIB)
{
    return( m_poData->m_oCurInput.m_bButtons[_eIB] && ! m_poData->m_oOldInput.m_bButtons[_eIB] );
}
// ----------------------------------------------------------------------------
bool CFEInput::bUp(EFEInputButton _eIB)
{
    return( !m_poData->m_oCurInput.m_bButtons[_eIB] && m_poData->m_oOldInput.m_bButtons[_eIB] );
}
// ----------------------------------------------------------------------------
bool CFEInput::bPressed(EFEInputButton _eIB)
{
    return( m_poData->m_oCurInput.m_bButtons[_eIB] );
}
// ----------------------------------------------------------------------------
bool CFEInput::bReleased(EFEInputButton _eIB)
{
    return( ! m_poData->m_oCurInput.m_bButtons[_eIB] );
}
// ----------------------------------------------------------------------------
FEReal CFEInput::rValue(EFEInputAxis _eIA)
{
    return( m_poData->m_oCurInput.m_rAxis[_eIA] );
}
// ----------------------------------------------------------------------------
void CFEInput::SetScreenVWidth(uint _uiScrVWidth)
{
    m_poData->m_uiScreenVWidth = _uiScrVWidth;
}
// ----------------------------------------------------------------------------
void CFEInput::SetScreenVHeight(uint _uiScrVHeight)
{
    m_poData->m_uiScreenVHeight = _uiScrVHeight;
}
// ----------------------------------------------------------------------------
const CFEVect2& CFEInput::oCursorCoords()
{
    static CFEVect2 oCursor;

    oCursor.x = m_poData->m_oCurInput.m_oCursor.m_oCoords.x * (FEReal)m_poData->m_uiScreenVWidth;
    oCursor.y = m_poData->m_oCurInput.m_oCursor.m_oCoords.y * (FEReal)m_poData->m_uiScreenVHeight;

    return(oCursor);
}
// ----------------------------------------------------------------------------
FEReal CFEInput::rCursorPressure(EFEInputPressureButton _eIPB)
{
    return( m_poData->m_oCurInput.m_oCursor.m_rPressure[_eIPB] );
}
// ----------------------------------------------------------------------------
bool CFEInput::bDown(EFEInputPressureButton _eIPB)
{
    return(
                (m_poData->m_oCurInput.m_oCursor.m_rPressure[_eIPB] > _0r)
            && (m_poData->m_oOldInput.m_oCursor.m_rPressure[_eIPB] <= _0r)
        );
}
// ----------------------------------------------------------------------------
bool CFEInput::bUp(EFEInputPressureButton _eIPB)
{
    return(
                (m_poData->m_oCurInput.m_oCursor.m_rPressure[_eIPB]<=_0r)
            && (m_poData->m_oOldInput.m_oCursor.m_rPressure[_eIPB] > _0r)
        );
}
// ----------------------------------------------------------------------------
bool CFEInput::bPressed(EFEInputPressureButton _eIPB)
{
    return(m_poData->m_oCurInput.m_oCursor.m_rPressure[_eIPB]>_0r);
}
// ----------------------------------------------------------------------------
bool CFEInput::bReleased(EFEInputPressureButton _eIPB)
{
    return(m_poData->m_oCurInput.m_oCursor.m_rPressure[_eIPB]<=_0r);
}
// ----------------------------------------------------------------------------
