//-----------------------------------------------------------------------------
#include "CInteractionMgr.h"
#include "CAttackChecker.h"
#include "Game/CGameGlobals.h"
//-----------------------------------------------------------------------------
void CInteractionMgr::Init()
{
}
//-----------------------------------------------------------------------------
void CInteractionMgr::Finish()
{
}
//-----------------------------------------------------------------------------
void CInteractionMgr::EnemyAttack(CEntity* _poEnt,const TEntityAttack& _oAttack)
{
	CAttackChecker oAttackChecker;
	oAttackChecker.SetAttack(_oAttack);
	oAttackChecker.Check(_poEnt);
}
//-----------------------------------------------------------------------------
void CInteractionMgr::ObjectCollided(CEntity* _poEnt,const TEntityAttack& _oAttack)
{
	CAttackChecker oAttackChecker;
	oAttackChecker.SetAttack(_oAttack);
	oAttackChecker.Check(_poEnt);
}
//-----------------------------------------------------------------------------
void CInteractionMgr::PlayerAttack(const TEntityAttack& _oAttack)
{
	CAttackChecker oAttackChecker;
	oAttackChecker.SetAttack(_oAttack);
	oAttackChecker.Check(goGameGlobals.m_poPlayer);
}
//-----------------------------------------------------------------------------
