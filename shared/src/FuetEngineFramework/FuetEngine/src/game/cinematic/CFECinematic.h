// -----------------------------------------------------------------------------
/*! \class CFECinematic
 *  \brief A class to manage scripts.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFECinematicH
#define CFECinematicH
// -----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
// -----------------------------------------------------------------------------
class CFEScript;
// -----------------------------------------------------------------------------
class CFECinematicPlane
{
	public:
		/// Default constructor of the class
		CFECinematicPlane() : m_poScript(NULL) {};
		
		CFECinematicPlane(CFEScript* _poScript) : m_poScript(_poScript)
		{
		
		};
		
		/// Destructor of the class
		~CFECinematicPlane();

		/// Retrieve the script used bu this cinematic plane
		CFEScript* poGetScript() { return(m_poScript); };

	protected:
		
		CFEScript* m_poScript;	
	
		#ifdef _DEBUG
	public:
		CFEString m_sPlaneName;
		#endif
};
// -----------------------------------------------------------------------------
class CFECinematic
{
	public:
		/// Default constructor
		 CFECinematic();
		 
		 /// Destructor of the class
		~CFECinematic();
	
	public:

		 CFEArray<CFECinematicPlane*> m_oPlanes;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
