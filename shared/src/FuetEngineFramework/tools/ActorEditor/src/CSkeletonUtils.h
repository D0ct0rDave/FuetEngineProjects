//---------------------------------------------------------------------------
//
// Name:        CSkeletonUtils.h
// Author:      David Márquez de la Cruz
// Created:     7/10/2010
// Description: CSkeleton class declaration
//
//---------------------------------------------------------------------------
#ifndef CSkeletonUtilsH
#define CSkeletonUtilsH
//---------------------------------------------------------------------------
#include <FuetEngine.h>
//----------------------------------------------------------------------------
class CLocalPosGetter : public CFESkelAnimNodeVisitor
{
	public:

		CLocalPosGetter(const CFEVect2& _oAbsPos,const CFEString& _sObjName,FEReal _rTime,CFESkelAnim* _poAnim);
		
		static CFEVect2 oGetPos(const CFEVect2& _oAbsPos,const CFEString& _sObjName,FEReal _rTime,FEHandler _hAnimInst);

		/// 
		virtual void Visit(CFESkelAnimGroup* _poObj);

		/// 
		virtual void Visit(CFESkelAnimBone* _poObj);

		/// 
		virtual void Visit(CFESkelAnimSpriteModel* _poObj);

		/// 
		virtual void Visit(CFESkelAnimSprite* _poObj);

		/// 
		virtual void Visit(CFESkelAnimMeshModel* _poObj);

		/// 
		virtual void Visit(CFESkelAnimMesh* _poObj);

	protected:

		// -------------
		template <class T>
		inline CFEVect2 oComputePos(T* )
		{
			CFEMatrix oInvMat = m_oMat;
			oInvMat.Inverse();
			return( oInvMat.Transform(m_oAbsPos) );
		}

		bool			m_bFound;
		FEReal			m_rTime;
		CFEString		m_sObjName;		
		CFEMatrix		m_oMat;
		CFEVect2		m_oPos;
		CFEVect2		m_oAbsPos;

		// To be probably removed
		CFESkelAnim*	m_poAnim;
		CFESkelAnimNodeAction* poGetNodeAction(CFESkelAnimNode* _poNode);
};
//----------------------------------------------------------------------------
class CAbsPosGetter : public CFESkelAnimNodeVisitor
{
	public:

		CAbsPosGetter(const CFEVect2& _oLocalPos,const CFEString& _sObjName,FEReal _rTime,CFESkelAnim* _poAnim);

		static CFEVect2 oGetPos(const CFEVect2& _oLocalPos,const CFEString& _sObjName,FEReal _rTime,FEHandler _hAnimInst);

		static CFEVect2 oGetPos(const CFEString& _sObjName,FEReal _rTime,FEHandler _hAnimInst);
		
		/// 
		virtual void Visit(CFESkelAnimGroup* _poObj);

		/// 
		virtual void Visit(CFESkelAnimBone* _poObj);

		/// 
		virtual void Visit(CFESkelAnimSpriteModel* _poObj);

		/// 
		virtual void Visit(CFESkelAnimSprite* _poObj);

		/// 
		virtual void Visit(CFESkelAnimMeshModel* _poObj);

		/// 
		virtual void Visit(CFESkelAnimMesh* _poObj);

	protected:

		// -------------
		template <class T>
		inline CFEVect2 oComputePos(T* _poObj)
		{
			CFEVect2 oPos = m_oMat.Transform( _poObj->oGetPos() );
			return( oPos + m_oLocalPos  );
		}

		bool			m_bFound;
		FEReal			m_rTime;
		CFEString		m_sObjName;
		CFEMatrix		m_oMat;
		CFEVect2		m_oPos;
		CFEVect2		m_oLocalPos;
		bool			m_bOfPos;
		
		// To be probably removed
		CFESkelAnim*	m_poAnim;
		CFESkelAnimNodeAction* poGetNodeAction(CFESkelAnimNode* _poNode);		
};
//----------------------------------------------------------------------------
class CBoneLocalPosGetter : public CFESkelAnimNodeVisitor
{
	public:

		CBoneLocalPosGetter(const CFEVect2& _oAbsPos,const CFEString& _sObjName);
		
		static CFEVect2 oGetPos(const CFEVect2& _oAbsPos,CFESkelAnimNode* _poNode,const CFEString& _sObjName);

		/// 
		virtual void Visit(CFESkelAnimGroup* _poObj);

		/// 
		virtual void Visit(CFESkelAnimBone* _poObj);

		/// 
		virtual void Visit(CFESkelAnimSpriteModel* _poObj);

		/// 
		virtual void Visit(CFESkelAnimSprite* _poObj);

		/// 
		virtual void Visit(CFESkelAnimMeshModel* _poObj);

		/// 
		virtual void Visit(CFESkelAnimMesh* _poObj);

	protected:

		// -------------
		template <class T>
		inline CFEVect2 oComputePos(T* _poObj)
		{
			CFEMatrix oInvMat = m_oMat;
			oInvMat.Inverse();
			return( oInvMat.Transform(m_oAbsPos) );
		}

		bool			m_bFound;
		CFEString		m_sObjName;
		CFEMatrix		m_oMat;
		CFEVect2		m_oPos;
		CFEVect2		m_oAbsPos;
};
//----------------------------------------------------------------------------
class CBoneAbsPosGetter : public CFESkelAnimNodeVisitor
{
	public:

		CBoneAbsPosGetter(const CFEString& _sObjName);
		
		static CFEVect2 oGetPos(CFESkelAnimNode* _poNode,const CFEString& _sObjName);

		/// 
		virtual void Visit(CFESkelAnimGroup* _poObj);

		/// 
		virtual void Visit(CFESkelAnimBone* _poObj);

		/// 
		virtual void Visit(CFESkelAnimSpriteModel* _poObj);

		/// 
		virtual void Visit(CFESkelAnimSprite* _poObj);

		/// 
		virtual void Visit(CFESkelAnimMeshModel* _poObj);

		/// 
		virtual void Visit(CFESkelAnimMesh* _poObj);

	protected:

		// -------------
		template <class T>
		inline CFEVect2 oComputePos(T* _poObj)
		{
			return( m_oMat.Transform( _poObj->oGetPos() ) );
		}
		
		bool			m_bFound;
		CFEString		m_sObjName;
		CFEMatrix		m_oMat;
		CFEVect2		m_oPos;
		CFEVect2		m_oAbsPos;
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
