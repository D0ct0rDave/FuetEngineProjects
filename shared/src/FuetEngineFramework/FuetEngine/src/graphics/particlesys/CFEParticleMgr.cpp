// ----------------------------------------------------------------------------
/*! \class CFEParticleMgr
 *  \brief Particle system clases and definitions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEParticleMgr.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/sprite/CFESpriteRenderer.h"
#include "graphics/sprite/CFESprite.h"
#include "support/graphics/CFERenderer.h"
#include "types/FEKFBFLerpFuncs.h"
#include "types/CFESingleton.h"
//-----------------------------------------------------------------------------
class CFEParticle
{
	public:

		void Init(const CFEVect2& _oPos,FEReal _rDepth, CFEParticleSys*	_poPS)
		{	    
			m_poPS	        = _poPS;
            m_rDepth        = _rDepth;

			m_rTime         = _0r;
			m_rTTL		    = m_poPS->m_rTTL.rGetValue();

			m_rAngleOfs     = m_poPS->m_rAngle.m_oOfs.rGetValue() * m_poPS->m_rAngle.oGetValue(0);
			m_rScaleOfs	    = m_poPS->m_rScale.m_oOfs.rGetValue() * m_poPS->m_rScale.oGetValue(0);
			m_rXPosOfs	    = m_poPS->m_rXPos.m_oOfs.rGetValue()  * m_poPS->m_rXPos.oGetValue(0) + _oPos.x; // instead of adding initial position in oGetPos function.
			m_rYPosOfs	    = m_poPS->m_rYPos.m_oOfs.rGetValue()  * m_poPS->m_rYPos.oGetValue(0) + _oPos.y; // instead of adding initial position in oGetPos function.

			m_rColorMult    = _1r - m_poPS->m_oColor.m_oMult.rGetValue();
			m_rAngleMult    = _1r - m_poPS->m_rAngle.m_oMult.rGetValue();
			m_rScaleMult	= _1r - m_poPS->m_rScale.m_oMult.rGetValue();
			m_rXPosMult	    = _1r - m_poPS->m_rXPos.m_oMult.rGetValue();
			m_rYPosMult	    = _1r - m_poPS->m_rYPos.m_oMult.rGetValue();

			m_oSprInst.m_poSprite               = _poPS->m_poSprite;
            m_oSprInst.m_rActionTime            = 0;
            m_oSprInst.m_uiCurrentActionFrame   = 0;
            m_oSprInst.m_uiSpriteAction         = 0;
		}

		CFEVect2 oGetPos()
		{
		    CFEVect2 oPos(  m_rXPosOfs + m_poPS->m_rXPos.oGetValue(m_rTime) * m_rXPosMult, 
		                    m_rYPosOfs + m_poPS->m_rYPos.oGetValue(m_rTime) * m_rYPosMult);

			return( oPos );
		}

		CFEVect2 oGetScale()
		{
		    FEReal rScale = m_rScaleOfs + m_poPS->m_rScale.oGetValue(m_rTime) * m_rScaleMult;
		    CFEVect2 oScale(rScale,rScale);
			return( oScale );
		}

		FEReal rGetAngle()
		{
			return( m_rAngleOfs + m_poPS->m_rAngle.oGetValue(m_rTime) * m_rAngleMult);
		}

		CFEColor oGetColor()
		{
			return( m_poPS->m_oColor.oGetValue(m_rTime) * m_rColorMult);
		}

		void Update(FEReal _rDeltaT)
		{
		    m_rTime += _rDeltaT;
		}

		bool bAlive()
		{
		    return(m_rTime <= m_rTTL);
		}

        CFESpriteInst* poGetSpriteIsnt()
        {
            return( &m_oSprInst );
        }

        FEReal rGetDepth()
        {
            return(m_rDepth);
        }

	protected:

		//
		CFEParticleSys*	    m_poPS;

		/// Initial TTL of the particle
		FEReal				m_rTTL;

		/// Current Time of the particle
		FEReal				m_rTime;


		// Multipliers
		FEReal				m_rColorMult;

		//
		FEReal				m_rAngleMult;

		//
		FEReal				m_rScaleMult;

		//
		FEReal				m_rXPosMult;

		//
		FEReal				m_rYPosMult;
		


		// Offsets
		FEReal				m_rAngleOfs;

		//
		FEReal				m_rScaleOfs;

		//
		FEReal				m_rXPosOfs;

		//
		FEReal				m_rYPosOfs;

    protected:

        //
        FEReal              m_rDepth;

	public:

		// Sprite instances are self managed by the particles.
		CFESpriteInst       m_oSprInst;
};
// ----------------------------------------------------------------------------
DECLARE_SINGLETON(CFEParticleMgr_DATA)
{
    public:

        CFEParticleMgr_DATA() : m_uiMaxParticles(0)
        {
        }

        uint                            m_uiMaxParticles;
        CFEResourceQueue<CFEParticle>   m_oParticlePool;
        CFEArray<CFEParticle*>          m_oLivingParts;
};
#define INSTANCEDDATA CFEParticleMgr_DATA::I()
//-----------------------------------------------------------------------------
/// Main initialiation procedure.
void CFEParticleMgr::Init(uint _uiMaxParticles)
{
    INSTANCEDDATA->m_uiMaxParticles = _uiMaxParticles;
    INSTANCEDDATA->m_oParticlePool.Init(INSTANCEDDATA->m_uiMaxParticles);

    for (uint i=0;i<INSTANCEDDATA->m_uiMaxParticles;i++)
        INSTANCEDDATA->m_oParticlePool.iAdd( new CFEParticle() );
}
//-----------------------------------------------------------------------------
void CFEParticleMgr::Finish()
{
    for (uint i=0;i<INSTANCEDDATA->m_uiMaxParticles;i++)
        delete INSTANCEDDATA->m_oParticlePool.poGet(i);

    INSTANCEDDATA->m_oParticlePool.Finish();
    INSTANCEDDATA->Finish();
}
//-----------------------------------------------------------------------------
void CFEParticleMgr::Reset()
{
    while ( INSTANCEDDATA->m_oLivingParts.size()>0 )
    {
        CFEParticle* poPart = INSTANCEDDATA->m_oLivingParts[0];

        // Delete
        INSTANCEDDATA->m_oLivingParts.Delete(0);

        // 
        INSTANCEDDATA->m_oParticlePool.Release(poPart);
    }
}
//-----------------------------------------------------------------------------
/// Creates the given number of particles.
void CFEParticleMgr::CreateParticles(uint _uiNumParticles,const CFEVect2& _oPos, FEReal _rDepth,CFEParticleSys* _poPS)
{
    for (uint i=0;i<_uiNumParticles;i++)
    {
        CFEParticle* poPart = INSTANCEDDATA->m_oParticlePool.poGetResource();
        if (poPart != NULL)
        {
            poPart->Init(_oPos, _rDepth, _poPS);
            INSTANCEDDATA->m_oLivingParts.push_back(poPart);
        }
    }
}
//-----------------------------------------------------------------------------
/// Updates the living particles in the system.
void CFEParticleMgr::Update(FEReal _rDeltaT)
{
    for (uint i=0;i<INSTANCEDDATA->m_oLivingParts.size();)
    {
        CFEParticle* poPart = INSTANCEDDATA->m_oLivingParts[i];
        poPart->Update(_rDeltaT);

        if (poPart->bAlive())
        {
            CFESpriteInstMgr::Update(poPart->poGetSpriteIsnt(), _rDeltaT);
            i++;
        }
        else
        {
            // Delete
            INSTANCEDDATA->m_oLivingParts.Delete(i);

            // 
            INSTANCEDDATA->m_oParticlePool.Release(poPart);
        }
    }
}
//-----------------------------------------------------------------------------
/// Renders the living particles in the system.
void CFEParticleMgr::Render(CFERenderer* _poRenderer)
{
    for (uint i=0;i<INSTANCEDDATA->m_oLivingParts.size();i++)
    {
        CFEParticle* poPart = INSTANCEDDATA->m_oLivingParts[i];
        CFESpriteRenderer::Render(  _poRenderer,
                                    poPart->poGetSpriteIsnt(),
                                    poPart->oGetPos(),
                                    poPart->rGetDepth(),
                                    poPart->oGetScale(),
                                    poPart->rGetAngle(),
                                    poPart->oGetColor());
    }
}
//-----------------------------------------------------------------------------
