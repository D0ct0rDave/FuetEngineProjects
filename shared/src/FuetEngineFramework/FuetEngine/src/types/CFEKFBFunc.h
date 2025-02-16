// ----------------------------------------------------------------------------
/*! \class CFEKFBFunc
 *  \brief Key Frame Based Function
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEKFBFuncH
#define CFEKFBFuncH
// ----------------------------------------------------------------------------
#include <math.h>		// for fmodf
#include <algorithm>	// for std::sort
#include "FEBasicTypes.h"    // basic type inclusion
#include "support/math/CFEMath.h"
// ----------------------------------------------------------------------------
const FEReal EPSILON = 0.001f;  // for insertions or key frame retrievement
// ----------------------------------------------------------------------------
typedef enum EFEKFLerpFunc
{
	KFLF_NONE,

	KFLF_CONSTANT,	/// To retrieve constant values until the next frame.
	KFLF_LERP,		/// To perform linear interpolation between this frame and the next.
	KFLF_SIN,		/// To perform sinusoidal interpolation between this frame and the next.
	KFLF_EXP,		/// To perform logarithmic interpolation between this frame and the next.
	KFLF_RAND,		/// To retrieve random values between the interval limits
	KFLF_SINSIN,	/// To perform sinusoidal interpolation between this frame and the next.
	KFLF_EXPLOG,	/// To perform logarithmic interpolation between this frame and the next.
    KFLF_ISIN,       /// To perform inverse sinusoidal interpolation between this frame and the next.

	KFLF_NUM
};
// ----------------------------------------------------------------------------
template<class T> T random_lerp(const T& _oA,const T& _oB,FEReal _rFact);
// ----------------------------------------------------------------------------
class CFEKeyFrame
{
    public:
		/// Default constructor for the class.
		CFEKeyFrame() :
			m_rKeyTime(_0r),
			m_eLerpFunc(KFLF_NONE)
		{
		}

		CFEKeyFrame(FEReal _rKeyTime,EFEKFLerpFunc _eLF)
		{
			m_rKeyTime  = _rKeyTime;
			m_eLerpFunc = _eLF;
		}    

		/// Comparaison operator for sorting purposes.
		bool operator < (const CFEKeyFrame& b)
		{
			return ( m_rKeyTime < b.m_rKeyTime );
		}

		/// Retrieves the limit of this key.
		FEReal rGetKeyTime()
		{
			return(m_rKeyTime);
		}

		/// Sets the time of this key.
		void SetKeyTime(FEReal _rKeyTime)
		{
			m_rKeyTime = _rKeyTime;
		}

		/// Retrieves the lerp function of this key frame.
		EFEKFLerpFunc eGetLerpFunc()
		{
			return(m_eLerpFunc);
		}

		/// Sets the lerp function of this key.
		void SetLerpFunc(EFEKFLerpFunc _eLerpFunc)
		{
			m_eLerpFunc = _eLerpFunc;
		}

	protected:

		EFEKFLerpFunc		m_eLerpFunc;
		FEReal			m_rKeyTime;
};
// ----------------------------------------------------------------------------
/// Struct to hold the keyframes
template <typename T>
class CFETypedKeyFrame : public CFEKeyFrame
{
	public:

		/// Default constructor for the class.
		CFETypedKeyFrame() : CFEKeyFrame(_0r,KFLF_NONE)
		{
		}

		CFETypedKeyFrame(const T& _oKey,FEReal _rKeyTime,EFEKFLerpFunc _eLF) : CFEKeyFrame(_rKeyTime,_eLF)
		{
			m_oKey   = _oKey;
		}

	public:
		// these parameters are allowed to be changed externally.
		T				m_oKey;
};
// ----------------------------------------------------------------------------
typedef enum EFEKFBFuncWrapMode
{
	KFBFWM_NONE,

	KFBFWM_LOOP,			/// To starts from the begining.
	KFBFWM_PINGPONG,		/// To performs a pingpong over the function.
	KFBFWM_FINALVALUE,		/// To repeat the last value.
	KFBFWM_INITIALVALUE,	/// To repeat the initial value.

	KFBFWM_NUM
};
// ----------------------------------------------------------------------------
template <class T>
class CFEKFBFunc
{
    public:


		/// Default constructor for the class.
		CFEKFBFunc()
		{
			// m_oKeyFrames.SetReallocFactor(1);
			m_eWrapMode = KFBFWM_FINALVALUE;
			m_rMaxLimit = 0;
			m_rLastT	= -_1r;
		}

		/// Retrieves the function value corresponding to the given factor.
		T oGetValue(FEReal _rT);

		/// Deletes the keyframe with the given index in the keyframe array.
		void DeleteKeyFrame(uint _uiKeyFrameIdx)
		{
			if (_uiKeyFrameIdx >= m_oKeyFrames.size())
			{
				// assert ???
				return;
			}

			m_oKeyFrames.erase(this->m_oKeyFrames.begin() + _uiKeyFrameIdx);
			
			// in case we delete the last key frame ...
			if (m_oKeyFrames.size()==0)
			{
			    m_rMaxLimit = 0;
			    m_rLastT	= -_1r;
			}
			else
			{
			    m_rMaxLimit = m_oKeyFrames[m_oKeyFrames.size()-1].rGetKeyTime();
            }
		}

		/// Deletes (if exists) the keyframe at the given time.
		void DeleteKeyFrameAtTime(FEReal _rTime)
		{
	        for (uint i=0;( (i<m_oKeyFrames.size()) && (_rTime >= m_oKeyFrames[i].rGetKeyTime()) );i++)
	            if ( CFEMath::bBetween(m_oKeyFrames[i].rGetKeyTime()-EPSILON,m_oKeyFrames[i].rGetKeyTime()+EPSILON,_rTime) )
	            {
	                DeleteKeyFrame(i);
	                return;
	            }
		}

		/// Deletes all key frames.
		void Reset()
		{
			m_oKeyFrames.erase(this->m_oKeyFrames.begin(),this->m_oKeyFrames.end() );

			m_rMaxLimit = 0;
			m_rLastT	= -_1r;
		}
		
		/// Retrieves the number of keyframes in the function
		uint uiGetNumKeyFrames() const
		{
			return ( m_oKeyFrames.size() );
		}

		/// Inserts a new key frame in the function. If the limit already exists, modifies the keyframe associated with it.
		void InsertKeyFrame(const T& _oKey,FEReal _rKeyTime,EFEKFLerpFunc _eLF);

		/// Retrieves the information related to a given keyframe.
		CFEKeyFrame* poGetKeyFrame(uint _uiKeyFrameIdx)
		{
			if (_uiKeyFrameIdx < m_oKeyFrames.size())
				return (&m_oKeyFrames[_uiKeyFrameIdx]);
			else
				return(NULL);
		}

		/// Retrieves the keyframe associated to a given moment in time or NULL if no keyframe.
		CFEKeyFrame* poGetKeyFrameAtTime(FEReal _rTime)
		{
		    FEReal rKeyTime = _0r;
		    uint i = 0;

	        while ( (i<m_oKeyFrames.size()) && (_rTime >= (rKeyTime-EPSILON)) )
	        {
	            if ( CFEMath::bBetween(rKeyTime-EPSILON,rKeyTime+EPSILON,_rTime) )	            
	                return(&m_oKeyFrames[i]);

                i++;

                if (i<m_oKeyFrames.size())
                    rKeyTime = m_oKeyFrames[i].rGetKeyTime();
            }

		    return(NULL);
		}

		/// Retrieves the information related to a given keyframe.
		CFETypedKeyFrame<T>* poGetKeyFrameData(uint _uiKeyFrameIdx)
		{
		    return( (CFETypedKeyFrame<T>*)poGetKeyFrame(_uiKeyFrameIdx) );
		}

		/// Retrieves the information related to a given keyframe at a given time (if exists)
		CFETypedKeyFrame<T>* poGetKeyFrameDataAtTime(FEReal _rTime)
		{   
		    return( (CFETypedKeyFrame<T>*)poGetKeyFrameAtTime(_rTime) );
		}

		/// Retrieves the wrapping mode of this function.
		EFEKFBFuncWrapMode eGetWrapMode() const
		{
			return (m_eWrapMode);
		}

		/// Sets the wrapping mode for this function.
		void SetWrapMode(EFEKFBFuncWrapMode _eWrapMode)
		{
			m_eWrapMode = _eWrapMode;
		}

		/// Retrieves the MaxLimit of the function.
		FEReal rGetMaxLimit()
		{
			return(m_rMaxLimit);
		}
		
		/// Recalc Max Limit
		void RecalcMaxLimit()
		{
			m_rMaxLimit = m_oKeyFrames[m_oKeyFrames.size()-1].rGetKeyTime();
		}

		/// Invalidates the cached contents of the function.
		void Invalidate()
		{
			m_rLastT = -_1r;
		}

	protected:

		CFEArray< CFETypedKeyFrame<T> >	    m_oKeyFrames;
		EFEKFBFuncWrapMode		            m_eWrapMode;

		FEReal					            m_rMaxLimit;

	    // for random lerp func. Useful also for ptimization purposes.
		FEReal					            m_rLastT;
		T                                   m_oLastValue;
};
// ----------------------------------------------------------------------------
template <class T>
inline T CFEKFBFunc<T>::oGetValue(FEReal _rT)
{
	// If the keyframe array is empty return an undefined value.
	if (_rT == m_rLastT)
	    return(m_oLastValue);
	else
	    m_rLastT = _rT;

	if (m_oKeyFrames.size() == 0)
	{
		// return undefined value
		return(m_oLastValue);
	}

	// If the array has only one value, return it as if the func is constant.
else if ((m_oKeyFrames.size() == 1) || (_rT==_0r))
    {
        m_oLastValue = m_oKeyFrames[0].m_oKey;
		return(m_oLastValue);
	}

	/// Retrieve the proper interval and factor
	FEReal rFact = CFEMath::rAbs(_rT);

	switch (m_eWrapMode)
	{
		case KFBFWM_LOOP:
		{
			if (rFact > m_rMaxLimit)
			{
				// rFact = fmodf(rFact,m_rMaxLimit);
				uint uiLoops = rFact / m_rMaxLimit;
				rFact = rFact - ((FEReal)uiLoops*m_rMaxLimit);

			    //
                if (rFact == _0r)
                {
                    m_oLastValue = m_oKeyFrames[0].m_oKey;
		            return(m_oLastValue);
	            }
			}
		}
		break;
		
		case KFBFWM_INITIALVALUE:
		{
			if (_rT>=m_rMaxLimit)
			{
			    m_oLastValue = m_oKeyFrames[0].m_oKey;
				return( m_oLastValue );
			}
		}
		break;


		case  KFBFWM_PINGPONG:
		{
			if (rFact >= _2r*m_rMaxLimit)
			rFact = CFEMath::rMod(_rT,_2r*m_rMaxLimit);
			
			// WARNING:!!!! si pongo este condicional debajo del siguiente, CW peta!!!!!
            if (rFact == _0r)
            {
                m_oLastValue = m_oKeyFrames[0].m_oKey;
	            return(m_oLastValue);
            }

            // 0 <= rFact < 2*m_rMaxLimit
			if (rFact >= m_rMaxLimit)
				rFact = _2r*m_rMaxLimit - rFact;
		}
		break;


		default:
		case KFBFWM_FINALVALUE:
		{
			if (_rT>=m_rMaxLimit)
			{
			    m_oLastValue = m_oKeyFrames[m_oKeyFrames.size()-1].m_oKey;
				return( m_oLastValue );
			}
		}
		break;
	}
	
	//	PRECONDITION:
	//	    fFact >	_0rf
	//	    fFact <= m_fMaxLimit
	//	    fFact <= m_oKeyFrames[m_oKeyFrame.uiNumElems()-1].m_fLimit
    //      m_oKeyFrames.size() > 1

	uint uiIdx = 1;
	while (rFact>((CFEKeyFrame)m_oKeyFrames[uiIdx]).rGetKeyTime())
		uiIdx++;

	/// Process factor depending upon the interpolation function for this interval
	FEReal rMin = ((CFEKeyFrame)m_oKeyFrames[uiIdx-1]).rGetKeyTime();
	rFact = (rFact - rMin) / ( ((CFEKeyFrame)m_oKeyFrames[uiIdx]).rGetKeyTime() - rMin);

	switch (((CFEKeyFrame)m_oKeyFrames[uiIdx-1]).eGetLerpFunc())
	{
		case KFLF_LERP:
		{
			m_oLastValue = (m_oKeyFrames[uiIdx-1].m_oKey*(_1r-rFact) + m_oKeyFrames[uiIdx].m_oKey*(rFact));
		}
		break;

		case KFLF_CONSTANT:
		{
			m_oLastValue = m_oKeyFrames[uiIdx-1].m_oKey;
		}
		break;

		case KFLF_SIN:
		{
			rFact = CFEMath::rSin(rFact*_PI2r_);
			m_oLastValue = (m_oKeyFrames[uiIdx-1].m_oKey*(_1r-rFact) + m_oKeyFrames[uiIdx].m_oKey*(rFact));
		}
		break;
		
		case KFLF_ISIN:
        {
			rFact = CFEMath::rSin( (rFact - _1r)*_PI2r_ );
			m_oLastValue = (m_oKeyFrames[uiIdx-1].m_oKey*(_1r-rFact) + m_oKeyFrames[uiIdx].m_oKey*(rFact));
		}
		break;
		
		case KFLF_EXP:
		{
			rFact = rFact*rFact;
			m_oLastValue = (m_oKeyFrames[uiIdx-1].m_oKey*(_1r-rFact) + m_oKeyFrames[uiIdx].m_oKey*(rFact));
		}
        break;

		case KFLF_RAND:
		{
	        m_oLastValue = random_lerp((const T&)m_oKeyFrames[uiIdx-1].m_oKey,(const T&)m_oKeyFrames[uiIdx].m_oKey,rFact);
	    }
        break;

        case KFLF_SINSIN:
        {
			FEReal rFact1 = CFEMath::rSin((rFact+_1r)*_PI2r_);
			FEReal rFact2 = CFEMath::rSin(rFact*_PI2r_);
			m_oLastValue = (m_oKeyFrames[uiIdx-1].m_oKey*(rFact1) + m_oKeyFrames[uiIdx].m_oKey*(rFact2));			
        }
        break;

        case KFLF_EXPLOG:
        #if 0
        {
			// FEReal rFact1 = rFact*rFact;
			// FEReal rFact2 = CFEMath::rSin(rFact*_PI2r_);
			// m_oLastValue = (m_oKeyFrames[uiIdx-1].m_oKey*(rFact1) + m_oKeyFrames[uiIdx].m_oKey*(rFact2));			
        }
        break;
        #endif

		default:
		{
			m_oLastValue = (m_oKeyFrames[uiIdx-1].m_oKey*(_1r-rFact) + m_oKeyFrames[uiIdx].m_oKey*(rFact));
		}
		break;
    }

	return( m_oLastValue );
}
// ----------------------------------------------------------------------------
template <class T>
inline void CFEKFBFunc<T>::InsertKeyFrame(const T& _oKey,FEReal _rKeyTime,EFEKFLerpFunc _eLF)
{
    /// "Invalidate" the optimization.
    Invalidate();

    ///
	CFETypedKeyFrame<T> oKF(_oKey,(m_oKeyFrames.size()==0)?FEReal(_0r):_rKeyTime,_eLF);
	
	// look if the key already exists
	for (uint i=0;( (i<m_oKeyFrames.size()) && (_rKeyTime >= m_oKeyFrames[i].rGetKeyTime()) );i++)
	{
	    FEReal rTime = m_oKeyFrames[i].rGetKeyTime();
	    if ( CFEMath::bBetween(rTime-EPSILON,rTime+EPSILON,_rKeyTime) )
	    {
	        // Update this key
	        m_oKeyFrames[i].m_oKey = _oKey;
	        m_oKeyFrames[i].SetLerpFunc(_eLF);
	        return;
	    }
	}
	
	m_oKeyFrames.push_back(oKF);

	// reorder array
	#ifndef DS
	// DMC
	if (m_oKeyFrames.size() > 1)
		std::sort(&m_oKeyFrames[0],&m_oKeyFrames[0] + m_oKeyFrames.size());
    #endif

	RecalcMaxLimit();
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
