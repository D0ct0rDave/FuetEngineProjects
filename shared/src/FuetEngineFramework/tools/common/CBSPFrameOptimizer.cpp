// Standard Gen Includes
#include <windows.h>
#include <math.h>
#include <direct.h>
// #include <io.h>
#include <vector>
#include <string>
#include <math.h>
#include <FuetEngine.h>
#include <assert.h>
#include "CBSPFrameOptimizer.h"
// ----------------------------------------------------------------------------
class CRect
{
	public:

		CRect()
		{
		}

		CRect(unsigned int _uiLeft,unsigned int _uiTop,unsigned int _uiRight,unsigned int _uiBottom)
		{
			left = _uiLeft;
			right = _uiRight;
			top = _uiTop;
			bottom = _uiBottom;
			CFESystem::Print("(%d-%d,%d-%d)\n",_uiLeft,_uiRight,_uiTop,_uiBottom);
		}

		unsigned int width()
		{
			return(right-left+1);
		}
		
		unsigned int height()
		{
			return(bottom-top+1);
		}

		unsigned int left;
		unsigned int right;
		unsigned int top;
		unsigned int bottom;
};
// ----------------------------------------------------------------------------
class CBSPNode;
class CBSPNode
{
	public:

		CBSPNode() : m_poGlobalImg(NULL), m_bFilled(false)
		{
			child[0] = child[1] = NULL;
		}
		
		CBSPNode(fipImage* _poGlobalImg,bool _bInitRect = false) : m_bFilled(false)
		{
			m_poGlobalImg = _poGlobalImg;
			child[0] = child[1] = NULL;
			
			if (_bInitRect)
			{				
				r = CRect(0,0,_poGlobalImg->getWidth()-1,_poGlobalImg->getHeight()-1);
			}
		}

		~CBSPNode()
		{
			if (child[0] != NULL) delete child[0];
			if (child[1] != NULL) delete child[1];
		}

		bool bIsLeaf()
		{
			return( (child[0] == NULL) && (child[1] == NULL) );
		}

		uint uiGetArea()
		{
			if (bIsLeaf())
			{
				if (m_bFilled)
					return(0.0f);
				else
					return(r.width() * r.height());
			}
			else
			{
				uint uiArea = 0.0f;
				if (child[0] != NULL) uiArea  = child[0]->uiGetArea();
				if (child[1] != NULL) uiArea += child[1]->uiGetArea();
				return(uiArea);
			}
		}

		CBSPNode* poInsert(fipImage* _poFrame);

	public:

		CBSPNode*	child[2];
		CRect		r;
		fipImage*	m_poGlobalImg;
		bool		m_bFilled;
};
// ----------------------------------------------------------------------------
CBSPFrameOptimizer::CBSPFrameOptimizer() : m_poBSP(NULL)
{

}
// ----------------------------------------------------------------------------
// Here comes the "magic"
CBSPNode* CBSPNode::poInsert(fipImage* _poFrame)
{
	// +1 1 pixel of margin
	uint uiFrameWidth  = _poFrame->getWidth() + 1;
	uint uiFrameHeight = _poFrame->getHeight() + 1;

	// if we're not a leaf then
	if ( ! bIsLeaf() )
	{
		/*
		// (try inserting into first child)
		CBSPNode* poNewNode = child[0]->poInsert( _poFrame );
		if (poNewNode != NULL) return poNewNode;

		// (no room, insert into second)
		return child[1]->poInsert( _poFrame );
		*/

		uint uiRequiredArea = uiFrameWidth*uiFrameHeight;
		uint uiChild0Area   = child[0]->uiGetArea();
		uint uiChild1Area   = child[1]->uiGetArea();
		
		if (uiRequiredArea > uiChild0Area)
		{
			// No room in the first child try the second one.
			if (uiRequiredArea < uiChild1Area)
				return child[1]->poInsert( _poFrame );
			else
				return(NULL);
		}
		else if (uiRequiredArea > uiChild1Area)
		{
			// No room in the first child try the second one.
			return child[0]->poInsert( _poFrame );
		}
		else
		{
			// There is room in both childs.
			// Try inserting into the smallest one.
			if (uiChild0Area <= uiChild1Area)
			{
				CBSPNode* poNewNode = child[0]->poInsert( _poFrame );
				if (poNewNode != NULL) return poNewNode;

				// (no room, insert into second)
				return child[1]->poInsert( _poFrame );
			}
			else
			{
				CBSPNode* poNewNode = child[1]->poInsert( _poFrame );
				if (poNewNode != NULL) return poNewNode;

				// (no room, insert into second)
				return child[0]->poInsert( _poFrame );
			}
		}
	}
	else
	{
		if (m_bFilled) return(NULL);

		// (if we're too small, return)
		// if img doesn't fit in pnode->rect
		if (
			(uiFrameWidth  > r.width())
			||
			(uiFrameHeight > r.height())
			)
			return NULL;

		// (if we're just right, accept)
		// if img fits perfectly in this->rect
		if (
			(uiFrameWidth  == r.width())
			&&
			(uiFrameHeight == r.height())
			)
		{
			// we've found a hole: put the frame there.
			m_bFilled = true;
			m_poGlobalImg->pasteSubImage(*_poFrame,r.left,r.top);
			return this;
		}

		// (otherwise, gotta split this node and create some kids)
		child[0] = new CBSPNode(m_poGlobalImg);
		child[1] = new CBSPNode(m_poGlobalImg);

		// (decide which way to split)
		int dw = r.width()  - uiFrameWidth;
		int dh = r.height() - uiFrameHeight;

		if (dw >= dh)
		{
			child[0]->r = CRect(r.left, r.top, 
							    r.left+uiFrameWidth-1,r.bottom);
			child[1]->r = CRect(r.left+uiFrameWidth, r.top,
								r.right, r.bottom);
		}
		else
		{
			child[0]->r = CRect(r.left, r.top, 
								r.right,r.top+uiFrameHeight-1);
			child[1]->r = CRect(r.left, r.top+uiFrameHeight,
								r.right, r.bottom);
        }

		// (insert into first child we created)
		return child[0]->poInsert( _poFrame);
	}
}
// ----------------------------------------------------------------------------
bool CBSPFrameOptimizer::bInsertFrame(fipImage* _poFrame,COptiFrameSequence* _poOFS)
{
	if ((_poFrame->getWidth() == 0) || (_poFrame->getHeight() == 0)) return(true);

	// Create root BSP node?
	if (m_poBSP == NULL)
	{
		if (! ((_poOFS->m_oFrameSeq.getWidth()>0) && (_poOFS->m_oFrameSeq.getHeight()>0)) )
		{
			CFESystem::Print("Image with invalid image dimensions found.");
			return(false);
		}

		m_poBSP = new CBSPNode(&_poOFS->m_oFrameSeq,true);
	}
	
	CBSPNode* poNode = m_poBSP->poInsert(_poFrame);
	if (poNode != NULL)
	{
		CFERect r;
		r.m_oIni.x = poNode->r.left;
		r.m_oIni.y = poNode->r.top;
		r.m_oEnd.x = poNode->r.right-1;		// 1 pixel margin
		r.m_oEnd.y = poNode->r.bottom-1;		// 1 pixel margin
		
		_poOFS->m_oRects.push_back(r);
		return(true);
	}
	else
		return(false);
}
// ----------------------------------------------------------------------------
void CBSPFrameOptimizer::ReinitProcess()
{
	if (m_poBSP != NULL)
	{
		delete m_poBSP;
		m_poBSP = NULL;
	}
}
// ----------------------------------------------------------------------------
