#include <stdio.h>

#define DBG_ACTIVE
#include "Debug.h"

void PrintMemory(void);

// Test init and quit functions
int main(int argc, char **argv)
{
    // Setup error log and initialise
    FILE *ErrorLog = fopen("DebugErrorLog.txt", "w+");

    if (ErrorLog == NULL)
        printf("Error creating log: %s\n", strerror(errno));

    FILE *ProfileLog = fopen("DebugProfileLog.txt", "w");

    if (ProfileLog == NULL)
        printf("Error creating profile log: %s\n\n", strerror(errno));
    
    FILE *RunningLog = fopen("DebugRunningLog.txt", "w");

    if (RunningLog == NULL)
        printf("Error creating running log: %s\n\n", strerror(errno));
    
    // DBG_FLAGS_TIME | DBG_FLAGS_OWNTIME | DBG_FLAGS_TOTAL | DBG_FLAGS_AVG | DBG_FLAGS_STD | DBG_FLAGS_MIN | DBG_FLAGS_MAX | DBG_FLAGS_LIST;
    uint64_t Flags = DBG_FLAGS_TIME | DBG_FLAGS_OWNTIME | DBG_FLAGS_TOTAL | DBG_FLAGS_AVG | DBG_FLAGS_STD | DBG_FLAGS_MIN | DBG_FLAGS_MAX | DBG_SortFlag(DBG_FLAGS_AVG) | DBG_SortFlag(DBG_FLAGS_INVERT);

    uint64_t ErrorID = DBG_Init(ProfileLog, RunningLog, ErrorLog, Flags, 0);

    // Start function1
    printf("Starting function 1\n");
    DBG_StartSession("Function1");
    PrintMemory();

    // Allocate memory1
    printf("Allocated memory 1\n");
    void *Memory1 = DBG_Malloc(100, "Memory1");
    PrintMemory();

    // Allocate memory1
    printf("Allocated memory 2\n");
    void *Memory2 = DBG_Malloc(99, "Memory2");
    PrintMemory();

    // Add memory to start
    printf("Allocate memory to start\n");
    void *Memory3 = DBG_Malloc(98, "Memory3");
    DBG_Free(Memory1);
    void *MemoryStart = DBG_Malloc(1, "MemoryStart");
    PrintMemory();

    // Reallocate memory
    printf("Reallocated memory 2\n");
    Memory2 = DBG_Realloc(Memory2, 300, NULL);
    PrintMemory();

    // Reallocate not used memory
    printf("Try reallocating non-allocated memory\n\n");
    DBG_Realloc((void *)0x1, 10, NULL);

    // Free not used memory
    printf("Try freeing non-allocated memory\n\n");
    DBG_Free((void *)0x1);

    // don't Free memory 2
    //printf("Free memory 2\n");
    //DBG_Free(Memory2);
    //PrintMemory();

    // Free memory 3
    printf("Free memory 3\n");
    DBG_Free(Memory3);
    PrintMemory();

    //  don't Free memory start
    //printf("Free memory start\n");
    //DBG_Free(MemoryStart);
    //PrintMemory();

    // End function1
    DBG_EndSession();

    // Close down
    DBG_Quit();

    fclose(ErrorLog);
    fclose(ProfileLog);
    fclose(RunningLog);

    printf("Done\n");

    return 0;
}

void PrintMemory(void)
{
    extern _DBG_Memory **_DBG_MemoryList;
    extern uint32_t _DBG_MemoryCount;

    printf("Memory count: %u\n", _DBG_MemoryCount);
    printf("Memory list:\n");

    for (uint32_t Var = 0; Var < _DBG_MemoryCount; ++Var)
        printf("- %s\n", _DBG_PrintMemory(_DBG_MemoryList[Var]));

    printf("\n");
}
