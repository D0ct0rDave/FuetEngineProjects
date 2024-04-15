// ----------------------------------------------------------------------------
/*! \class CFESingleton
 *  \brief Singleton template pattern to help on resource creation control.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par GammaE License
 */
// ----------------------------------------------------------------------------
#ifndef CFESingletonH
#define CFESingletonH
// ----------------------------------------------------------------------------
#ifndef NULL
#define NULL 0L
#endif
// ----------------------------------------------------------------------------
#define DECLARE_SINGLETON(CLASSNAME) \
    class CLASSNAME : public CFESingleton < CLASSNAME >

#define DECLARE_INHERITANT_SINGLETON(CLASSNAME,BASECLASSVISIBILITY,BASECLASS) \
    class CLASSNAME : public CFESingleton < CLASSNAME >,BASECLASSVISIBILITY BASECLASS
// ----------------------------------------------------------------------------
#if defined(__arm__)
// suppress message about "friend T is non-standard"
#pragma diag_suppress 451
#endif
// ----------------------------------------------------------------------------
template <class T>
class CFESingleton
{
	friend T;

    protected:

        CFESingleton(){};
        ~CFESingleton(){};

    public:

        static T* I()
        {
            if (m_spoInstance == NULL)
                m_spoInstance = new T;

            return(m_spoInstance);
        }

        static void Finish()
        {
            if (m_spoInstance != NULL)
            {
                delete m_spoInstance;
                m_spoInstance = NULL;
            }
        }

    protected:

        static T* m_spoInstance;
};

//
template <typename T>
T* CFESingleton<T>::m_spoInstance;

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
