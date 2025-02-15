//-----------------------------------------------------------------------------
/*! \class CCameraMgr
 *  \brief Helper class to retrieve the ground level at a certain map place.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CCameraMgrH
#define CCameraMgrH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CCameraMgr
{
	public:

		/// 
		static void Init(uint _uiLevel);

		/// 
		static void Reset();

		/// 
		static void Finish();

		/// 
		static void Update(FEReal _rDeltaT);
		
		/// Perturbates the camera
        static void Perturb(FEReal _rTime,FEReal _rXMagnitude,FEReal _rYMagnitude);

        /// Perturbates the camera
        static void Perturb(FEReal _rTime,FEReal _rMagnitude);
		
		///
		static const CFEVect2& oGetPos();
		
		///
		static const FEReal rGetZoom();
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
