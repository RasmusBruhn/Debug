#include <stdio.h>

#define DBG_ACTIVE
#include "Debug.h"

// Test init and quit functions
int main(int argc, char **argv)
{
    // Setup error log
    FILE *ErrorLog = fopen("c/Debug/DebugErrorLog.txt", "w+");

    if (ErrorLog == NULL)
        printf("Error creating log: %s\n\n", strerror(errno));

    FILE *ProfileLog = fopen("c/Debug/DebugProfileLog.txt", "w");

    if (ProfileLog == NULL)
        printf("Error creating profile log: %s\n\n", strerror(errno));



    // Test standard values before initialisation
    printf("Before initialisation\n");
    
    // First session
    extern _DBG_Session *_DBG_FirstSession;
    printf("First session: %lX\n", (uint64_t)_DBG_FirstSession);

    // Current session
    extern _DBG_Session *_DBG_CurrentSession;
    printf("Current session: %lX\n", (uint64_t)_DBG_CurrentSession);

    // Main function
    extern _DBG_FunctionData _DBG_FunctionMain;
    printf("Main function: %s\n", _DBG_PrintFunctionData(&_DBG_FunctionMain));

    // Function list
    printf("Function count: %u\n", _DBG_FunctionCount);
    printf("Function list:\n");

    for (uint32_t Var = 0; Var < _DBG_FunctionCount; ++Var)
        printf("- %s\n", _DBG_PrintFunctionData(_DBG_Functions[Var]));

    // Flags
    extern uint64_t _DBG_UsedFlags;
    printf("Flags: %lX\n", _DBG_UsedFlags);

    // Logs
    extern FILE *_DBG_ProfileLog;
    extern FILE *_DBG_RunningLog;
    printf("ProfileLog: %lX\n", (uint64_t)_DBG_ProfileLog);
    printf("RunningLog: %lX\n\n", (uint64_t)_DBG_RunningLog);



    // Make sure error is given in quit when not initialised
    DBG_Quit();



    // Print values after init
    uint64_t ErrorID = DBG_Init(ProfileLog, (FILE *)5, ErrorLog, 0xF);

    if (ErrorID != DBG_ERRORID_NOERROR)
        printf("Error initialising: %s\n\n", DBG_GetError());

    printf("After initialisation\n");

    // First session
    printf("First session: %lX\n", (uint64_t)_DBG_FirstSession);

    // First session
    printf("Current session: %lX\n", (uint64_t)_DBG_CurrentSession);

    // Function list
    printf("Function count: %u\n", _DBG_FunctionCount);
    printf("Function list:\n");

    for (uint32_t Var = 0; Var < _DBG_FunctionCount; ++Var)
        printf("- %s\n", _DBG_PrintFunctionData(_DBG_Functions[Var]));

    // Flags
    printf("Flags: %lX\n", _DBG_UsedFlags);

    // Log
    printf("ProfileLog: %lX\n", (uint64_t)_DBG_ProfileLog);
    printf("RunningLog: %lX\n\n", (uint64_t)_DBG_RunningLog);



    // make sure error is given in init when already initialised
    if (DBG_Init(ProfileLog, NULL, NULL, 0xFF) != DBG_ERRORID_NOERROR)
        printf("Error initialising twice: %s\n", DBG_GetError());

    // Flags
    printf("Flags: %lX\n", _DBG_UsedFlags);

    // Log
    printf("ProfileLog: %lX\n", (uint64_t)_DBG_ProfileLog);
    printf("RunningLog: %lX\n\n", (uint64_t)_DBG_RunningLog);



    // Set the session
    _DBG_CurrentSession = _DBG_CreateSession(0, NULL, 0);
    _DBG_FirstSession = _DBG_CurrentSession;



    // Make sure error is given in quit when function data is NULL and when sessions are open
    extern _DBG_FunctionData **_DBG_Functions;
    extern uint32_t _DBG_FunctionCount;

    _DBG_Functions = (_DBG_FunctionData **)realloc(_DBG_Functions, sizeof(_DBG_FunctionData *) * 2);
    _DBG_Functions[1] = NULL;
    _DBG_FunctionCount = 2;

    extern _DBG_Session *_DBG_CurrentSession;
    extern _DBG_Session *_DBG_FirstSession;

    _DBG_CurrentSession = _DBG_CreateSession(0, NULL, 0);
    _DBG_FirstSession = _DBG_CurrentSession;

    DBG_Quit();



    // Print values after quit
    printf("After quitting\n");

    // First session
    printf("First session: %lX\n", (uint64_t)_DBG_FirstSession);

    // First session
    printf("Current session: %lX\n", (uint64_t)_DBG_CurrentSession);

    // Function list
    printf("Function count: %u\n", _DBG_FunctionCount);
    printf("Function list:\n");

    for (uint32_t Var = 0; Var < _DBG_FunctionCount; ++Var)
        printf("- %s\n", _DBG_PrintFunctionData(_DBG_Functions[Var]));

    // Flags
    printf("Flags: %lX\n", _DBG_UsedFlags);

    // Log
    printf("ProfileLog: %lX\n", (uint64_t)_DBG_ProfileLog);
    printf("RunningLog: %lX\n\n", (uint64_t)_DBG_RunningLog);



    printf("Error archive:\n");
    char *ErrorString;
    while ((ErrorString = DBG_GetArchivedError()) != NULL)
        printf("Error: %s\n", ErrorString);

    fclose(ErrorLog);
    fclose(ProfileLog);

    return 0;
}