//#define DBG_ACTIVE
// If it is active
#ifdef DBG_ACTIVE
#ifndef DEBUG_ACTIVE_H_INCLUDED
#define DEBUG_ACTIVE_H_INCLUDED

#ifndef DEBUG_INACTIVE_H_INCLUDED
#define DEBUG_INACTIVE_H_INCLUDED
#else

#ifdef DBG_GetError
#undef DBG_GetError
#endif

#ifdef DBG_Init
#undef DBG_Init
#endif

#ifdef DBG_Quit
#undef DBG_Quit
#endif

#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <string.h>

void DBG_ExitFunc(uint64_t ErrorID);

#ifdef DBG_EXITFUNC
#define ERR_EXITFUNC DBG_EXITFUNC
#else
#define ERR_EXITFUNC &DBG_ExitFunc
#endif

#define ERR_PREFIX DBG
#define ERR_MAXLENGTH 1000
#define ERR_MAXARCHIVED 100
#include <Error.h>

// Structs
typedef struct __DBG_Session _DBG_Session;
typedef struct __DBG_FunctionData _DBG_FunctionData;

// Data for a session, a session starts when a function is executed and ends when the function is done
struct __DBG_Session
{
    uint32_t ID;            // The id of the session, maps to a function name in the DBG_FunctionList list.
    uint64_t startTime;     // What time it started the session
    uint64_t subTime;       // How much time has been used in subfunctions
    uint64_t removeTime;    // How much time has been used by DBG functions inside this session
    uint64_t removeSubTime; // How much time has been used by DBG functions inside children sessions of this session
    _DBG_Session *child;    // What session is running inside this session, NULL if it doesn't have another session running
    _DBG_Session *parent;   // What session is this session running in, NULL if it doesn't run in another session
    uint32_t depth;         // How many sessions are above this one, 0 when there are none above it
};

// All the important data for the functions used
struct __DBG_FunctionData
{
    uint32_t ID;            // The id of the session, this is where in the list it is
    char *name;             // The name of the function
    uint32_t count;         // The number of times this function has been executed
    uint64_t *time;         // List of time spent in each of the sessions without time in sub sessions
    uint64_t *subTime;      // List of time spent in each of the sessions with time in sub sessions
};

// Contants
// Errors
enum DBG_ErrorID
{
    DBG_ERRORID_NOERROR = 0x00000000,
    DBG_ERRORID_INIT_MALLOC = 0x100010300,
    DBG_ERRORID_INIT_INIT = 0x100010201,
    DBG_ERRORID_QUIT_INIT = 0x100020200,
    DBG_ERRORID_QUIT_NULL = 0x100020101,
    DBG_ERRORID_QUIT_SESSION = 0x100020102,
    DBG_ERRORID_CREATESESSION_MALLOC = 0x100030200,
    DBG_ERRORID_CREATEFUNCTIONDATA_MALLOC = 0x100040200,
    DBG_ERRORID_PRINTSESSION_LONG = 0x100050100,
    DBG_ERRORID_PRINTFUNCTIONDATA_LONG1 = 0x100060100,
    DBG_ERRORID_PRINTFUNCTIONDATA_LONG2 = 0x100060101,
    DBG_ERRORID_PRINTFUNCTIONDATA_LONG3 = 0x100060102,
    DBG_ERRORID_STARTSESSION_NAME = 0x100070200,
    DBG_ERRORID_STARTSESSION_CREATEFUNCTION = 0x100070201,
    DBG_ERRORID_STARTSESSION_REALLOC = 0x100070202
};

#define _DBG_ERRORMES_MALLOC "Unable to allocate memory"
#define _DBG_ERRORMES_REALLOC "Unable to reallocate memory"
#define _DBG_ERRORMES_NOINIT "Debugging has not been initialised"
#define _DBG_ERRORMES_FOUNDNULL "Found a stray NULL pointer in %s"
#define _DBG_ERRORMES_ALREADYINIT "Debugging has already been initialised"
#define _DBG_ERRORMES_LONGPRINT "The printed message was too long and was truncated, length was %u with max length of %u"
#define _DBG_ERRORMES_OPENSESSIONS "There are still open sessions"
#define _DBG_ERRORMES_ARGNULL "Argument \"%s\" was NULL"
#define _DBG_ERRORMES_CREATESTRUCT "Unable to create \"%s\" struct"

// Flags
enum DBG_Flags
{
    DBG_FLAG_NOFLAG = 0
};

// struct print max length
#define DBG_PRINTSTRUCT_MAXLENGTH 1000
#define DBG_PRINTSTRUCT_LISTMAXLENGTH 5

// global variables
// The outer most session, NULL if no session has been started
_DBG_Session *_DBG_FirstSession = NULL;
_DBG_Session *_DBG_CurrentSession = NULL;

// List of all the functions
_DBG_FunctionData _DBG_FunctionMain = {.ID = 0, .name = "main", .count = 0, .time = NULL, .subTime = NULL};
_DBG_FunctionData **_DBG_Functions = NULL;
uint32_t _DBG_FunctionCount = 0;

// Flags for what to save
uint64_t _DBG_UsedFlags = 0;

// String to print structs in
char _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH] = "No struct printed yet";

// Function declarations
// Allocates memory for and initialises a Session struct
// Returns a pointer to the struct, NULL on error
// ID: The ID of the session, what function it corresponds to in the function list
// Parent: What session was this created inside, NULL if no session was above this
// Depth: How many sessions are above this, 0 if none is above it
_DBG_Session *_DBG_CreateSession(uint32_t ID, _DBG_Session *Parent, uint32_t Depth);

// Allocates memory for and initialises a FunctionData struct
// Returns a pointer to the , NULL on error
// ID: The ID of the function, where is it located in the function list
// Name: What is the name of the function
_DBG_FunctionData *_DBG_CreateFunctionData(uint32_t ID, char *Name);

// Frees a Session struct and all memory allocated for it
// Session: The struct to free
void _DBG_DestroySession(_DBG_Session *Session);

// Frees a FunctionData struct and all memory allocated for it
// FunctionData: The struct to free
void _DBG_DestroyFunctionData(_DBG_FunctionData *FunctionData);

// Turns a Session struct into a string
// Returns a pointer to the string
// Session: The struct to turn into a string
char *_DBG_PrintSession(const _DBG_Session *Session);

// Turns a FunctionData struct into a string
// Returns a pointer to the string
// FunctionData: The struct to turn into a string
char *_DBG_PrintFunctionData(const _DBG_FunctionData *FunctionData);

// Initialises debugging
// Returns 0 on success and an error code on failure
// ErrorLog: A text file pointer to where it can write the error messages, NULL if it should not write any messages
// Flags: Contains all the information for what data to save, options are from DBG_Flags
uint64_t DBG_Init(FILE *ErrorLog, uint64_t Flags);

// Closes down everything and frees allocated memory
void DBG_Quit(void);

uint64_t DBG_StartSession(char *Name);

uint64_t DBG_EndSession(void);

// Functions
void DBG_ExitFunc(uint64_t ErrorID)
{
    // Exit
    exit((uint32_t)ErrorID);
}

_DBG_Session *_DBG_CreateSession(uint32_t ID, _DBG_Session *Parent, uint32_t Depth)
{
    // Allocate memory
    _DBG_Session *Session = (_DBG_Session *)malloc(sizeof(_DBG_Session));

    if (Session == NULL)
    {
        _DBG_AddErrorForeign(DBG_ERRORID_CREATESESSION_MALLOC, strerror(errno), _DBG_ERRORMES_MALLOC);
        return NULL;
    }
 
    // Set default values
    Session->startTime = 0;
    Session->subTime = 0;
    Session->removeTime = 0;
    Session->removeSubTime = 0;
    Session->child = NULL;

    // Set input values
    Session->ID = ID;
    Session->parent = Parent;
    Session->depth = Depth;

    return Session;
}

_DBG_FunctionData *_DBG_CreateFunctionData(uint32_t ID, char *Name)
{
    // Allocate memory
    _DBG_FunctionData *FunctionData = (_DBG_FunctionData *)malloc(sizeof(_DBG_FunctionData));

    if (FunctionData == NULL)
    {
        _DBG_AddErrorForeign(DBG_ERRORID_CREATEFUNCTIONDATA_MALLOC, strerror(errno), _DBG_ERRORMES_MALLOC);
        return NULL;
    }

    // Set default values
    FunctionData->time = 0;
    FunctionData->subTime = 0;
    FunctionData->count = 0;

    FunctionData->ID = ID;
    FunctionData->name = Name;

    return FunctionData;
}

void _DBG_DestroySession(_DBG_Session *Session)
{
    free(Session);
}

void _DBG_DestroyFunctionData(_DBG_FunctionData *FunctionData)
{
    // Free arrays inside structure
    if (FunctionData->time != NULL)
        free(FunctionData->time);

    if (FunctionData->subTime != NULL)
        free(FunctionData->subTime);

    // Free the struct
    free(FunctionData);
}

char *_DBG_PrintSession(const _DBG_Session *Session)
{
    // Print everything to a string
    extern char _DBG_PrintStructString[];

    int32_t Length = snprintf(_DBG_PrintStructString, DBG_PRINTSTRUCT_MAXLENGTH, "{ID = %u, startTime = %lu, subTime = %lu, removeTime = %lu, removeSubTime = %lu, child = %lX, parent = %lX, depth = %u}",
                                                                                   Session->ID, Session->startTime, Session->subTime, Session->removeTime, Session->removeSubTime, (uint64_t)Session->child, (uint64_t)Session->parent, Session->depth);

    // Show if it was too long
    if (Length >= DBG_PRINTSTRUCT_MAXLENGTH)
    {
        _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH - 2] = '.';
        _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH - 3] = '.';
        _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH - 4] = '.';

        _DBG_SetError(DBG_ERRORID_PRINTSESSION_LONG, _DBG_ERRORMES_LONGPRINT, Length, DBG_PRINTSTRUCT_MAXLENGTH - 1);
    }

    return _DBG_PrintStructString;
}

char *_DBG_PrintFunctionData(const _DBG_FunctionData *FunctionData)
{
    extern char _DBG_PrintStructString[];

    // Convert the list of times to strings
    char TimeString[DBG_PRINTSTRUCT_MAXLENGTH];
    char SubTimeString[DBG_PRINTSTRUCT_MAXLENGTH];

    int32_t Length = snprintf(TimeString, DBG_PRINTSTRUCT_MAXLENGTH, "[");
    char *String = TimeString + Length;
    int32_t MaxLength = DBG_PRINTSTRUCT_MAXLENGTH - Length;

    for (uint64_t *TimeList = FunctionData->time, *EndTimeList = FunctionData->time + FunctionData->count; TimeList < EndTimeList; ++TimeList)
    {
        // Check if there are too many elements
        if (TimeList >= FunctionData->time + DBG_PRINTSTRUCT_LISTMAXLENGTH)
        {
            *String = '.';
            *(String + 1) = '.';
            *(String + 2) = '.';

            String += 5;

            break;
        }

        // Print the element
        Length = snprintf(String, MaxLength, "%lu, ", *TimeList);
        String += Length;
        MaxLength -= Length;

        if (MaxLength <= 4)
        {
            TimeString[DBG_PRINTSTRUCT_MAXLENGTH - 3] = '.';
            TimeString[DBG_PRINTSTRUCT_MAXLENGTH - 4] = '.';
            TimeString[DBG_PRINTSTRUCT_MAXLENGTH - 5] = '.';

            String = TimeString + DBG_PRINTSTRUCT_MAXLENGTH;

            _DBG_SetError(DBG_ERRORID_PRINTFUNCTIONDATA_LONG1, _DBG_ERRORMES_LONGPRINT, DBG_PRINTSTRUCT_MAXLENGTH - MaxLength, DBG_PRINTSTRUCT_MAXLENGTH - 5);

            break;
        }
    }

    // Add ending bracket
    if (FunctionData->count == 0)
    {
        *String = ']';
        *(String + 1) = '\0';
    }

    else
    {
        *(String - 2) = ']';
        *(String - 1) = '\0';
    }

    Length = snprintf(SubTimeString, DBG_PRINTSTRUCT_MAXLENGTH, "[");
    String = SubTimeString + Length;
    MaxLength = DBG_PRINTSTRUCT_MAXLENGTH - Length;

    for (uint64_t *TimeList = FunctionData->subTime, *EndTimeList = FunctionData->subTime + FunctionData->count; TimeList < EndTimeList; ++TimeList)
    {
        // Check if there are too many elements
        if (TimeList >= FunctionData->subTime + DBG_PRINTSTRUCT_LISTMAXLENGTH)
        {
            *String = '.';
            *(String + 1) = '.';
            *(String + 2) = '.';

            String += 5;

            break;
        }

        // Print the element
        Length = snprintf(String, MaxLength, "%lu, ", *TimeList);
        String += Length;
        MaxLength -= Length;

        if (MaxLength <= 4)
        {
            TimeString[DBG_PRINTSTRUCT_MAXLENGTH - 3] = '.';
            TimeString[DBG_PRINTSTRUCT_MAXLENGTH - 4] = '.';
            TimeString[DBG_PRINTSTRUCT_MAXLENGTH - 5] = '.';

            String = TimeString + DBG_PRINTSTRUCT_MAXLENGTH;

            _DBG_SetError(DBG_ERRORID_PRINTFUNCTIONDATA_LONG2, _DBG_ERRORMES_LONGPRINT, DBG_PRINTSTRUCT_MAXLENGTH - MaxLength, DBG_PRINTSTRUCT_MAXLENGTH - 5);

            break;
        }
    }

    // Add ending bracket
    if (FunctionData->count == 0)
    {
        *String = ']';
        *(String + 1) = '\0';
    }

    else if (MaxLength > 0)
    {
        *(String - 2) = ']';
        *(String - 1) = '\0';
    }

    // Print the struct
    Length = snprintf(_DBG_PrintStructString, DBG_PRINTSTRUCT_MAXLENGTH, "{ID = %u, name = \"%s\", count = %u, time = %s, subTime = %s}",
                                                                           FunctionData->ID, FunctionData->name, FunctionData->count, TimeString, SubTimeString);

    if (Length >= DBG_PRINTSTRUCT_MAXLENGTH)
    {
        _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH - 2] = '.';
        _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH - 3] = '.';
        _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH - 4] = '.';

        _DBG_SetError(DBG_ERRORID_PRINTFUNCTIONDATA_LONG3, _DBG_ERRORMES_LONGPRINT, Length, DBG_PRINTSTRUCT_MAXLENGTH - 1);
    }

    return _DBG_PrintStructString;
}

uint64_t DBG_Init(FILE *ErrorLog, uint64_t Flags)
{
    extern _DBG_FunctionData **_DBG_Functions;
    extern uint32_t _DBG_FunctionCount;
    extern uint64_t _DBG_UsedFlags;

    // Make sure it has not been initialised already
    if (_DBG_Functions != NULL)
    {
        _DBG_SetError(DBG_ERRORID_INIT_INIT, _DBG_ERRORMES_ALREADYINIT);

        return DBG_ERRORID_INIT_INIT;
    }
        
    // Set error log file
    _DBG_SetLogFile(ErrorLog);

    // Allocate memory for the functions
    _DBG_Functions = (_DBG_FunctionData **)malloc(sizeof(_DBG_FunctionData));

    if (_DBG_Functions == NULL)
    {
        _DBG_AddErrorForeign(DBG_ERRORID_INIT_MALLOC, strerror(errno), _DBG_ERRORMES_MALLOC);

        return DBG_ERRORID_INIT_MALLOC;
    }

    _DBG_FunctionCount = 1;

    // Setup the main function
    _DBG_Functions[0] = &_DBG_FunctionMain;

    // Setup flags
    _DBG_UsedFlags = Flags;

    return DBG_ERRORID_NOERROR;
}

void DBG_Quit(void)
{
    extern _DBG_FunctionData **_DBG_Functions;
    extern uint32_t _DBG_FunctionCount;
    extern uint64_t _DBG_UsedFlags;

    // Make sure it has been initialised
    if (_DBG_Functions == NULL)
    {
        _DBG_SetError(DBG_ERRORID_QUIT_INIT, _DBG_ERRORMES_NOINIT);

        return;
    }

    // Free all of the functions
    for (_DBG_FunctionData **DataList = _DBG_Functions + 1, **DataListEnd = _DBG_Functions + _DBG_FunctionCount; DataList < DataListEnd; ++DataList)
    {
        if (*DataList != NULL)
            free(*DataList);
        
        else
            _DBG_SetError(DBG_ERRORID_QUIT_NULL, _DBG_ERRORMES_FOUNDNULL, "_DBG_Functions");
    }

    // Close all open sessions
    extern _DBG_Session *_DBG_CurrentSession;
    extern _DBG_Session *_DBG_FirstSession;

    _DBG_CurrentSession = NULL;

    // Make sure every session is closed
    if (_DBG_FirstSession != NULL)
    {
        _DBG_SetError(DBG_ERRORID_QUIT_SESSION, _DBG_ERRORMES_OPENSESSIONS);

        _DBG_Session *FreeSession = NULL;
        _DBG_Session *NextSession = _DBG_FirstSession;

        while (NextSession != NULL)
        {
            FreeSession = NextSession;
            NextSession = NextSession->child;
            _DBG_DestroySession(FreeSession);
        }
    }

    _DBG_FirstSession = NULL;

    // Free the functions list
    free(_DBG_Functions);

    // Reset values
    _DBG_FunctionCount = 0;
    _DBG_Functions = NULL;
    _DBG_UsedFlags = 0;
}

uint64_t DBG_StartSession(char *Name)
{
    extern uint32_t _DBG_FunctionCount;
    extern _DBG_FunctionData **_DBG_Functions;

    // Make sure name is not NULL
    if (Name == NULL)
    {
        _DBG_SetError(DBG_ERRORID_STARTSESSION_NAME, _DBG_ERRORMES_ARGNULL, "Name");

        return DBG_ERRORID_STARTSESSION_NAME;
    }

    // Find the function
    _DBG_FunctionData *FoundFunction = NULL;

    for (_DBG_FunctionData **FunctionList = _DBG_Functions, **EndFunctionList = _DBG_Functions + _DBG_FunctionCount; FunctionList < EndFunctionList; ++FunctionList)
        if (strcmp((*FunctionList)->name, Name) == 0)
        {
            FoundFunction = *FunctionList;
            break;
        }

    // Create new function if it didn't exist
    if (FoundFunction == NULL)
    {
        // Get memory
        FoundFunction = _DBG_CreateFunctionData(_DBG_FunctionCount, Name);

        if (FoundFunction == NULL)
        {
            _DBG_AddError(DBG_ERRORID_STARTSESSION_CREATEFUNCTION, _DBG_ERRORMES_CREATESTRUCT, "FunctionData");

            return DBG_ERRORID_STARTSESSION_CREATEFUNCTION;
        }

        _DBG_FunctionData **NewFunctionList = (_DBG_FunctionData **)realloc(_DBG_Functions, sizeof(_DBG_FunctionData *) * (_DBG_FunctionCount + 1));

        if (NewFunctionList == NULL)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_STARTSESSION_REALLOC, strerror(errno), _DBG_ERRORMES_REALLOC);

            return DBG_ERRORID_STARTSESSION_REALLOC;
        }

        _DBG_Functions = NewFunctionList;
        _DBG_Functions[_DBG_FunctionCount++] = FoundFunction;
    }

    // Create new session struct

    // Set starting time

    // Update current and first session
}

uint64_t DBG_EndSession(void)
{
    // Calculate time

    // Add time to function time list

    // Add time to subtime of parent

    // Update current and first session
}

#endif
#else
#ifndef DEBUG_INACTIVE_H_INCLUDED
#define DEBUG_INACTIVE_H_INCLUDED

// Error function should always return NULL
#define DBG_GetError() NULL

// Init and quit should alwas return no error
#define DBG_Init(ErrorLog, Flags) 0
#define DBG_Quit()

// Start and end session always return no error
#define DBG_StartSession(Name) 0
#define DBG_EndSession() 0

#endif
#endif
