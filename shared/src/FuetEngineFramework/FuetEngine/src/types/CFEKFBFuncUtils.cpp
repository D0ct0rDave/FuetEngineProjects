// ----------------------------------------------------------------------------
/*! \class CFEKFBFuncUtils
 *  \brief Key Frame Based Function
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "CFEKFBFuncUtils.h"

using namespace CFEKFBFuncUtils;
//-----------------------------------------------------------------------------
EFEKFBFuncWrapMode CFEKFBFuncUtils::eGetWrapMode(const CFEString& _sWrapMode)
{
    if (_sWrapMode |= "Loop")
        return(KFBFWM_LOOP);

else if (_sWrapMode |= "PingPong")
        return(KFBFWM_PINGPONG);

else if (_sWrapMode |= "InitialValue")
        return(KFBFWM_INITIALVALUE);

else if (_sWrapMode |= "FinalValue")
        return(KFBFWM_FINALVALUE);

	return(KFBFWM_FINALVALUE);
}
//-----------------------------------------------------------------------------
EFEKFLerpFunc CFEKFBFuncUtils::eGetLerpFunc(const CFEString& _sLerpFunc)
{
     if (_sLerpFunc |= "const")
        return(KFLF_CONSTANT);

else if (_sLerpFunc |= "linear")
        return(KFLF_LERP);

else if (_sLerpFunc |= "sin")
        return(KFLF_SIN);

else if (_sLerpFunc |= "invsin")
        return(KFLF_ISIN);

else if (_sLerpFunc |= "exp")
        return(KFLF_EXP);

else if (_sLerpFunc |= "random")
        return(KFLF_RAND);

else if (_sLerpFunc |= "sinsin")
        return(KFLF_SINSIN);

else if (_sLerpFunc |= "explog")
        return(KFLF_EXPLOG);

	return(KFLF_LERP);
}
//-----------------------------------------------------------------------------
CFEString CFEKFBFuncUtils::sGetLerpFuncName(EFEKFLerpFunc _eLerpFunc)
{
    switch (_eLerpFunc)
    {
	    case KFLF_CONSTANT:
	    return("const");
	    // break;	// avoid "statement is un reachable" warning

	    case KFLF_SIN:
	    return("sin");
	    // break;	// avoid "statement is un reachable" warning

	    case KFLF_ISIN:
	    return("invsin");
	    // break;	// avoid "statement is un reachable" warning

	    case KFLF_EXP:
	    return("exp");
	    // break;	// avoid "statement is un reachable" warning

	    case KFLF_RAND:
	    return("random");
        // break;	// avoid "statement is un reachable" warning

	    case KFLF_SINSIN:
	    return("sinsin");
	    // break;	// avoid "statement is un reachable" warning

	    case KFLF_EXPLOG:
	    return("explog");
	    // break;	// avoid "statement is un reachable" warning

        case KFLF_LERP:
        default:
            return("linear");
    };
}
//-----------------------------------------------------------------------------
CFEString CFEKFBFuncUtils::sGetWrapMode(EFEKFBFuncWrapMode _eWrapMode)
{
    switch (_eWrapMode)
    {
        case KFBFWM_LOOP:
            return("Loop");
        // break;	// avoid "statement is un reachable" warning

	    case KFBFWM_PINGPONG:
	        return("PingPong");
	    // break;	// avoid "statement is un reachable" warning

	    case KFBFWM_INITIALVALUE:
	        return("InitialValue");
	    // break;	// avoid "statement is un reachable" warning

	    default:
	    case KFBFWM_FINALVALUE:
	        return("FinalValue");
	    // break;	// avoid "statement is un reachable" warning
    }
}
// ----------------------------------------------------------------------------
#ifdef _DEBUG
static void Test()
{
	CFEKFBFunc<FEReal> oR;
	CFEKFBFunc<FEReal> oC;

	CFEKFBFuncUtils::CopyKeyframedFunc(oR,oC);
}
#endif
// ----------------------------------------------------------------------------
