// ----------------------------------------------------------------------------
/*! \class CFEParticleSystem
 *  \brief A class to load Particle System definitions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version _1r
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
//-----------------------------------------------------------------------------

/*
/// The sprite used to render the particles.
CFEString			m_sSprite;

/// Time to live function parameters.
FEReal				m_rFreq;

/// Time to live function parameters.
CFEPSMult			m_oTTL;

/// Angle function parameters.
CFEPSFunc<FEReal>	m_rAngle;

/// Color function parameters.
CFEPSFunc<CFEColor> m_oColor;

/// Size function parameters.
CFEPSFunc<FEReal>	m_rSize;

/// Position function parameters.
CFEPSFunc<CFEVect2>	m_oPos;
*/
// ----------------------------------------------------------------------------
#include "support/config/CFEConfigFile.h"
#include "graphics/Sprite/CFESpriteMgr.h"
#include "CFEParticleSysLoader.h"
#include "support/misc/CFEStringUtils.h"
//-----------------------------------------------------------------------------
static EFEKFBFuncWrapMode eGetWrapMode(const CFEString& _sWrapMode)
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
static EFEKFLerpFunc eGetLerpFunc(const CFEString& _sLerpFunc)
{
     if (_sLerpFunc |= "constant")
        return(KFLF_CONSTANT);

else if (_sLerpFunc |= "sin")
        return(KFLF_SIN);
else if (_sLerpFunc |= "exp")
        return(KFLF_EXP);

else if (_sLerpFunc |= "random")
        return(KFLF_RAND);

else if (_sLerpFunc |= "linear")
        return(KFLF_LERP);

else if (_sLerpFunc |= "sinsin")
        return(KFLF_SINSIN);

else if (_sLerpFunc |= "explog")
        return(KFLF_EXPLOG);

    return(KFLF_CONSTANT);
}
// ----------------------------------------------------------------------------
void LoadFunc(CFEPSFunc<FEReal>* _poPSFunc,const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile)
{   
    _poPSFunc->m_oOfs.m_rRndMin = _oConfigFile.rGetReal(_sPrefix + ".RndOfs.min",_1r);
    _poPSFunc->m_oOfs.m_rRndMax = _oConfigFile.rGetReal(_sPrefix + ".RndOfs.max",_1r);
    _poPSFunc->m_oMult.m_rRndMin= _oConfigFile.rGetReal(_sPrefix + ".RndMult.min",_1r);
    _poPSFunc->m_oMult.m_rRndMax= _oConfigFile.rGetReal(_sPrefix + ".RndMult.max",_1r);
 
    CFEString sVar = _sPrefix + ".KFFunc";
    CFEString sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");

	_poPSFunc->m_oFunc.SetWrapMode( eGetWrapMode(sWrapMode) );
    
    uint uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
    for (uint i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);
        FEReal rValue = _oConfigFile.rGetReal(sIVar + ".Value",_0r);
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
        CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

        _poPSFunc->m_oFunc.InsertKeyFrame(rValue,rTime,eGetLerpFunc(sLerpFunc) );
    }
}
// ----------------------------------------------------------------------------
void LoadFunc(CFEPSFunc<CFEColor>* _poPSFunc,const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile)
{   
    _poPSFunc->m_oOfs.m_rRndMin = _oConfigFile.rGetReal(_sPrefix + ".RndOfs.min",_1r);
    _poPSFunc->m_oOfs.m_rRndMax = _oConfigFile.rGetReal(_sPrefix + ".RndOfs.max",_1r);
    _poPSFunc->m_oMult.m_rRndMin= _oConfigFile.rGetReal(_sPrefix + ".RndMult.min",_1r);
    _poPSFunc->m_oMult.m_rRndMax= _oConfigFile.rGetReal(_sPrefix + ".RndMult.max",_1r);
 
    CFEString sVar = _sPrefix + ".KFFunc";
    CFEString sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");

	_poPSFunc->m_oFunc.SetWrapMode( eGetWrapMode(sWrapMode) );
    
    uint uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
    for (uint i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);        
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
        CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");
        
        CFEColor oColor;
        oColor.r = _oConfigFile.rGetReal(sIVar + ".r",_1r);
        oColor.g = _oConfigFile.rGetReal(sIVar + ".g",_1r);
        oColor.b = _oConfigFile.rGetReal(sIVar + ".b",_1r);
        oColor.a = _oConfigFile.rGetReal(sIVar + ".a",_1r);

        _poPSFunc->m_oFunc.InsertKeyFrame(oColor,rTime,eGetLerpFunc(sLerpFunc) );
    }
}
// ----------------------------------------------------------------------------
CFEParticleSys* CFEParticleSysLoader::poLoad(const CFEString& _sFilename)
{
    CFEString sFilename = _sFilename + ".psf";
	CFEString sWorkingDir = CFEStringUtils::sGetPath(_sFilename);
    CFEConfigFile oConfig(sFilename);

	if (! oConfig.bInitialized() ) return(NULL);
	CFEParticleSys* poPS = new CFEParticleSys;

    CFEString sSprite = sWorkingDir + CFEString("/") + oConfig.sGetString("ParticleSystem.Sprite","");

    poPS->m_poSprite = CFESpriteMgr::poLoad(sSprite);
    poPS->m_rFreq = oConfig.rGetReal("ParticleSystem.Freq",_1r);
    poPS->m_rTTL.m_rRndMin= oConfig.rGetReal("ParticleSystem.MinTTL",_1r);
    poPS->m_rTTL.m_rRndMax= oConfig.rGetReal("ParticleSystem.MaxTTL",_1r);

    LoadFunc(&poPS->m_rXPos,"ParticleSystem.XFunc",oConfig);
    LoadFunc(&poPS->m_rYPos,"ParticleSystem.YFunc",oConfig);
    LoadFunc(&poPS->m_rScale,"ParticleSystem.ScaleFunc",oConfig);
    LoadFunc(&poPS->m_rAngle,"ParticleSystem.AngleFunc",oConfig);
    LoadFunc(&poPS->m_oColor,"ParticleSystem.ColorFunc",oConfig);

	return(poPS);
}
//-----------------------------------------------------------------------------
