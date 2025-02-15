// ----------------------------------------------------------------------------
/*! \class CFESkelAnimVisitor
 *  \brief A generic class to perform a process over all the elemnts of a HUD hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimNodeVisitorH
#define CFESkelAnimNodeVisitorH
//-----------------------------------------------------------------------------
class CFESkelAnimNode;
class CFESkelAnimSpriteModel;
class CFESkelAnimSprite;
class CFESkelAnimGroup;
class CFESkelAnimBone;
class CFESkelAnimMesh;
class CFESkelAnimMeshModel;
//-----------------------------------------------------------------------------
class CFESkelAnimNodeVisitor
{
    public:

        ///
        virtual void Visit(CFESkelAnimNode* _poObj)
        {
	        // give a default behaviour for undefined objects
	        // DO NOTHING!        
        }

        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
            Visit((CFESkelAnimNode*)_poObj);
        }
        /// 
        virtual void Visit(CFESkelAnimSpriteModel* _poObj)
        {
            Visit((CFESkelAnimNode*)_poObj);
        }
        
        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
            Visit((CFESkelAnimNode*)_poObj);
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
            Visit((CFESkelAnimNode*)_poObj);
        }

        virtual void Visit(CFESkelAnimMeshModel* _poObj)
        {
            Visit((CFESkelAnimNode*)_poObj);
        }        

        /// 
        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
            Visit((CFESkelAnimNode*)_poObj);
        }        
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
