#include <stdio.h>

//#define DBG_ACTIVE
#include "Debug.h"

#define DBG_ACTIVE
#include "Debug.h"

#define __STR(Macro) #Macro
#define STR(Macro) __STR(Macro)

// Test the debug functions
int main(int argc, char **argv)
{
    // Make sure it has defined things correctly
    printf("Checking Init: '%s'\n", STR(DBG_Init(NULL, 0)));
    printf("Checking Quit: '%s'\n", STR(DBG_Quit()));
    printf("Checking GetError: '%s'\n", STR(DBG_GetError()));

    return 0;
}