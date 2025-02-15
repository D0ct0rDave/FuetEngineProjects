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
#include "CFESingleton.h"

/*
 #define DECLARE_WAREHOUSE( CLASSNAME, WAREHOUSETYPE, RESOURCETYPE )\
   class CLASSNAME : public CGSingleton<CLASSNAME>, public WAREHOUSETYPE<RESOURCETYPE>


 #define DECLARE_WAREHOUSE( CLASSNAME, WAREHOUSETYPE, RESOURCETYPE )\
   class CLASSNAME : public CGSingleton<CLASSNAME>, public WAREHOUSETYPE<RESOURCETYPE>


   template <typename Type> class Singleton2
   {
   public:
    static Type* GetSingleton()
    {
        static Type* s_instance = new Type;
        return s_instance;
    }
   };

   template <typename Type, typename Base> class ResourceManager : public Singleton2<Base>
   {

   public:
    int GetT() {return sizeof (Type); }
   };

   template <typename Type, typename Base> class ExtResourceManager : public ResourceManager<Type, Base>
   {
   public:
    int Get2T() {return 2*sizeof (Type); }
   };


   class MyNewClass  : public ExtResourceManager<bool, MyNewClass>
   {

   };

   class MyNewClass2  : public ResourceManager<bool, MyNewClass2>
   {

   };

 #define DECLARE_WAREHOUSE( CLASSNAME, WAREHOUSETYPE, RESOURCETYPE )\
   class CLASSNAME : public WAREHOUSETYPE<RESOURCETYPE,CLASSNAME>

   //-----------------------------------------------------------------------------
 #define DECLARE_SINGLETON(CLASSNAME)\
   class CLASSNAME : public CGSingleton<CLASSNAME>

 #define DECLARE_INHERITANT_SINGLETON(CLASSNAME,BASECLASSVISIBILITY,BASECLASS)\
   class T##CLASSNAME : BASECLASSVISIBILITY BASECLASS{};\
   class CLASSNAME : CSingleton<T##CLASSNAME>{};

   class CLASSNAME : public CGSingleton<CLASSNAME>,BASECLASSVISIBILITY BASECLASS
 */
