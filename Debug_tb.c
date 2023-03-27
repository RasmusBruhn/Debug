#include <stdio.h>
#include <stdint.h>
#include "Debug2.h"

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
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

    // Check overflows
    uint32_t *MemoryInt1 = malloc(sizeof(uint32_t) * 10);
    uint32_t *MemoryInt2 = malloc(sizeof(uint32_t) * 10);
    uint32_t *MemoryInt3 = malloc(sizeof(uint32_t) * 10);

    MemoryInt1[-1] = 0x11121314;
    MemoryInt2[10] = 0x21222324;
    MemoryInt3[-1] = 0x31323334;
    MemoryInt3[10] = 0x41424344;

    free(MemoryInt1);
    free(MemoryInt2);
    free(MemoryInt3);

    printf("\nUnder/overflow:\n");
    DBG_MemoryPrint();

    printf("\nDone\n");

    return 0;
}