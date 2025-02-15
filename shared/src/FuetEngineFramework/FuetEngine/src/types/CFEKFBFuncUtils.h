// ----------------------------------------------------------------------------
/*! \class CFEKFBFuncUtils
 *  \brief Key Frame Based Function Utilities.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEKFBFuncUtilsH
#define CFEKFBFuncUtilsH
// ----------------------------------------------------------------------------
#include "CFEKFBFunc.h"
// ----------------------------------------------------------------------------
namespace CFEKFBFuncUtils
{
	// Makes a copy of the source function into the destination one.
	template <class T>
	void CopyKeyframedFunc(CFEKFBFunc<T>& _oDstFunc,CFEKFBFunc<T>& _oSrcFunc);

	/// Retrieves the string identifying the given lerp function.
	CFEString sGetLerpFuncName(EFEKFLerpFunc _eLerpFunc);
	
	/// Retrieves the lerp function identified by the given string.
	EFEKFLerpFunc eGetLerpFunc(const CFEString& _sLerpFunc);

	/// Retrieves the wrap mode identified by the given string.
	EFEKFBFuncWrapMode eGetWrapMode(const CFEString& _sWrapMode);

	/// Retrieves the string identified by the given wrap mode.
	CFEString sGetWrapMode(EFEKFBFuncWrapMode _eWrapMode);
}



// ----------------------------------------------------------------------------
// Makes a copy of the source function into the destination one.
// ----------------------------------------------------------------------------
template <class T>
inline void CFEKFBFuncUtils::CopyKeyframedFunc(CFEKFBFunc<T>& _oDstFunc,CFEKFBFunc<T>& _oSrcFunc)
{
	_oDstFunc.Reset();
	for (uint i=0;i<_oSrcFunc.uiGetNumKeyFrames();i++)
	{
		CFETypedKeyFrame<T>* poKF = _oSrcFunc.poGetKeyFrameData(i);
		_oDstFunc.InsertKeyFrame(poKF->m_oKey,poKF->rGetKeyTime(),poKF->eGetLerpFunc());
	}
	_oDstFunc.SetWrapMode(_oSrcFunc.eGetWrapMode());
}

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
