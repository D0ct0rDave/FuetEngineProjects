// ----------------------------------------------------------------------------
/*! \class CFERandomGen
 *  \brief Provides a faciliy to get random numbers in a deterministic way.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par GammaE License
 */
// ----------------------------------------------------------------------------
#ifndef CFERandomGenH
#define CFERandomGenH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
class CFERandomGen
{
    public:

		/// Default constructor for the class
        CFERandomGen();
        
        /// Constructor with seed parameter.
		CFERandomGen(uint _uiSeed)
		{
			SetSeed(_uiSeed);
		};

		/// Returns a random real value between (0..1).
		FEReal rRand();

		/// Returns a random real value between (_rMin.._rMax).
		FEReal rRand(FEReal _rMin,FEReal _rMax);

		/// Returns a random integer value between (_iMin .. _iMax) both included.
		int iRand(int _iMin,int _iMax);

		/// Returns a random unsigned integer value between (0.._uiMax-1) _uiMax not included.
		uint uiRand(uint _uiMax);

		/// Retrieves the current seed of this random generator 
		uint uiGetSeed()
		{
			return(m_uiSeed);
		}

		/// Sets the seed for this random number generator
		void SetSeed(uint _uiSeed);

    protected:

		uint	m_uiSeed;
		uint	m_uiSt;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
