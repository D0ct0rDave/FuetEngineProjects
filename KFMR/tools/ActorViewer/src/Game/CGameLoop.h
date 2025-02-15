//-----------------------------------------------------------------------------
/*! \class CGameLoop
 *  \brief The Game Loop Class all the game components.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CGameLoopH
#define CGameLoopH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CGameLoop
{
	public:

		/// 
		static void Init(CFERenderer* _poRnd,CFEInput* _poInput);

		/// 
		static void Finish();
		
		///		
		static void Loop();

	protected:
		/// 
		static void Update(FEReal _rDeltaT);

		/// 
		static void Render();
	
		/// Sets the renderer this module will use.
		static void SetRenderer(CFERenderer* _poRnd)
		{
			m_poRnd = _poRnd;
		}
	
	protected:

		static CFERenderer*	m_poRnd;
		static CFEInput*	m_poInput;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
