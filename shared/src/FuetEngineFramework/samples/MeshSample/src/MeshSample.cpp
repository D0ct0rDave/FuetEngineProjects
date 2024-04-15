//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
CFERenderer* gpoRenderer;
FEHandler ghMeshInst;
FEHandler ghMeshInst2; 
FEHandler ghMaterial;
float gfAngle = 0.0f;
bool gbLink = true;
// -----------------------------------------------------------------------------
CFEMeshFrame* poCreateFrameMesh(int _iNumSlices, float _fRadius)
{
	CFEMeshFrame* poFrame = new CFEMeshFrame;

    int cI,cJ,iIdx;

    float theta1,theta2,theta3;
    float cs_t1,cs_t2,cs_t3;
    float sn_t1,sn_t2,sn_t3;

    float x1,y1;
    float x2,y2;
    float u,v;

    float fRhoStep = (_PIr) / (_iNumSlices);
    float fPhiStep = (_2PIr) / _iNumSlices;

    float fUStep = _1r / _iNumSlices;
    float fVStep = _1r / _iNumSlices;

    iIdx = 0;

    v = _0r;
    theta1 = -_PI2r;
    theta2 = theta1 + fRhoStep;

    for ( cJ = 0; cJ < _iNumSlices; cJ++ )
    {
        cs_t1 = CFEMath::rCos(theta1);
        cs_t2 = CFEMath::rCos(theta2);
        sn_t1 = CFEMath::rSin(theta1);
        sn_t2 = CFEMath::rSin(theta2);

        u = _0r;
        theta3 = _0r;

        for ( cI = 0; cI < _iNumSlices; cI++ )
        {
            cs_t3 = CFEMath::rCos(theta3);
            sn_t3 = CFEMath::rSin(theta3);

            x1 = _fRadius * cs_t1 * cs_t3;
            y1 = _fRadius * cs_t1 * sn_t3;

            x2 = _fRadius * cs_t2 * cs_t3;
            y2 = _fRadius * cs_t2 * sn_t3;


			CFEVect2 oVX;
			oVX.x = x1;
			oVX.y = y1;
			poFrame->m_oVX.push_back(oVX);

			oVX.x = x2;
			oVX.y = y2;
			poFrame->m_oVX.push_back(oVX);

            u += fUStep;
            theta3 += fPhiStep;
        }

        v += fVStep;
        theta1 += fRhoStep;
        theta2 += fRhoStep;
    }

	poFrame->m_rBlendTime = _05r;
	poFrame->m_rDelay = _05r;
	poFrame->m_rFrameTime = _1r;
	poFrame->m_rStartTime = _0r;

    return(poFrame);
}
// -----------------------------------------------------------------------------
CFEMesh* poCreateMesh()
{
	CFEMesh* poMesh = new CFEMesh;
	CFEMeshAction* poAction = new CFEMeshAction;
	poAction->SetName("lolo");
	poAction->m_rActionTime = _1r;
	poAction->m_ePlayMode = MAPM_LOOP;
	poAction->m_rRandStartTime = _0r;
	
	poAction->m_oSeq.push_back( poCreateFrameMesh(10,600) );
	poAction->m_oSeq.push_back( poCreateFrameMesh(10,500) );
	poAction->m_oSeq[0]->m_rStartTime = _0r;
	poAction->m_oSeq[1]->m_rStartTime = _1r;
	
	poMesh->m_poActions.push_back( poAction );
	
	/// The sprite instance attached to this mesh.
    poMesh->m_hSpriteInst = CFESpriteInstMgr::I()->hGetInstance("eee");

    /// UVs for this mesh
    for (uint i=0;i<poAction->m_oSeq[0]->m_oVX.size();i++)
    {
		CFEVect2 oVX = poAction->m_oSeq[0]->m_oVX[i];
		oVX /= 600;
		poMesh->m_oUV.push_back(oVX);
	}

	return(poMesh);
}
// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{
    gpoRenderer = new CFERenderer;
    gpoRenderer->Init(_pParam);
    gpoRenderer->SetScreenVWidth(1280);
    gpoRenderer->SetScreenVHeight(960);

	ghMaterial = CFEMaterialMgr::I()->hLoad("eee");

    CFEMeshInstMgr::I()->Init(32);
    CFESpriteInstMgr::I()->Init(32);

    CFEVect2 goPos(gpoRenderer->uiGetScreenVWidth()/2,gpoRenderer->uiGetScreenVHeight()/2);
	CFEMeshMgr::I()->Register(poCreateMesh(),"lala");

	for (uint i=0;i<10;i++)
	{
		// ghMeshInst = CFEMeshInstMgr::I()->hGetInstance("sunray");
		ghMeshInst = CFEMeshInstMgr::I()->hGetInstance("test");
		// ghMeshInst = CFEMeshInstMgr::I()->hGetInstance("lala");

		CFEMeshInstMgr::I()->SetPos(ghMeshInst,CFEVect2(CFEMath::rRand(0,1280),CFEMath::rRand(0,960)) );
		CFEMeshInstMgr::I()->Enable(ghMeshInst);
		CFEMeshInstMgr::I()->SetAction(ghMeshInst,0);
		CFEMeshInstMgr::I()->SetScale(ghMeshInst,CFEMath::rRand(50.0f,200.0f) );
		CFEMeshInstMgr::I()->SetColor(ghMeshInst,CFEColor(1,1,1,1.0f));
	}
}
// -----------------------------------------------------------------------------
// Finish Engine
// -----------------------------------------------------------------------------
void FinishEngine()
{
    gpoRenderer->Finish();
}
//-----------------------------------------------------------------------------
// Update Frame
//-----------------------------------------------------------------------------
void LoopEngine()
{
    gpoRenderer->BeginScene();
		
		// gpoRenderer->ZoomCamera(5);
		gpoRenderer->SetBlendMode(BM_ALPHA);
		gpoRenderer->SetMaterial(NULL);
		static float fX = 0.0f;
		static float fSX = 1.0f;

		gpoRenderer->ResetTransform();
		gpoRenderer->TranslateTransform(fX,0.0f);
		gpoRenderer->ScaleTransform(fSX,1.0f);
        gpoRenderer->RenderQuad(0,0,gpoRenderer->uiGetScreenVWidth(),gpoRenderer->uiGetScreenVHeight(),CFEColor(_05r,CFEMath::rMod(gfAngle,_1r),_05r,1));

        FEReal rDeltaT = gpoRenderer->rGetDeltaT();
        gfAngle += 0.25f*rDeltaT;

        // Draw something here
        CFEMeshInstMgr::I()->SetAngle(ghMeshInst,gfAngle);
        CFEMeshInstMgr::I()->Update(rDeltaT);
        CFEMeshInstMgr::I()->Render(gpoRenderer);
		
    gpoRenderer->EndScene();
}
// -----------------------------------------------------------------------------
