// ----------------------------------------------------------------------------
/*! \class
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par GammaE License
 */
// ----------------------------------------------------------------------------
#ifndef CFESingletonH
#define CFESingletonH
// ----------------------------------------------------------------------------
#define DECLARE_SINGLETON(CLASSNAME) \
    class CLASSNAME : public CFESingleton < CLASSNAME >

#define DECLARE_INHERITANT_SINGLETON(CLASSNAME,BASECLASSVISIBILITY,BASECLASS) \
    class CLASSNAME : public CFESingleton < CLASSNAME >,BASECLASSVISIBILITY BASECLASS
// ----------------------------------------------------------------------------
template <class T>
class CFESingleton
{
    friend CFESingleton<T>;

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
template <typename T> T* CFESingleton<T>::m_spoInstance;

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
