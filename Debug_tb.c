#include <stdio.h>
#include "Debug2.h"

int main(int argc, char **argv)
{
    // Allocate memory and free it
    void *Memory1 = malloc(10);
    void *Memory2 = malloc(20);
    Memory2 = realloc(Memory2, 30);
    void *Memory3 = realloc(NULL, 30);
    free(Memory1);
    free(Memory2);
    realloc(Memory3, 0);

    printf("Allocating and freeing memory:\n");
    DBG_MemoryPrint();

    // Test single memory leak
    printf("\nSingle memory leak:\n");
    Memory1 = malloc(10);

    DBG_MemoryPrint();
    printf("\nPrint twice:\n");
    DBG_MemoryPrint();

    free(Memory1);

    // Allocate, reallocate and remove some
    Memory1 = malloc(10);
    Memory2 = malloc(20);
    Memory1 = realloc(Memory1, 30);
    Memory3 = malloc(40);
    free(Memory3);
    free(Memory2);

    printf("\nAllocate, reallocate and remove some:\n");
    DBG_MemoryPrint();

    free(Memory1);

    printf("\nDone\n");

    return 0;
}