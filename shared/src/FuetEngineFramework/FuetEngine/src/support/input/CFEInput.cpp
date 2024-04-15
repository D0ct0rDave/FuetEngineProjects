// ----------------------------------------------------------------------------
/*! \class CFEInput
 *  \brief High Level Input Management Class
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEInput.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
#include <string.h>
// ----------------------------------------------------------------------------
class CFEInputData
{
	public:

        CFEInputData() :
            m_hHandler(NULL),
            m_uiScreenVWidth(640),
            m_uiScreenVHeight(480)
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
	if (m_poData->m_hHandler != NULL)
		CFECore::Input::Finish(m_poData->m_hHandler);

	delete m_poData;
}
// ----------------------------------------------------------------------------
void CFEInput::Init(FEHandler _hParam)
{
    m_poData->m_hHandler = CFECore::Input::hInit(_hParam);
    memset(&m_poData->m_oCurInput,0,sizeof(TFEInputStruct));
    memset(&m_poData->m_oOldInput,0,sizeof(TFEInputStruct));
}
// ----------------------------------------------------------------------------
void CFEInput::Finish()
{
	CFECore::Input::Finish(m_poData->m_hHandler);
	m_poData->m_hHandler = NULL;
}
// ----------------------------------------------------------------------------
void CFEInput::Update(FEReal _rDeltaT)
{
	// Comment out this line. 21-Oct-2013 (Cubit)
	/// if (_rDeltaT == _0r) return;   <-----

	// Si se hace eso, hay que hacerlo para todo el código. Si pilla un 
	// evento down a true, y varios de los siguientes fotogramas son a tiempo _0r,
	// tendremos varios fotogramas consecutivos con bDown.
	// En el caso de una acción como cargar recursos al apretar bDown, implicaría
	// cargarlos varias veces seguidas. Si después de la carga se hace un reset timers del 
	// renderer puede implicar incluso un cuelgue.
	// Casi lo mejor es que este tipo de cosas se hagan a nivel de aplicación, pues surte el mismo
	// efecto y además permite tomar la decisión de hacerlo o no hacerlo.
	
	
	
    // copy last data to old structure
    memcpy(&m_poData->m_oOldInput,&m_poData->m_oCurInput,sizeof(TFEInputStruct));

    // retrieve new data into current structure
    CFECore::Input::Update(m_poData->m_hHandler,&m_poData->m_oCurInput);
}
// ----------------------------------------------------------------------------
FEBool CFEInput::bDown(EFEInputButton _eIB)
{
    return( m_poData->m_oCurInput.m_bButtons[_eIB] && ! m_poData->m_oOldInput.m_bButtons[_eIB] );
}
// ----------------------------------------------------------------------------
FEBool CFEInput::bUp(EFEInputButton _eIB)
{
    return( !m_poData->m_oCurInput.m_bButtons[_eIB] && m_poData->m_oOldInput.m_bButtons[_eIB] );
}
// ----------------------------------------------------------------------------
FEBool CFEInput::bPressed(EFEInputButton _eIB)
{
    return( m_poData->m_oCurInput.m_bButtons[_eIB] );
}
// ----------------------------------------------------------------------------
FEBool CFEInput::bReleased(EFEInputButton _eIB)
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
FEBool CFEInput::bDown(EFEInputPressureButton _eIPB)
{
    return(
                (m_poData->m_oCurInput.m_oCursor.m_rPressure[_eIPB] > _0r)
            && (m_poData->m_oOldInput.m_oCursor.m_rPressure[_eIPB] <= _0r)
        );
}
// ----------------------------------------------------------------------------
FEBool CFEInput::bUp(EFEInputPressureButton _eIPB)
{
    return(
                (m_poData->m_oCurInput.m_oCursor.m_rPressure[_eIPB]<=_0r)
            && (m_poData->m_oOldInput.m_oCursor.m_rPressure[_eIPB] > _0r)
        );
}
// ----------------------------------------------------------------------------
FEBool CFEInput::bPressed(EFEInputPressureButton _eIPB)
{
    return(m_poData->m_oCurInput.m_oCursor.m_rPressure[_eIPB]>_0r);
}
// ----------------------------------------------------------------------------
FEBool CFEInput::bReleased(EFEInputPressureButton _eIPB)
{
    return(m_poData->m_oCurInput.m_oCursor.m_rPressure[_eIPB]<=_0r);
}
// ----------------------------------------------------------------------------
