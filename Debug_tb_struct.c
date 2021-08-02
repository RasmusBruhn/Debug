#include <stdio.h>

#define DBG_ACTIVE
#include "Debug.h"

// Test the struct functions
int main(int argc, char **argv)
{
    // Setup error log
    FILE *ErrorLog = fopen("DebugErrorLog.txt", "w+");

    if (ErrorLog == NULL)
        printf("Error creating log: %s\n", strerror(errno));
    
    FILE *ProfileLog = fopen("DebugProfileLog.txt", "w");

    if (ProfileLog == NULL)
        printf("Error creating profile log: %s\n\n", strerror(errno));

    uint64_t ErrorID = DBG_Init(ProfileLog, NULL, ErrorLog, 0, 0);

    // Test session creation
    _DBG_Session *TestSession = _DBG_CreateSession(25, (_DBG_Session *)0xFFFFFFFF, 3);

    // Print pointer
    printf("Session pointer: %p\n", TestSession);

    // Print it to make sure it was created correctly, cannot test too long message because it is impossible
    printf("Session: %s\n\n", _DBG_PrintSession(TestSession));

    // Destroy session
    _DBG_DestroySession(TestSession);

    // Test function data creation
    _DBG_FunctionData *TestFunctionData = _DBG_CreateFunctionData(13, "NameOfFunction");

    // Print pointer
    printf("Function data pointer: %p\n", TestFunctionData);

    // Print data
    printf("Function data: %s\n\n", _DBG_PrintFunctionData(TestFunctionData));

    // Give a time list
    TestFunctionData->count = 5;
    TestFunctionData->time = (uint64_t *)malloc(sizeof(uint64_t) * 5);
    TestFunctionData->ownTime = (uint64_t *)malloc(sizeof(uint64_t) * 5);

    TestFunctionData->time[0] = 0;
    TestFunctionData->time[1] = 1;
    TestFunctionData->time[2] = 2;
    TestFunctionData->time[3] = 3;
    TestFunctionData->time[4] = 4;

    TestFunctionData->ownTime[0] = 0;
    TestFunctionData->ownTime[1] = 10;
    TestFunctionData->ownTime[2] = 20;
    TestFunctionData->ownTime[3] = 30;
    TestFunctionData->ownTime[4] = 40;

    printf("Function data with time lists: %s\n\n", _DBG_PrintFunctionData(TestFunctionData));

    // Test stats creation
    extern _DBG_FunctionData **_DBG_Functions;
    _DBG_Functions = (_DBG_FunctionData **)realloc(_DBG_Functions, sizeof(_DBG_FunctionData *) * 2);
    _DBG_Functions[1] = TestFunctionData;

    _DBG_Stats *TestStats = _DBG_CreateStats(1);

    // Print pointer
    printf("Session pointer: %p\n", TestStats);

    // Print it to make sure it was created correctly, cannot test too long message because it is impossible
    printf("Stats: %s\n\n", _DBG_PrintStats(TestStats));


    // Test memory struct
    _DBG_Session *Session1 = _DBG_CreateSession(0, NULL, 1);
    _DBG_Session *Session2 = _DBG_CreateSession(1, Session1, 2);
    _DBG_Session *Session3 = _DBG_CreateSession(2, Session2, 3);
    _DBG_Session *Session4 = _DBG_CreateSession(3, Session3, 4);
    _DBG_Session *Session5 = _DBG_CreateSession(4, Session4, 5);
    _DBG_Session *Session6 = _DBG_CreateSession(5, Session5, 6);

    _DBG_CurrentSession = Session3;

    size_t Size = 10;
    char Memory[Size];
    _DBG_Memory *TestMemory = _DBG_CreateMemory("Memory", Memory, Size);

    _DBG_CurrentSession = Session6;
    _DBG_Memory *TestMemory2 = _DBG_CreateMemory("Memory", Memory, Size);

    printf("Size: %lu, Pointer: %p\n", Size, Memory);
    printf("Memory: %s\n", _DBG_PrintMemory(TestMemory));
    printf("Long Memory: %s\n\n", _DBG_PrintMemory(TestMemory2));

    // Clean up
    _DBG_DestroyMemory(TestMemory);
    _DBG_DestroyMemory(TestMemory2);
    _DBG_CurrentSession = NULL;
    _DBG_DestroySession(Session1);
    _DBG_DestroySession(Session2);
    _DBG_DestroySession(Session3);
    _DBG_DestroySession(Session4);
    _DBG_DestroySession(Session5);
    _DBG_DestroySession(Session6);

    // Too long time list
    TestFunctionData->count = 6;
    TestFunctionData->time = (uint64_t *)realloc(TestFunctionData->time, sizeof(uint64_t) * 6);
    TestFunctionData->ownTime = (uint64_t *)realloc(TestFunctionData->ownTime, sizeof(uint64_t) * 6);

    printf("Function data with long time lists: %s\n\n", _DBG_PrintFunctionData(TestFunctionData));

    _DBG_DestroyFunctionData(TestFunctionData);

    // Message is too long
    char Name[DBG_PRINTSTRUCT_MAXLENGTH];

    for (uint32_t Var = 0; Var < DBG_PRINTSTRUCT_MAXLENGTH - 1; ++Var)
        Name[Var] = 'a';

    Name[DBG_PRINTSTRUCT_MAXLENGTH - 1] = '\0';

    _DBG_FunctionData *TooLongName = _DBG_CreateFunctionData(0, Name);

    printf("Too long name: %s\n\n", _DBG_PrintFunctionData(TooLongName));

    _DBG_DestroyFunctionData(TooLongName);

    DBG_Quit();

    fclose(ErrorLog);
    fclose(ProfileLog);

    return 0;
}