// ----------------------------------------------------------------------------
/*! \class CFEKFBFunc
 *  \brief Key Frame Based Function
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEKFBFuncH
#define CFEKFBFuncH
// ----------------------------------------------------------------------------
#include <math.h>		// for fmodf
#include <algorithm>	// for std::sort
#include "types/CFEArray.h"
#include "FEBasicTypes.h"    // basic type inclusion
#include "FEConsts.h"    // basic type inclusion
#include "support/math/CFEMath.h"
// ----------------------------------------------------------------------------
typedef enum EFEKFLerpFunc
{
	KFLF_NONE,

	KFLF_CONSTANT,	/// To retrieve constant values until the next frame.
	KFLF_LERP,		/// To perform linear interpolation between this frame and the next.
	KFLF_SIN,		/// To perform sinusoidal interpolation between this frame and the next.
	KFLF_EXP,		/// To perform logarithmic interpolation between this frame and the next.
	KFLF_RAND,		/// To retrieve random values between the interval limits.
	KFLF_SINSIN,	/// To perform sinusoidal interpolation between this frame and the next.
	KFLF_EXPLOG,	/// To perform logarithmic interpolation between this frame and the next.
    KFLF_ISIN,      /// To perform inverse sinusoidal interpolation between this frame and the next.

	KFLF_NUM,
	KFLF_MAX = 0xffffffff // to force 32 bit enum

}EFEKFLerpFunc;
// ----------------------------------------------------------------------------
// Generic function
// ----------------------------------------------------------------------------
FEReal FEKFBFLerpFuncs_rRand(const FEReal& _rFact);
template<class T> T random_lerp(const T& _oA,const T& _oB,const FEReal& _rFact)
{
    FEReal rFact = FEKFBFLerpFuncs_rRand(_rFact);
    return ( _oA*(_1r - rFact) + _oB*rFact );
}
// ----------------------------------------------------------------------------
// Generic function
// ----------------------------------------------------------------------------
template<class T> T default_value(const T& v)
{
	return(v);
}
// ----------------------------------------------------------------------------
#include "FEKFBFLerpFuncs.h"
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

		CFEKeyFrame(const FEReal& _rKeyTime,EFEKFLerpFunc _eLF) : m_rKeyTime(_rKeyTime), m_eLerpFunc(_eLF)
		{
		}

		/// Comparaison operator for sorting purposes.
		FEBool operator < (const CFEKeyFrame& b)
		{
			return ( m_rKeyTime < b.m_rKeyTime );
		}

		/// Retrieves the limit of this key.
		FEReal rGetKeyTime()
		{
			return(m_rKeyTime);
		}

		/// Sets the time of this key.
		void SetKeyTime(const FEReal& _rKeyTime)
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

		FEReal			m_rKeyTime;
		EFEKFLerpFunc	m_eLerpFunc;
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

		CFETypedKeyFrame(const T& _oKey,const FEReal& _rKeyTime,EFEKFLerpFunc _eLF) : CFEKeyFrame(_rKeyTime,_eLF), m_oKey(_oKey)
		{
		}

		/// Comparaison operator for sorting purposes.
		FEBool operator < (const CFETypedKeyFrame& b)
		{
			return ( m_rKeyTime < b.m_rKeyTime );
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

	KFBFWM_NUM,
	KFBFWM_MAX = 0xffffffff

}EFEKFBFuncWrapMode;
// ----------------------------------------------------------------------------
template <class T>
class CFEKFBFunc
{
    public:


		/// Default constructor for the class.
		CFEKFBFunc()
		{
			Reset();
		}

		/// Retrieves the value corresponding to the given keyframe.
		const T& oGetKeyValue(uint _uiKeyFrame);

		/// Retrieves the function value corresponding to the given factor.
		T oGetValue(const FEReal& _rT);

		/// Retrieves the value corresponding to the given factor between the given keys.
		T oLerp(uint _uiCurKey,uint _uiNextKey,const FEReal& _rFact,EFEKFLerpFunc _eLerpFunc);

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
			    m_rMaxLimit = _0r;
			}
			else
			{
			    m_rMaxLimit = m_oKeyFrames[m_oKeyFrames.size()-1].rGetKeyTime();
            }

            Invalidate();
		}

		/// Deletes (if exists) the keyframe at the given time.
		void DeleteKeyFrameAtTime(const FEReal& _rTime)
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

			m_rMaxLimit = _0r;
			m_eWrapMode = KFBFWM_FINALVALUE;

			// for random lerp func. Useful also for ptimization purposes.
			Invalidate();
		}

		/// Retrieves the number of keyframes in the function
		uint uiGetNumKeyFrames() const
		{
			return ( m_oKeyFrames.size() );
		}

		/// Inserts a new key frame in the function. If the limit already exists, modifies the keyframe associated with it.
		void InsertKeyFrame(const T& _oKey,const FEReal& _rKeyTime,EFEKFLerpFunc _eLF);

		/// Retrieves the information related to a given keyframe.
		CFEKeyFrame* poGetKeyFrame(uint _uiKeyFrameIdx)
		{
			if (_uiKeyFrameIdx < m_oKeyFrames.size())
				return (&m_oKeyFrames[_uiKeyFrameIdx]);
			else
				return(NULL);
		}

		/// Retrieves the keyframe associated to a given moment in time or NULL if no keyframe.
		CFEKeyFrame* poGetKeyFrameAtTime(const FEReal& _rTime)
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
		CFETypedKeyFrame<T>* poGetKeyFrameDataAtTime(const FEReal& _rTime)
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
			if (m_oKeyFrames.size()==0)
			{
				m_rMaxLimit = _0r;
				return;
			}

			m_rMaxLimit = m_oKeyFrames[m_oKeyFrames.size()-1].rGetKeyTime();
		}

		/// Invalidates the cached contents of the function.
		void Invalidate()
		{
		}

	protected:

		CFEArray< CFETypedKeyFrame<T> >	    m_oKeyFrames;
		EFEKFBFuncWrapMode		            m_eWrapMode;

		FEReal					            m_rMaxLimit;
};
// ----------------------------------------------------------------------------
template <class T>
inline T CFEKFBFunc<T>::oLerp(uint _uiCurKey,uint _uiNextKey,const FEReal& _rFact,EFEKFLerpFunc _eLerpFunc)
{
	switch (_eLerpFunc)
	{
		case KFLF_LERP:
		{
			return(m_oKeyFrames[_uiCurKey].m_oKey*(_1r-_rFact) + m_oKeyFrames[_uiNextKey].m_oKey*(_rFact));
		}
		// break; // to avoid unreachable code "warning"

		case KFLF_CONSTANT:
		{
			if (_rFact == _1r)
				return(m_oKeyFrames[_uiNextKey].m_oKey);
			else
				return(m_oKeyFrames[_uiCurKey].m_oKey);
		}
		// break; // to avoid unreachable code "warning"

		case KFLF_SIN:
		{
			FEReal rFact = CFEMath::rSin(_rFact*_PI2r);
			return(m_oKeyFrames[_uiCurKey].m_oKey*(_1r-rFact) + m_oKeyFrames[_uiNextKey].m_oKey*(rFact));
		}
		// break; // to avoid unreachable code "warning"

		case KFLF_ISIN:
        {
			FEReal rFact = _1r + CFEMath::rSin( (_rFact - _1r)*_PI2r );
			return(m_oKeyFrames[_uiCurKey].m_oKey*(_1r-rFact) + m_oKeyFrames[_uiNextKey].m_oKey*(rFact));
		}
		// break; // to avoid unreachable code "warning"

		case KFLF_EXP:
		{
			FEReal rFact = _rFact*_rFact;
			return(m_oKeyFrames[_uiCurKey].m_oKey*(_1r-rFact) + m_oKeyFrames[_uiNextKey].m_oKey*(rFact));
		}
		// break; // to avoid unreachable code "warning"

		case KFLF_RAND:
		#ifndef N3DS
		{
	        return(random_lerp(m_oKeyFrames[_uiCurKey].m_oKey,m_oKeyFrames[_uiNextKey].m_oKey,_rFact));
	    }
		// break; // to avoid unreachable code "warning"
	    #endif

        case KFLF_SINSIN:
        {
			FEReal rFact1 = CFEMath::rSin((_rFact+_1r)*_PI2r);
			FEReal rFact2 = CFEMath::rSin(_rFact*_PI2r);
			return(m_oKeyFrames[_uiCurKey].m_oKey*(rFact1) + m_oKeyFrames[_uiNextKey].m_oKey*(rFact2));
        }
		// break; // to avoid unreachable code "warning"

        case KFLF_EXPLOG:
        {
			FEReal rFact = (CFEMath::rSin(_rFact*_PIr - _PI2r) + _1r) * _05r;
			return(m_oKeyFrames[_uiCurKey].m_oKey*(_1r-rFact) + m_oKeyFrames[_uiNextKey].m_oKey*(rFact));

			// FEReal rFact1 = _rFact*_rFact;
			// FEReal rFact2 = CFEMath::rSin(rFact*_PI2r);
			// return(m_oKeyFrames[_uiCurKey].m_oKey*(rFact1) + m_oKeyFrames[uiNextKF].m_oKey*(rFact2));
        }
        // break; // to avoid unreachable code "warning"

		default:
		{
			return(m_oKeyFrames[_uiCurKey].m_oKey*(_1r-_rFact) + m_oKeyFrames[_uiNextKey].m_oKey*(_rFact));
		}
		// break; // to avoid unreachable code "warning"
    }
}
// ----------------------------------------------------------------------------
// explicit specialization for FEBool type
template <>
inline FEBool CFEKFBFunc<FEBool>::oLerp(uint _uiCurKey,uint _uiNextKey,const FEReal& _rFact,EFEKFLerpFunc _eLerpFunc)
{
	switch (_eLerpFunc)
	{
		case KFLF_CONSTANT:
		{
			if (_rFact == _1r)
				return(m_oKeyFrames[_uiNextKey].m_oKey);
			else
				return(m_oKeyFrames[_uiCurKey].m_oKey);
		}

		case KFLF_RAND:
		#ifndef N3DS
		{
			if (CFEMath::uiRand(2) == 0)
				return(m_oKeyFrames[_uiCurKey].m_oKey);
			else
				return(m_oKeyFrames[_uiNextKey].m_oKey);

			// da error de compilación
	        // return(random_lerp(m_oKeyFrames[_uiCurKey].m_oKey,m_oKeyFrames[_uiNextKey].m_oKey,_rFact));
	    }
		// break; // to avoid unreachable code "warning"
	    #endif
        case KFLF_SIN:
		case KFLF_ISIN:
        case KFLF_EXP:
		case KFLF_SINSIN:
        case KFLF_LERP:
        case KFLF_EXPLOG:
		default:
		{
			if (_rFact < _05r)
				return(m_oKeyFrames[_uiCurKey].m_oKey);
			else
				return(m_oKeyFrames[_uiNextKey].m_oKey);
		}
		// break; // to avoid unreachable code "warning"
    }
}
// ----------------------------------------------------------------------------
template <class T>
inline const T& CFEKFBFunc<T>::oGetKeyValue(uint _uiKeyFrame)
{
	return(m_oKeyFrames[_uiKeyFrame].m_oKey);
}
// ----------------------------------------------------------------------------
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4700)		// warning C4700: uninitialized local variable 'oT' used
#pragma warning( disable : 6001)		// warning C4700: uninitialized local variable 'oT' used
#endif
// ----------------------------------------------------------------------------
template <class T>
inline T CFEKFBFunc<T>::oGetValue(const FEReal& _rT)
{
	// If the keyframe array is empty return an undefined value.
	if (m_oKeyFrames.size() == 0)
	{
		// return undefined value
		static T oT;
		return(oT);
	}

	// If the array has only one value, return it as if the func is constant.
else if ((m_oKeyFrames.size() == 1) || (_rT==_0r))
    {
		return(oGetKeyValue(0));
	}

	/// Retrieve the proper interval and factor
	FEReal rFact = CFEMath::rAbs(_rT);

	switch (m_eWrapMode)
	{
		case KFBFWM_LOOP:
		{
			if (rFact >= m_rMaxLimit)
			{
				// rFact = fmodf(rFact,m_rMaxLimit);
				uint uiLoops = rFact / m_rMaxLimit;
				rFact = rFact - ((FEReal)uiLoops*m_rMaxLimit);

			    //
                if (rFact == _0r)
                {
                    return(oGetKeyValue(0));
	            }
			}
		}
		break;

		case KFBFWM_INITIALVALUE:
		{
			if (rFact>=m_rMaxLimit)
			{
			    return(oGetKeyValue(0));
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
				return(oGetKeyValue(0));
            }

            // 0 <= rFact < 2*m_rMaxLimit
			if (rFact >= m_rMaxLimit)
				rFact = _2r*m_rMaxLimit - rFact;
		}
		break;


		default:
		case KFBFWM_FINALVALUE:
		{
			if (rFact>=m_rMaxLimit)
			{
				return(oGetKeyValue(m_oKeyFrames.size()-1));
			}
		}
		break;
	}

	//	PRECONDITION:
	//	    fFact >	_0rf
	//	    fFact <= m_fMaxLimit
	//	    fFact <= m_oKeyFrames[m_oKeyFrame.uiNumElems()-1].m_fLimit
    //      m_oKeyFrames.size() > 1

	uint uiNextKF = 1;
	while (rFact>((CFEKeyFrame)m_oKeyFrames[uiNextKF]).rGetKeyTime())
		uiNextKF++;

	uint uiCurKF = uiNextKF-1;

	/// Process factor depending upon the interpolation function for this interval
	FEReal rMin = ((CFEKeyFrame)m_oKeyFrames[uiCurKF]).rGetKeyTime();
	rFact = (rFact - rMin) / ( ((CFEKeyFrame)m_oKeyFrames[uiNextKF]).rGetKeyTime() - rMin);

	return( oLerp(uiCurKF,uiNextKF,rFact,((CFEKeyFrame)m_oKeyFrames[uiCurKF]).eGetLerpFunc()) );
}
// ----------------------------------------------------------------------------
#ifdef _MSC_VER
#pragma warning( pop )
#endif
// ----------------------------------------------------------------------------
template <class T>
inline void CFEKFBFunc<T>::InsertKeyFrame(const T& _oKey,const FEReal& _rKeyTime,EFEKFLerpFunc _eLF)
{
    /// "Invalidate" the optimization.
    Invalidate();

    // we need always a keyframe at instant 0
	if (m_oKeyFrames.size()==0)
	{
		CFETypedKeyFrame<T> oKF( default_value(_oKey), _0r, _eLF);
		m_oKeyFrames.push_back(oKF);
	}

	CFETypedKeyFrame<T> oKF(_oKey,_rKeyTime,_eLF);

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
	#if defined(WIN32) && (! defined(__GNUC__))
	// DMC
	if (m_oKeyFrames.size() > 1)

		//std::sort(&m_oKeyFrames[0],&m_oKeyFrames[0] + m_oKeyFrames.size());
        std::sort(m_oKeyFrames.begin(),m_oKeyFrames.end());
    #endif

	RecalcMaxLimit();
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
