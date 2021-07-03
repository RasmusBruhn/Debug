#include <stdio.h>

#define DBG_ACTIVE
#include "Debug.h"

// Test the struct functions
int main(int argc, char **argv)
{
    // Setup error log
    FILE *ErrorLog = fopen("c/Debug/DebugErrorLog.txt", "w+");

    if (ErrorLog == NULL)
        printf("Error creating log: %s\n", strerror(errno));
    
    FILE *ProfileLog = fopen("c/Debug/DebugProfileLog.txt", "w");

    if (ProfileLog == NULL)
        printf("Error creating profile log: %s\n\n", strerror(errno));

    uint64_t ErrorID = DBG_Init(ProfileLog, NULL, ErrorLog, 0, 0);

    // Test session creation
    _DBG_Session *TestSession = _DBG_CreateSession(25, (_DBG_Session *)0xFFFFFFFF, 3);

    // Print pointer
    printf("Session pointer: %lX\n", (uint64_t)TestSession);

    // Print it to make sure it was created correctly, cannot test too long message because it is impossible
    printf("Session: %s\n\n", _DBG_PrintSession(TestSession));

    // Destroy session
    _DBG_DestroySession(TestSession);

    // Test function data creation
    _DBG_FunctionData *TestFunctionData = _DBG_CreateFunctionData(13, "NameOfFunction");

    // Print pointer
    printf("Function data pointer: %lX\n", (uint64_t)TestFunctionData);

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