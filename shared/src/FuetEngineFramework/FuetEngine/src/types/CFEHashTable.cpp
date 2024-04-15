#include "types/CFEHashTable.h"

#ifdef _DEBUG
static void Test()
{
    int i = 10;

    CFEHashTable<int> oHash;

    oHash.Init(16,256);
    oHash.AddVar("lolo",&i);

    int *pi = oHash.poGetVar("lolo");
}
#endif
