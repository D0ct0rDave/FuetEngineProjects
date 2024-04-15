// ----------------------------------------------------------------------------
/*! \class CFEParticleSystem
 *  \brief A class to load Particle System definitions.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
#include "types/CFEKFBFuncUtils.h"
// ----------------------------------------------------------------------------
void LoadFunc(CFEKFBFunc<CFEColor>* _poPSFunc,const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile)
{   
    CFEString sVar = _sPrefix;
    CFEString sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");

	_poPSFunc->SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(sWrapMode) );

    uint uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
    for (uint i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString((int)i);        
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
        CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");
        
        CFEColor oColor;
        oColor.r = _oConfigFile.rGetReal(sIVar + ".r",_1r);
        oColor.g = _oConfigFile.rGetReal(sIVar + ".g",_1r);
        oColor.b = _oConfigFile.rGetReal(sIVar + ".b",_1r);
        oColor.a = _oConfigFile.rGetReal(sIVar + ".a",_1r);

        _poPSFunc->InsertKeyFrame(oColor,rTime,CFEKFBFuncUtils::eGetLerpFunc(sLerpFunc) );
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

    poPS->m_poSprite = CFESpriteMgr::I()->poLoad(sSprite);
    poPS->m_rFreq = oConfig.rGetReal("ParticleSystem.Freq",_1r);
    poPS->m_rTTL  = oConfig.rGetReal("ParticleSystem.TTL",_1r);
	
	// --------------------------------------
	poPS->m_oEmisorRad.m_rRndMin = oConfig.rGetReal("ParticleSystem.EmisorRad.Min",_0r);
	poPS->m_oEmisorRad.m_rRndMax = oConfig.rGetReal("ParticleSystem.EmisorRad.Max",_0r);

	poPS->m_oEmisorPhase.m_rRndMin = oConfig.rGetReal("ParticleSystem.EmisorPhase.Min",_0r);
	poPS->m_oEmisorPhase.m_rRndMax = oConfig.rGetReal("ParticleSystem.EmisorPhase.Max",_0r);
	
	poPS->m_oEmisorSpeed.m_rRndMin = oConfig.rGetReal("ParticleSystem.EmisorSpeed.Min",_0r);
	poPS->m_oEmisorSpeed.m_rRndMax = oConfig.rGetReal("ParticleSystem.EmisorSpeed.Max",_0r);
	
	poPS->m_rEmisorAccel = oConfig.rGetReal("ParticleSystem.EmisorAccel",_0r);
	
	// --------------------------------------
	poPS->m_oEmisorXOfs.m_rRndMin = oConfig.rGetReal("ParticleSystem.EmisorXOfs.Min",_0r);
	poPS->m_oEmisorXOfs.m_rRndMax = oConfig.rGetReal("ParticleSystem.EmisorXOfs.Max",_0r);
	
	poPS->m_oEmisorYOfs.m_rRndMin = oConfig.rGetReal("ParticleSystem.EmisorYOfs.Min",_0r);
	poPS->m_oEmisorYOfs.m_rRndMax = oConfig.rGetReal("ParticleSystem.EmisorYOfs.Max",_0r);
	
	poPS->m_oEmisorXSpeed.m_rRndMin = oConfig.rGetReal("ParticleSystem.EmisorXSpeed.Min",_0r);
	poPS->m_oEmisorXSpeed.m_rRndMax = oConfig.rGetReal("ParticleSystem.EmisorXSpeed.Max",_0r);
	
	poPS->m_oEmisorYSpeed.m_rRndMin = oConfig.rGetReal("ParticleSystem.EmisorYSpeed.Min",_0r);
	poPS->m_oEmisorYSpeed.m_rRndMax = oConfig.rGetReal("ParticleSystem.EmisorYSpeed.Max",_0r);
	
	poPS->m_rEmisorXAccel = oConfig.rGetReal("ParticleSystem.EmisorXAccel",_0r);	
	poPS->m_rEmisorYAccel = oConfig.rGetReal("ParticleSystem.EmisorYAccel",_0r);	
	
	// --------------------------------------
	poPS->m_oPartSize.m_rRndMin = oConfig.rGetReal("ParticleSystem.PartSize.Min",_0r);
	poPS->m_oPartSize.m_rRndMax = oConfig.rGetReal("ParticleSystem.PartSize.Max",_0r);
	
	poPS->m_oPartSizeSpeed.m_rRndMin = oConfig.rGetReal("ParticleSystem.PartSizeSpeed.Min",_0r);
	poPS->m_oPartSizeSpeed.m_rRndMax = oConfig.rGetReal("ParticleSystem.PartSizeSpeed.Max",_0r);
	
	poPS->m_rPartSizeAccel = oConfig.rGetReal("ParticleSystem.PartSizeAccel",_0r);
		
	// --------------------------------------
	poPS->m_oPartAngle.m_rRndMin = oConfig.rGetReal("ParticleSystem.PartAngle.Min",_0r);
	poPS->m_oPartAngle.m_rRndMax = oConfig.rGetReal("ParticleSystem.PartAngle.Max",_0r);
	
	poPS->m_oPartAngleSpeed.m_rRndMin = oConfig.rGetReal("ParticleSystem.PartAngleSpeed.Min",_0r);
	poPS->m_oPartAngleSpeed.m_rRndMax = oConfig.rGetReal("ParticleSystem.PartAngleSpeed.Max",_0r);

	poPS->m_rPartAngleAccel = oConfig.rGetReal("ParticleSystem.PartAngleAccel",_0r);

    LoadFunc(&poPS->m_oColor,"ParticleSystem.ColorFunc",oConfig);

	poPS->m_uiFlags = oConfig.rGetReal("ParticleSystem.Flags",0);
	return(poPS);
}
//-----------------------------------------------------------------------------
