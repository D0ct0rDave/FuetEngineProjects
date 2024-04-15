/* A simple program to show how to initialize and use basic functionality
 * of the FuetEngine.
 */

#include <stdlib.h>
#include <stdio.h>
#include "FuetEngine.h"
#include "../../src/sprites.h"

int main(int argc, char **argv)
{
    InitEngine(NULL);

    while(1)
        UpdateFrame();

    return 0;
}
