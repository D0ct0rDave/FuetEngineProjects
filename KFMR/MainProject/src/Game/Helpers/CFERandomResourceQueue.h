//-----------------------------------------------------------------------------
/*! \class CFERandomResourceQueue
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CFERandomResourceQueueH
#define CFERandomResourceQueueH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/CGameVisitor.h"
//-----------------------------------------------------------------------------
template <typename T>
class CFERandomResourceQueue : public CFEResourceQueue<T>
{
	public:
		/// Gets a free resource in the system.
		T* poGetResource()
		{
		   bool bExit = false;
		   uint uiTestElems = 0;
		   uint uiTryCounter = 0;
		   uint uiMaxTries = uiGetNumElems() * 2;		// Al ser aleatorio, para no entrar en un bucle infinito al no encontrar resource libre haremos NumElements * 2 intentos

		   do {
			   uiTestElems = CFEMath::iRand(0, uiGetNumElems() - 1);

			   if ( m_oList[uiTestElems].m_bFree)
			   {
					m_oList[uiTestElems].m_bFree = false;
					return( m_oList[uiTestElems].m_poElem );
			   }
			   uiTryCounter++;
		   } while (!bExit && uiTryCounter < uiMaxTries);


		   // En el caso de que no haya encontrado ninguno en "MaxTries" intentos, los recorremos como lo haría el GetResource secuencial
		   uiTestElems = 0;
		   do {

			   if ( m_oList[m_uiCurElem].m_bFree)
			   {
					m_oList[m_uiCurElem].m_bFree = false;
					return( m_oList[m_uiCurElem].m_poElem );
			   }

			   m_uiCurElem = (m_uiCurElem + 1) % uiGetNumElems();
			   uiTestElems++;

		   } while ( uiTestElems < uiGetNumElems() );

			// No free resources ...
		   return(NULL);
		}
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
