// ----------------------------------------------------------------------------
/*! \class CFEParticleMgr
 *  \brief Particle system clases and definitions.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
static uint uiCP = 0;
static uint uiDP = 0;
//-----------------------------------------------------------------------------
class CFEParticle
{

	protected:

		//
		CFEParticleSys*	    m_poPS;

		/// Initial TTL of the particle
		FEReal				m_rTTL;

		/// Current Time of the particle
		FEReal				m_rTime;
		
		/// Initial pos
		CFEVect2			m_oIPos;
		
		/// Initial speed
		CFEVect2			m_oISpeed;

		/// Initial Acceleration
		CFEVect2			m_oIAccel;
		
		/// Initial size
		FEReal				m_rISize;
	
		/// Initial size speed
		FEReal				m_rISSpeed;

		/// Initial size accel
		// FEReal				m_rISAccel;

		/// Initial angle
		FEReal				m_rIAngle;

		/// Initial angle speed
		FEReal				m_rIASpeed;
		
		/// Initial angle accel
		// FEReal				m_rIAAccel;

    protected:

        //
        FEReal              m_rDepth;
        FEHandler			m_hPSysInst;
		CFEVect2*			m_poPSysSpeed;

	public:

		// Sprite instances are self managed by the particles.
		CFESpriteInst*		m_poSprInst;

	public:

		CFEParticle()
		{
			// WARNING: This casting is ugly...
			m_poSprInst = (CFESpriteInst*)CFESpriteInstMgr::I()->poCreateInstance();			
		};

		~CFEParticle()
		{
			// WARNING: This casting is ugly...
			CFESpriteInstMgr::I()->DestroyInstance((CFESpriteMgrInst*)m_poSprInst);
		};

		void Init(const CFEVect2& _oPos,const FEReal& _rDepth, const CFEVect2& _oSpd, CFEParticleSys*	_poPS,FEHandler _hPSysInst)
		{
			m_hPSysInst	    = _hPSysInst;
			m_poPS	        = _poPS;
            m_rDepth        = _rDepth;

			m_rTime         = _0r;
			m_rTTL		    = m_poPS->m_rTTL;

			CFEVect2 oD = CFEVect2::X();
			oD.Rotate( m_poPS->m_oEmisorPhase.rGetValue() );

			// Initial position
			m_oIPos = _oPos + oD*m_poPS->m_oEmisorRad.rGetValue() + CFEVect2(m_poPS->m_oEmisorXOfs.rGetValue(),m_poPS->m_oEmisorYOfs.rGetValue());

			if (m_poPS->m_uiFlags & PSF_TANGENTIAL_EMISION)
				oD.Rotate(_PI2r);

			/// Initial speed
			FEReal rIXSpeed = m_poPS->m_oEmisorXSpeed.rGetValue() + _oSpd.x;
			FEReal rIYSpeed = m_poPS->m_oEmisorYSpeed.rGetValue() + _oSpd.y;
			FEReal rIESpeed = m_poPS->m_oEmisorSpeed.rGetValue();
			m_oISpeed = oD*rIESpeed + CFEVect2(rIXSpeed,rIYSpeed);

			/// Initial Acceleration
			FEReal rIXAccel = m_poPS->m_rEmisorXAccel; // *CFEMath::rSign(rIXSpeed);
			FEReal rIYAccel = m_poPS->m_rEmisorYAccel; // *CFEMath::rSign(rIYSpeed);
			FEReal rIEAccel = m_poPS->m_rEmisorAccel;  // *CFEMath::rSign(rIESpeed);
			m_oIAccel = oD*rIEAccel + CFEVect2(rIXAccel,rIYAccel);

			/// Initial size
			m_rISize = m_poPS->m_oPartSize.rGetValue();

			/// Initial size speed
			m_rISSpeed = m_poPS->m_oPartSizeSpeed.rGetValue();

			/// Initial size accel
			// m_rISAccel = m_poPS->m_rPartSizeAccel*CFEMath::rSign(m_rISSpeed);

			/// Initial angle
			m_rIAngle = m_poPS->m_oPartAngle.rGetValue();

			/// Initial angle speed
			m_rIASpeed = m_poPS->m_oPartAngleSpeed.rGetValue();

			/// Initial angle accel
			// m_rIAAccel = m_poPS->m_rPartAngleAccel*CFEMath::rSign(m_rIASpeed);

			if ((_poPS->m_poSprite!=NULL) && (_poPS->m_poSprite->poGetAction(0) != NULL))
				m_poSprInst->m_rActionTime = CFEMath::rRand() * _poPS->m_poSprite->poGetAction(0)->m_rRandStartTime;
			else
				m_poSprInst->m_rActionTime = _0r;

			m_poSprInst->m_poSprite               = _poPS->m_poSprite;
            m_poSprInst->m_uiCurrentActionFrame   = 0;
            m_poSprInst->m_uiSpriteAction         = 0;
		}

		CFEVect2 oGetPos()
		{
			CFEVect2 oSpd = m_oISpeed + m_oIAccel*m_rTime;
		    CFEVect2 oPos = m_oIPos   + oSpd*m_rTime;
			return( oPos );
		}

		CFEVect2 oGetScale()
		{
			// FEReal rSizeSpeed = m_rISSpeed + m_rISAccel*m_rTime;
			FEReal rSizeSpeed = m_rISSpeed + m_poPS->m_rPartSizeAccel*m_rTime;
		    FEReal rScale = m_rISize + rSizeSpeed*m_rTime;
		    CFEVect2 oScale(rScale,rScale);
			return( oScale );
		}

		FEReal rGetAngle()
		{
			// FEReal rAngleSpeed = m_rIASpeed + m_rIAAccel*m_rTime;
			FEReal rAngleSpeed = m_rIASpeed + m_poPS->m_rPartAngleAccel*m_rTime;
			FEReal rAngle = m_rIAngle + rAngleSpeed*m_rTime;
			return( rAngle );
		}

		CFEColor oGetColor()
		{
			return( m_poPS->m_oColor.oGetValue(m_rTime) );
		}

		void Update(const FEReal& _rDeltaT)
		{
		    m_rTime += _rDeltaT;
		}

		FEBool bAlive()
		{
		    return(m_rTime <= m_rTTL);
		}

        CFESpriteInst* poGetSpriteIsnt()
        {
            return( m_poSprInst );
        }

        FEReal rGetDepth()
        {
            return(m_rDepth);
        }
        
        FEHandler hGetPSysInst()
        {
			return(m_hPSysInst);
        }
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
    for (uint i=0;i<INSTANCEDDATA->m_uiMaxParticles;i++)
        INSTANCEDDATA->m_oParticlePool.iAdd( new CFEParticle() );
}
//-----------------------------------------------------------------------------
void CFEParticleMgr::Finish()
{
    for (uint i=0;i<INSTANCEDDATA->m_uiMaxParticles;i++)
        delete INSTANCEDDATA->m_oParticlePool.poGetAt(i);

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
void CFEParticleMgr::CreateParticles(uint _uiNumParticles,const CFEVect2& _oPos, const FEReal& _rDepth, const CFEVect2& _oSpd,CFEParticleSys* _poPS,FEHandler _hPSysInst)
{
    for (uint i=0;i<_uiNumParticles;i++)
    {
        CFEParticle* poPart = INSTANCEDDATA->m_oParticlePool.poGetResource();
        if (poPart != NULL)
        {
            uiCP++;
            poPart->Init(_oPos, _rDepth, _oSpd, _poPS, _hPSysInst);
            INSTANCEDDATA->m_oLivingParts.push_back(poPart);
        }
    }
}
//-----------------------------------------------------------------------------
/// Updates the living particles in the system.
void CFEParticleMgr::Update(const FEReal& _rDeltaT)
{
    for (uint i=0;i<INSTANCEDDATA->m_oLivingParts.size();)
    {
        CFEParticle* poPart = INSTANCEDDATA->m_oLivingParts[i];
        poPart->Update(_rDeltaT);

        if (poPart->bAlive())
        {
            CFESpriteInstMgr::I()->Update(poPart->poGetSpriteIsnt(), _rDeltaT);
            i++;
        }
        else
        {
            // Delete
            INSTANCEDDATA->m_oLivingParts.Delete(i);

            // 
            INSTANCEDDATA->m_oParticlePool.Release(poPart);

            uiDP++;
        }
    }
}
//-----------------------------------------------------------------------------
/// Renders the living particles in the system.
void CFEParticleMgr::Render(CFERenderer* _poRenderer,FEHandler _hPSysInst)
{
    for (uint i=0;i<INSTANCEDDATA->m_oLivingParts.size();i++)
    {
        CFEParticle* poPart = INSTANCEDDATA->m_oLivingParts[i];
//        if (_hPSysInst != poPart->hGetPSysInst()) continue;

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
