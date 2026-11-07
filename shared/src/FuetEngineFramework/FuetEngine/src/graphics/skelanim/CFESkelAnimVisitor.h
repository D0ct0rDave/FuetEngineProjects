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
#ifndef CFESkelAnimVisitorH
#define CFESkelAnimVisitorH
//-----------------------------------------------------------------------------
class CFESkelAnimNode;
class CFESkelAnimSprite;
class CFESkelAnimGroup;
class CFESkelAnimNodeAction;
class CFESkelAnimAction;
class CFESkelAnim;
//-----------------------------------------------------------------------------
class CFESkelAnimVisitor
{
    public:
        /// 
        virtual void Visit(CFESkelAnimNode* _poObj) = 0;
        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj) = 0;
        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj) = 0;
        /// 
        virtual void Visit(CFESkelAnimNodeAction* _poObj) = 0;
        /// 
        virtual void Visit(CFESkelAnimAction* _poObj) = 0;
        /// 
        virtual void Visit(CFESkelAnim* _poObj) = 0;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
