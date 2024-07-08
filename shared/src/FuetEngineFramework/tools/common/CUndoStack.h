//---------------------------------------------------------------------------
//
// Name:        CUndoStack.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#ifndef CUndoStackH
#define CUndoStackH
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------
template <class T>
class CUndoStack
{
    public:

		/// Default constructor for the class.
		CUndoStack() : m_iHeaderPos(-1)
		{
		}
		
		/// Default destructor of the class.
		~CUndoStack()
		{
			if (uiNumPushedStates()>0)
				TrimTo(0);
		}

		/// Pushes a state into the stack.
		void Push(T* _poState);

		/// Pops a state from the stack. Moves the current header position one step down. Swaps the given state into the header position.
		T* poPop(T* _poState);

		/// Moves the current header position one step up.
		T* poUnpop(T* _poState);

		/// Returns true if is possible to unpop more times. Swaps the given state into the header position.
		bool bCanUnpop()
		{
			return(m_iHeaderPos < ((int)m_oStack.size() - 1));
		}

		/// Returns true if is possible to pop more times.
		bool bCanPop()
		{
			return(m_iHeaderPos > -1);
		}

		/// Retrieves the number of states pushed into the stack.
		unsigned int uiNumPushedStates()
		{
			return(m_oStack.size());
		}

		/// Erases old pushed states.
		void TrimTo(unsigned int _uiNumStates);

	protected:

		int				m_iHeaderPos;
		std::vector<T*>	m_oStack;
};
//---------------------------------------------------------------------------
/// Pushes a state into the stack.
template <class T>
inline void CUndoStack<T>::Push(T* _poState)
{
	while (m_oStack.size() > size_t(m_iHeaderPos+1))
	{
		// poped states from the stack. Trim it.
		delete m_oStack[m_oStack.size()-1];
		m_oStack.erase(m_oStack.begin()+m_oStack.size()-1);
	}

	m_oStack.push_back(_poState);
	m_iHeaderPos++;
}
//---------------------------------------------------------------------------
/// Pops a state from the stack. Moves the current header position one step down.
template <class T>
inline T* CUndoStack<T>::poPop(T* _poState)
{
	if (! bCanPop()) return(NULL);
	
	T* poState = m_oStack[m_iHeaderPos];
	m_oStack[m_iHeaderPos] = _poState;
	m_iHeaderPos--;

	return(poState);
}
//---------------------------------------------------------------------------
/// Moves the current header position one step up.
template <class T>
inline T* CUndoStack<T>::poUnpop(T* _poState)
{
	if (! bCanUnpop()) return(NULL);

	m_iHeaderPos++;
	T* poState = m_oStack[m_iHeaderPos];
	m_oStack[m_iHeaderPos] = _poState;

	return(poState);
}
//---------------------------------------------------------------------------
template <class T>
inline void CUndoStack<T>::TrimTo(unsigned int _uiNumStates)
{
	unsigned int uiInitialStates = m_oStack.size();
	
	int iTrimSize = m_oStack.size() - _uiNumStates;
	if (iTrimSize > 0)
	{
		while (m_oStack.size() > _uiNumStates)
		{
			// poped states from the stack. Trim it.
			delete m_oStack[0];
			m_oStack.erase(m_oStack.begin());
		}	
		
		if (m_iHeaderPos < iTrimSize)
			m_iHeaderPos = -1;
		else
			m_iHeaderPos -= iTrimSize;
	}
}
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
