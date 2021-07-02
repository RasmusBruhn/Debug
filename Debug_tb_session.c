#include <stdio.h>

#define DBG_ACTIVE
#include "Debug.h"

void PrintFunctions(void);
void PrintSessions(void);

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

    uint64_t ErrorID = DBG_Init(ProfileLog, RunningLog, ErrorLog, 0);



    // Print starting function list
    printf("Right after initialisation:\n");
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // Start a session
    printf("Starting first session: %lX\n", DBG_StartSession("FirstFunction"));
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // Start another session
    printf("Starting second session: %lX\n", DBG_StartSession("SecondFunction"));
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // End session
    printf("Ending second session: %lX\n", DBG_EndSession());
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // Start another session
    printf("Starting first session again: %lX\n", DBG_StartSession("FirstFunction"));
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // Start another session
    printf("Starting second session: %lX\n", DBG_StartSession("SecondFunction"));
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // End session
    printf("Ending second session: %lX\n", DBG_EndSession());
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // End session
    printf("Ending first session: %lX\n", DBG_EndSession());
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // End session
    printf("Ending first session again: %lX\n", DBG_EndSession());
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // Start another session
    printf("Starting first session: %lX\n", DBG_StartSession("FirstFunction"));
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // End session
    printf("Ending first session: %lX\n", DBG_EndSession());
    PrintFunctions();
    PrintSessions();
    printf("\n");



    // Close down
    DBG_Quit();

    fclose(ErrorLog);
    fclose(ProfileLog);
    fclose(RunningLog);

    printf("Done\n");

    return 0;
}

void PrintFunctions(void)
{
    extern _DBG_FunctionData **_DBG_Functions;
    extern uint32_t _DBG_FunctionCount;

    printf("Function count: %u\n", _DBG_FunctionCount);
    printf("Function list:\n");

    for (uint32_t Var = 0; Var < _DBG_FunctionCount; ++Var)
        printf("- %s\n", _DBG_PrintFunctionData(_DBG_Functions[Var]));
}

void PrintSessions(void)
{
    extern _DBG_Session *_DBG_FirstSession;
    extern _DBG_Session *_DBG_CurrentSession;

    printf("CurrentSession: %lX\n", (uint64_t)_DBG_CurrentSession);
    printf("FirstSession:   %lX\n", (uint64_t)_DBG_FirstSession);

    printf("Session count: %u\n", ((_DBG_CurrentSession == NULL) ? (0) : (_DBG_CurrentSession->depth + 1)));

    _DBG_Session *Session = _DBG_FirstSession;

    while (Session != NULL)
    {
        printf("- %s\n", _DBG_PrintSession(Session));
        Session = Session->child;
    }
}