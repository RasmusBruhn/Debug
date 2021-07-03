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
#include <math.h>

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
    uint64_t *time;         // List of time spent in each of the sessions with time in sub sessions
    uint64_t *ownTime;      // List of time spent in each of the sessions without time in sub sessions
};

// Contants
// Errors
enum DBG_ErrorID
{
    DBG_ERRORID_NOERROR = 0x00000000,
    DBG_ERRORID_INIT_MALLOC = 0x100010300,
    DBG_ERRORID_INIT_INIT = 0x100010201,
    DBG_ERRORID_INIT_PROFILELOG = 0x100010302,
    DBG_ERRORID_QUIT_INIT = 0x100020300,
    DBG_ERRORID_QUIT_NULL = 0x100020101,
    DBG_ERRORID_QUIT_SESSION = 0x100020102,
    DBG_ERRORID_QUIT_PROFILE = 0x100020303,
    DBG_ERRORID_CREATESESSION_MALLOC = 0x100030200,
    DBG_ERRORID_CREATEFUNCTIONDATA_MALLOC = 0x100040200,
    DBG_ERRORID_PRINTSESSION_LONG = 0x100050100,
    DBG_ERRORID_PRINTFUNCTIONDATA_LONG1 = 0x100060100,
    DBG_ERRORID_PRINTFUNCTIONDATA_LONG2 = 0x100060101,
    DBG_ERRORID_PRINTFUNCTIONDATA_LONG3 = 0x100060102,
    DBG_ERRORID_STARTSESSION_NAME = 0x100070300,
    DBG_ERRORID_STARTSESSION_CREATEFUNCTION = 0x100070301,
    DBG_ERRORID_STARTSESSION_REALLOC = 0x100070302,
    DBG_ERRORID_STARTSESSION_CREATESESSION = 0x100070303,
    DBG_ERRORID_STARTSESSION_RUNNINGLOG = 0x100070104,
    DBG_ERRORID_STARTSESSION_INIT = 0x100070305,
    DBG_ERRORID_ENDSESSION_NONEOPEN = 0x100080300,
    DBG_ERRORID_ENDSESSION_REALLOC = 0x100080301,
    DBG_ERRORID_ENDSESSION_INIT = 0x100080302,
    DBG_ERRORID_ENDSESSION_RUNNINGLOG = 0x100080103,
    DBG_ERRORID_CREATEPROFILE_PRINT1 = 0x100090300,
    DBG_ERRORID_CREATEPROFILE_PRINT2 = 0x100090301,
    DBG_ERRORID_CREATEPROFILE_PRINT3 = 0x100090302,
    DBG_ERRORID_CREATEPROFILE_FUNCTION = 0x100090303,
    DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIME1 = 0x100090300,
    DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIME2 = 0x100090301,
    DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIMESTATS = 0x100090302,
    DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIME3 = 0x100090303,
    DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIME1 = 0x100090304,
    DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIME2 = 0x100090305,
    DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIMESTATS = 0x100090306,
    DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIME3 = 0x100090307,
    DBG_ERRORID_CREATEPROFILESTATS_TOTAL1 = 0x1000A0300,
    DBG_ERRORID_CREATEPROFILESTATS_TOTAL2 = 0x1000A0301,
    DBG_ERRORID_CREATEPROFILESTATS_AVG1 = 0x1000A0302,
    DBG_ERRORID_CREATEPROFILESTATS_AVG2 = 0x1000A0303,
    DBG_ERRORID_CREATEPROFILESTATS_STD1 = 0x1000A0304,
    DBG_ERRORID_CREATEPROFILESTATS_STD2 = 0x1000A0305,
    DBG_ERRORID_CREATEPROFILESTATS_LIST1 = 0x1000A0306,
    DBG_ERRORID_CREATEPROFILESTATS_LIST2 = 0x1000A0307,
    DBG_ERRORID_CREATEPROFILESTATS_LIST3 = 0x1000A0308,
    DBG_ERRORID_CREATEPROFILESTATS_LIST4 = 0x1000A0309,
    DBG_ERRORID_CREATEPROFILESTATS_LIST5 = 0x1000A030A,
    DBG_ERRORID_CREATEPROFILESTATS_MIN1 = 0x1000A030B,
    DBG_ERRORID_CREATEPROFILESTATS_MIN2 = 0x1000A030C,
    DBG_ERRORID_CREATEPROFILESTATS_MAX1 = 0x1000A030D,
    DBG_ERRORID_CREATEPROFILESTATS_MAX2 = 0x1000A030E
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
#define _DBG_ERRORMES_NOOPENSESSION "Attempting to close session with no open sessions"
#define _DBG_ERRORMES_WRITEFILE "Unable to write to the \"%s\" file"
#define _DBG_ERRORMES_WHILEWRITING "An error occured while writing %s"

// Flags
enum DBG_Flags
{
    DBG_FLAGS_NOFLAG = 0x00,
    DBG_FLAGS_TOTAL = 0x01,
    DBG_FLAGS_AVG = 0x02,
    DBG_FLAGS_STD = 0x04,
    DBG_FLAGS_LIST = 0x08,
    DBG_FLAGS_MIN = 0x10,
    DBG_FLAGS_MAX = 0x20,
    DBG_FLAGS_TIME = 0x0100,
    DBG_FLAGS_OWNTIME = 0x0200
};

#define DBG_FLAGS_STATS 0xFF
#define DBG_FLAGS_TYPE 0xFF00
#define DBG_FLAGS_ALL 0xFFFF

// texts for profile
#define _DBG_PROFILETEXT_TOTAL "Total"
#define _DBG_PROFILETEXT_AVG "Avg"
#define _DBG_PROFILETEXT_STD "Std"
#define _DBG_PROFILETEXT_MIN "Min"
#define _DBG_PROFILETEXT_MAX "Max"
#define _DBG_PROFILETEXT_LIST "Data"
#define _DBG_PROFILETEXT_TIME "Total Time"
#define _DBG_PROFILETEXT_OWNTIME "Own Time"

// Running log messages
#define _DBG_RUNNINGLOG_STARTSESSION "%*.lu: Started session \"%s\"\n"
#define _DBG_RUNNINGLOG_ENDSESSION "%*.lu: Ended session   \"%s\", TotalTime = %lu, OwnTime = %lu\n"

// struct print max length
#define DBG_PRINTSTRUCT_MAXLENGTH 1000
#define DBG_PRINTSTRUCT_LISTMAXLENGTH 5

// Internal format string max length
#define _DBG_FORMATSTRING_MAXLENGTH 1000

// global variables
// The outer most session, NULL if no session has been started
_DBG_Session *_DBG_FirstSession = NULL;
_DBG_Session *_DBG_CurrentSession = NULL;

// List of all the functions
_DBG_FunctionData _DBG_FunctionMain = {.ID = 0, .name = "main", .count = 0, .time = NULL, .ownTime = NULL};
_DBG_FunctionData **_DBG_Functions = NULL;
uint32_t _DBG_FunctionCount = 0;

// Flags for what to save
uint64_t _DBG_UsedFlags = 0;

// File to save the profile
FILE *_DBG_ProfileLog = NULL;

// File to save running log
FILE *_DBG_RunningLog = NULL;

// Precision of profile
uint32_t _DBG_Precision = 2;

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

// Writes the profile file
// Returns 0 on success and error code on failure
uint64_t _DBG_CreateProfile(void);

// Write the profile for one function
// Returns 0 on success and error code on failure
// FunctionData: The function to print
uint64_t _DBG_CreateProfileFunctionData(_DBG_FunctionData *FunctionData);

// Writes all the stats for one set of timing data
// Returns 0 on success and error code on failure
// Time: The time array to get stats from
// Count: The length of the time array
uint64_t _DBG_CreateProfileStats(uint64_t *Time, uint32_t Count);

// Initialises debugging
// Returns 0 on success and an error code on failure
// ProfileLog: The log to write the final statistics in
// RunningLog: The log to write every event in
// ErrorLog: A text file pointer to where it can write the error messages, NULL if it should not write any messages
// Flags: Contains all the information for what data to save, options are from DBG_Flags
// Precision: How many digits to write in the statistics, if 0 it will use the default
uint64_t DBG_Init(FILE *ProfileLog, FILE *RunningLog, FILE *ErrorLog, uint64_t Flags, uint32_t Precision);

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
    FunctionData->time = NULL;
    FunctionData->ownTime = NULL;
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

    if (FunctionData->ownTime != NULL)
        free(FunctionData->ownTime);

    // Free the struct
    free(FunctionData);
}

char *_DBG_PrintSession(const _DBG_Session *Session)
{
    // Print everything to a string
    extern char _DBG_PrintStructString[];

    int32_t Length = snprintf(_DBG_PrintStructString, DBG_PRINTSTRUCT_MAXLENGTH, "{ID = %u, startTime = %lu, subTime = %lu, removeTime = %lu, child = %lX, parent = %lX, depth = %u}",
                                                                                   Session->ID, Session->startTime, Session->subTime, Session->removeTime, (uint64_t)Session->child, (uint64_t)Session->parent, Session->depth);

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
    char OwnTimeString[DBG_PRINTSTRUCT_MAXLENGTH];

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

    Length = snprintf(OwnTimeString, DBG_PRINTSTRUCT_MAXLENGTH, "[");
    String = OwnTimeString + Length;
    MaxLength = DBG_PRINTSTRUCT_MAXLENGTH - Length;

    for (uint64_t *TimeList = FunctionData->ownTime, *EndTimeList = FunctionData->ownTime + FunctionData->count; TimeList < EndTimeList; ++TimeList)
    {
        // Check if there are too many elements
        if (TimeList >= FunctionData->ownTime + DBG_PRINTSTRUCT_LISTMAXLENGTH)
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
    Length = snprintf(_DBG_PrintStructString, DBG_PRINTSTRUCT_MAXLENGTH, "{ID = %u, name = \"%s\", count = %u, time = %s, ownTime = %s}",
                                                                           FunctionData->ID, FunctionData->name, FunctionData->count, TimeString, OwnTimeString);

    if (Length >= DBG_PRINTSTRUCT_MAXLENGTH)
    {
        _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH - 2] = '.';
        _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH - 3] = '.';
        _DBG_PrintStructString[DBG_PRINTSTRUCT_MAXLENGTH - 4] = '.';

        _DBG_SetError(DBG_ERRORID_PRINTFUNCTIONDATA_LONG3, _DBG_ERRORMES_LONGPRINT, Length, DBG_PRINTSTRUCT_MAXLENGTH - 1);
    }

    return _DBG_PrintStructString;
}

uint64_t _DBG_CreateProfile(void)
{
    extern FILE *_DBG_ProfileLog;
    extern uint32_t _DBG_FunctionCount;
    extern _DBG_FunctionData **_DBG_Functions;

    // Loop through all the functions
    int32_t Length;
    bool FirstLine = true;

    for (_DBG_FunctionData **FunctionList = _DBG_Functions, **EndFunctionList = _DBG_Functions + _DBG_FunctionCount; FunctionList < EndFunctionList; ++FunctionList)
    {
        // Add , when not the first line
        if (!FirstLine)
        {
            Length = fprintf(_DBG_ProfileLog, ",\n\n");

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILE_PRINT1, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILE_PRINT1;
            }
        }

        // Write the name of the function
        Length = fprintf(_DBG_ProfileLog, "%s = {\n", (*FunctionList)->name);

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILE_PRINT2, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILE_PRINT2;
        }

        // Write the function data
        if (_DBG_CreateProfileFunctionData(*FunctionList) != 0)
        {
            _DBG_AddError(DBG_ERRORID_CREATEPROFILE_FUNCTION, _DBG_ERRORMES_WHILEWRITING, "function data");

            return DBG_ERRORID_CREATEPROFILE_FUNCTION;
        }

        // Write the ending bracket
        Length = fprintf(_DBG_ProfileLog, "\n}");

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILE_PRINT3, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILE_PRINT3;
        }

        FirstLine = false;
    }
    
    return DBG_ERRORID_NOERROR;
}

uint64_t _DBG_CreateProfileFunctionData(_DBG_FunctionData *FunctionData)
{
    extern uint64_t _DBG_UsedFlags;
    extern FILE *_DBG_ProfileLog;

    // Go through the different time data and print it
    bool FirstLine = true;
    int32_t Length;

    // Write the total time stats
    if (_DBG_UsedFlags & DBG_FLAGS_TIME)
    {
        // Add , when not the first line
        if (!FirstLine)
        {
            Length = fprintf(_DBG_ProfileLog, ",\n\n");

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIME1, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIME1;
            }
        }

        // Write the name
        Length = fprintf(_DBG_ProfileLog, "    %s = {\n", _DBG_PROFILETEXT_TIME);

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIME2, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIME2;
        }

        // Write the stats
        if (_DBG_CreateProfileStats(FunctionData->time, FunctionData->count))
        {
            _DBG_AddError(DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIMESTATS, _DBG_ERRORMES_WHILEWRITING, "statistics");

            return DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIMESTATS;
        }

        // Write the ending bracket
        Length = fprintf(_DBG_ProfileLog, "\n    }");

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIME3, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_TIME3;
        }

        FirstLine = false;
    }

    // Write the own time stats
    if (_DBG_UsedFlags & DBG_FLAGS_OWNTIME)
    {
        // Add , when not the first line
        if (!FirstLine)
        {
            Length = fprintf(_DBG_ProfileLog, ",\n\n");

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIME1, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIME1;
            }
        }

        // Write the name
        Length = fprintf(_DBG_ProfileLog, "    %s = {\n", _DBG_PROFILETEXT_OWNTIME);

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIME2, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIME2;
        }

        // Write the stats
        if (_DBG_CreateProfileStats(FunctionData->ownTime, FunctionData->count))
        {
            _DBG_AddError(DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIMESTATS, _DBG_ERRORMES_WHILEWRITING, "statistics");

            return DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIMESTATS;
        }

        // Write the ending bracket
        Length = fprintf(_DBG_ProfileLog, "\n    }");

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIME3, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILEFUNCTIONDATA_OWNTIME3;
        }

        FirstLine = false;
    }

    return DBG_ERRORID_NOERROR;
}

uint64_t _DBG_CreateProfileStats(uint64_t *Time, uint32_t Count)
{
    extern uint64_t _DBG_UsedFlags;
    extern FILE *_DBG_ProfileLog;
    extern uint32_t _DBG_Precision;

    // Calculate the statistics
    uint64_t Total = 0;
    uint64_t TotalSqr = 0;
    uint64_t Min = -1;
    uint64_t Max = 0;

    for (uint64_t *Data = Time, *EndData = Time + Count; Data < EndData; ++Data)
    {
        Total += *Data;
        TotalSqr += (*Data) * (*Data);

        if (*Data < Min)
            Min = *Data;

        if (*Data > Max)
            Max = *Data;
    }

    bool FirstLine = true;
    int32_t Length;

    // Write the total time
    if (_DBG_UsedFlags & DBG_FLAGS_TOTAL)
    {
        // Add , when not the first line
        if (!FirstLine)
        {
            Length = fprintf(_DBG_ProfileLog, ",\n");

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_TOTAL1, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILESTATS_TOTAL1;
            }
        }

        // Write the data
        Length = fprintf(_DBG_ProfileLog, "        %s = %.*g", _DBG_PROFILETEXT_TOTAL, _DBG_Precision, (double)Total / (double)CLOCKS_PER_SEC);

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_TOTAL2, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILESTATS_TOTAL2;
        }

        FirstLine = false;
    }

    // Write the average time
    if (_DBG_UsedFlags & DBG_FLAGS_AVG)
    {
        // Add , when not the first line
        if (!FirstLine)
        {
            Length = fprintf(_DBG_ProfileLog, ",\n");

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_AVG1, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILESTATS_AVG1;
            }
        }

        // Write the data
        Length = fprintf(_DBG_ProfileLog, "        %s = %.*g", _DBG_PROFILETEXT_AVG, _DBG_Precision, (double)Total / (double)(CLOCKS_PER_SEC * Count));

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_AVG2, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILESTATS_AVG2;
        }

        FirstLine = false;
    }

    // Write the standard deviation of the average time
    if (_DBG_UsedFlags & DBG_FLAGS_STD)
    {
        // Add , when not the first line
        if (!FirstLine)
        {
            Length = fprintf(_DBG_ProfileLog, ",\n");

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_STD1, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILESTATS_STD1;
            }
        }

        // Write the data
        Length = fprintf(_DBG_ProfileLog, "        %s = %.*g", _DBG_PROFILETEXT_STD, _DBG_Precision, sqrt((double)(TotalSqr - Total * Total)) / (double)(CLOCKS_PER_SEC * Count));

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_STD2, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILESTATS_STD2;
        }

        FirstLine = false;
    }

    // Write the minimum time
    if (_DBG_UsedFlags & DBG_FLAGS_MIN)
    {
        // Add , when not the first line
        if (!FirstLine)
        {
            Length = fprintf(_DBG_ProfileLog, ",\n");

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_MIN1, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILESTATS_MIN1;
            }
        }

        // Write the data
        Length = fprintf(_DBG_ProfileLog, "        %s = %.*g", _DBG_PROFILETEXT_MIN, _DBG_Precision, (double)Min / (double)CLOCKS_PER_SEC);

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_MIN2, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILESTATS_MIN2;
        }

        FirstLine = false;
    }

    // Write the maximum time
    if (_DBG_UsedFlags & DBG_FLAGS_MAX)
    {
        // Add , when not the first line
        if (!FirstLine)
        {
            Length = fprintf(_DBG_ProfileLog, ",\n");

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_MAX1, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILESTATS_MAX1;
            }
        }

        // Write the data
        Length = fprintf(_DBG_ProfileLog, "        %s = %.*g", _DBG_PROFILETEXT_MAX, _DBG_Precision, (double)Max / (double)CLOCKS_PER_SEC);

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_MAX2, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILESTATS_MAX2;
        }

        FirstLine = false;
    }

    // Write all the data
    if (_DBG_UsedFlags & DBG_FLAGS_LIST)
    {
        // Add , when not the first line
        if (!FirstLine)
        {
            Length = fprintf(_DBG_ProfileLog, ",\n");

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_LIST1, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILESTATS_LIST1;
            }
        }

        // Write the start of the data
        Length = fprintf(_DBG_ProfileLog, "        %s = [", _DBG_PROFILETEXT_LIST);

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_LIST2, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILESTATS_LIST2;
        }

        // Write the data
        bool FirstData = true;

        for (uint64_t *Data = Time, *EndData = Time + Count; Data < EndData; ++Data)
        {
            // Write the ,
            if (!FirstData)
            {
                Length = fprintf(_DBG_ProfileLog, ", ");

                if (Length < 0)
                {
                    _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_LIST3, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                    return DBG_ERRORID_CREATEPROFILESTATS_LIST3;
                }
            }

            Length = fprintf(_DBG_ProfileLog, "%lu", *Data);

            if (Length < 0)
            {
                _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_LIST4, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

                return DBG_ERRORID_CREATEPROFILESTATS_LIST4;
            }

            FirstData = false;
        }

        // Write the end of the data
        Length = fprintf(_DBG_ProfileLog, "]");

        if (Length < 0)
        {
            _DBG_AddErrorForeign(DBG_ERRORID_CREATEPROFILESTATS_LIST5, strerror(errno), _DBG_ERRORMES_WRITEFILE, "ProfileLog");

            return DBG_ERRORID_CREATEPROFILESTATS_LIST5;
        }

        FirstLine = false;
    }

    return DBG_ERRORID_NOERROR;
}

uint64_t DBG_Init(FILE *ProfileLog, FILE *RunningLog, FILE *ErrorLog, uint64_t Flags, uint32_t Precision)
{
    extern _DBG_FunctionData **_DBG_Functions;
    extern uint32_t _DBG_FunctionCount;
    extern uint64_t _DBG_UsedFlags;
    extern FILE *_DBG_RunningLog;
    extern FILE *_DBG_ProfileLog;
    extern _DBG_FunctionData _DBG_FunctionMain;
    extern uint32_t _DBG_Precision;

    // Make sure input is valid
    if (ProfileLog == NULL)
    {
        _DBG_SetError(DBG_ERRORID_INIT_PROFILELOG, _DBG_ERRORMES_ARGNULL, "ProfileLog");

        return DBG_ERRORID_INIT_PROFILELOG;
    }

    // Make sure it has not been initialised already
    if (_DBG_Functions != NULL)
    {
        _DBG_SetError(DBG_ERRORID_INIT_INIT, _DBG_ERRORMES_ALREADYINIT);

        return DBG_ERRORID_INIT_INIT;
    }
        
    // Set error log file
    _DBG_SetLogFile(ErrorLog);

    // Set running log
    _DBG_RunningLog = RunningLog;

    // Set profile log
    _DBG_ProfileLog = ProfileLog;

    // Allocate memory for the functions
    _DBG_Functions = (_DBG_FunctionData **)malloc(sizeof(_DBG_FunctionData *));
    
    if (_DBG_Functions == NULL)
    {
        _DBG_AddErrorForeign(DBG_ERRORID_INIT_MALLOC, strerror(errno), _DBG_ERRORMES_MALLOC);

        return DBG_ERRORID_INIT_MALLOC;
    }

    _DBG_FunctionCount = 1;

    // Setup the main function
    *_DBG_Functions = &_DBG_FunctionMain;

    // Setup flags
    _DBG_UsedFlags = Flags;

    // Setup the precision
    if (Precision != 0)
        _DBG_Precision = Precision;

    return DBG_ERRORID_NOERROR;
}

void DBG_Quit(void)
{
    extern _DBG_FunctionData **_DBG_Functions;
    extern uint32_t _DBG_FunctionCount;
    extern uint64_t _DBG_UsedFlags;
    extern FILE *_DBG_RunningLog;
    extern FILE *_DBG_ProfileLog;

    // Make sure it has been initialised
    if (_DBG_Functions == NULL)
    {
        _DBG_SetError(DBG_ERRORID_QUIT_INIT, _DBG_ERRORMES_NOINIT);

        return;
    }

    // Write profile
    if (_DBG_CreateProfile() != 0)
    {
        _DBG_AddError(DBG_ERRORID_QUIT_PROFILE, _DBG_ERRORMES_WHILEWRITING, "profile");

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
    _DBG_ProfileLog = NULL;
    _DBG_RunningLog = NULL;

    // Reset the log file
    _DBG_SetLogFile(NULL);
}

uint64_t DBG_StartSession(char *Name)
{
    uint64_t StartTime = clock();

    extern uint32_t _DBG_FunctionCount;
    extern _DBG_FunctionData **_DBG_Functions;

    // Make sure name is not NULL
    if (Name == NULL)
    {
        _DBG_SetError(DBG_ERRORID_STARTSESSION_NAME, _DBG_ERRORMES_ARGNULL, "Name");

        return DBG_ERRORID_STARTSESSION_NAME;
    }

    // Make sure it has been initialised
    if (_DBG_Functions == NULL)
    {
        _DBG_SetError(DBG_ERRORID_STARTSESSION_INIT, _DBG_ERRORMES_NOINIT);

        return DBG_ERRORID_STARTSESSION_INIT;
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

        // Insert the new function
        _DBG_Functions = NewFunctionList;
        _DBG_Functions[_DBG_FunctionCount++] = FoundFunction;
    }

    // Create new session struct
    extern _DBG_Session *_DBG_CurrentSession;
    extern _DBG_Session *_DBG_FirstSession;
    uint32_t Depth = ((_DBG_CurrentSession == NULL) ? (0) : (_DBG_CurrentSession->depth + 1));

    _DBG_Session *NewSession = _DBG_CreateSession(FoundFunction->ID, _DBG_CurrentSession, Depth);

    if (NewSession == NULL)
    {
        _DBG_AddError(DBG_ERRORID_STARTSESSION_CREATESESSION, _DBG_ERRORMES_CREATESTRUCT, "Session");

        return DBG_ERRORID_STARTSESSION_CREATESESSION;
    }
    
    // Set starting time
    NewSession->startTime = StartTime;

    // Update current and first session
    if (_DBG_CurrentSession != NULL)
        _DBG_CurrentSession->child = NewSession;

    _DBG_CurrentSession = NewSession;

    if (_DBG_FirstSession == NULL)
        _DBG_FirstSession = NewSession;

    // Write to running log
    extern FILE *_DBG_RunningLog;

    if (_DBG_RunningLog != NULL)
        if (fprintf(_DBG_RunningLog, _DBG_RUNNINGLOG_STARTSESSION, 14 + NewSession->depth, StartTime, Name) < 0)
            _DBG_AddErrorForeign(DBG_ERRORID_STARTSESSION_RUNNINGLOG, _DBG_ERRORMES_WRITEFILE, "RunningLog");

    uint64_t EndTime = clock();

    NewSession->removeTime = EndTime - StartTime;

    return DBG_ERRORID_NOERROR;
}

uint64_t DBG_EndSession(void)
{
    uint64_t StartTime = clock();

    extern _DBG_Session *_DBG_CurrentSession;
    extern _DBG_Session *_DBG_FirstSession;

    // Make sure it has been initialised
    if (_DBG_Functions == NULL)
    {
        _DBG_SetError(DBG_ERRORID_ENDSESSION_INIT, _DBG_ERRORMES_NOINIT);

        return DBG_ERRORID_ENDSESSION_INIT;
    }

    // Make sure something is open
    if (_DBG_CurrentSession == NULL)
    {
        _DBG_SetError(DBG_ERRORID_ENDSESSION_NONEOPEN, _DBG_ERRORMES_NOOPENSESSION);
        return DBG_ERRORID_ENDSESSION_NONEOPEN;
    }

    // Calculate time
    uint64_t TotalTime = StartTime - _DBG_CurrentSession->startTime - _DBG_CurrentSession->removeTime;
    uint64_t OwnTime = TotalTime - _DBG_CurrentSession->subTime;

    // Add time to function time list
    extern _DBG_FunctionData **_DBG_Functions;

    uint64_t *NewTime = (uint64_t *)realloc(_DBG_Functions[_DBG_CurrentSession->ID]->time, sizeof(uint64_t) * (_DBG_Functions[_DBG_CurrentSession->ID]->count + 1));
    uint64_t *NewOwnTime = (uint64_t *)realloc(_DBG_Functions[_DBG_CurrentSession->ID]->ownTime, sizeof(uint64_t) * (_DBG_Functions[_DBG_CurrentSession->ID]->count + 1));

    if (NewTime == NULL || NewOwnTime == NULL)
    {
        _DBG_AddErrorForeign(DBG_ERRORID_ENDSESSION_REALLOC, strerror(errno), _DBG_ERRORMES_REALLOC);
        return DBG_ERRORID_ENDSESSION_REALLOC;
    }

    NewTime[_DBG_Functions[_DBG_CurrentSession->ID]->count] = TotalTime;
    NewOwnTime[_DBG_Functions[_DBG_CurrentSession->ID]->count++] = OwnTime;

    _DBG_Functions[_DBG_CurrentSession->ID]->time = NewTime;
    _DBG_Functions[_DBG_CurrentSession->ID]->ownTime = NewOwnTime;

    // Add time to subtime and removeSubTime and removeTime of parent
    if (_DBG_CurrentSession->parent != NULL)
    {
        _DBG_CurrentSession->parent->removeTime += _DBG_CurrentSession->removeTime;
        _DBG_CurrentSession->parent->subTime += TotalTime;
        _DBG_CurrentSession->parent->child = NULL;
    }

    // Uppdate first and current session
    else
        _DBG_FirstSession = NULL;

    _DBG_Session *Session = _DBG_CurrentSession;
    _DBG_CurrentSession = _DBG_CurrentSession->parent;

    // Write to running log
    extern FILE *_DBG_RunningLog;

    if (_DBG_RunningLog != NULL)
        if (fprintf(_DBG_RunningLog, _DBG_RUNNINGLOG_ENDSESSION, 14 + Session->depth, StartTime, _DBG_Functions[Session->ID]->name, TotalTime, OwnTime) < 0)
            _DBG_AddErrorForeign(DBG_ERRORID_ENDSESSION_RUNNINGLOG, _DBG_ERRORMES_WRITEFILE, "RunningLog");

    // free session
    _DBG_DestroySession(Session);

    // Add removeTime to parent
    uint64_t EndTime = clock();

    if (_DBG_CurrentSession != NULL)
        _DBG_CurrentSession->removeTime += EndTime - StartTime;

    return DBG_ERRORID_NOERROR;
}

#endif
#else
#ifndef DEBUG_INACTIVE_H_INCLUDED
#define DEBUG_INACTIVE_H_INCLUDED

// Error function should always return NULL
#define DBG_GetError() NULL

// Init and quit should alwas return no error
#define DBG_Init(ProfileLog, RunningLog, ErrorLog, Flags, Precision) 0
#define DBG_Quit()

// Start and end session always return no error
#define DBG_StartSession(Name) 0
#define DBG_EndSession() 0

#endif
#endif
