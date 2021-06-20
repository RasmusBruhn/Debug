#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <stdinc.h>

// If it is active
#ifdef DBG_ACTIVE

// Structs
typedef struct __DBG_Stats DBG_Stats;
typedef struct __DBG_Session DBG_Session;

struct __DBG_Stats
{
    char *name;              // The name of the session
    uint32_t count;          // How many times this session has been run
    uint64_t timeAvg;        // The average time it has been open
    uint64_t timeAvgExcl;    // The average time it has been open excluding time when newer sessions have opened
    uint64_t timeStd;        // The standard deviation of the time it has been open
    uint64_t timeStdExcl;    // The standard deviation of the time it has been open exlcuding time when newer sessions have opened
    uint64_t timeMin;        // The minimum time of a session
    uint64_t timeMinExcl;    // The minimum time of a session exlcuding time when newer sessions have opened
    uint64_t timeMax;        // The maximum time of a session
    uint64_t timeMaxExcl;    // The maximum time of a session exlcuding time when newer sessions have opened
    uint64_t timeTotal;      // The total time in this session type
    uint64_t timeTotalExcl;  // The total time in this session type exlcuding time when newer sessions have opened
};

struct __DBG_Session
{
    char *name;              // The name of the session
    uint64_t *time;          // The time it has been open
    uint64_t *timeExcl;      // The time it has been open excluding time when newer sessions have opened
    uint64_t *timeStart;     // The starting time of the session for all sessions
    uint64_t *timeStartExcl; // The starting time of the session + the time in newer sessions
    void **memory;           // The memory initialized in this session
    uint32_t count;          // How many times this session has been run
    uint32_t openCount;      // How many sessions are open (How long the timeStart lists are)
    uint32_t memoryCount;    // How many memory allocations has been done in this session
};

// Flags
enum DBG_Flags
{
    DBG_FLAGS_NONE = 0x00000000,
    DBG_FLAGS_WRITELOG = 0x00000001,
    DBG_FLAGS_WRITESTATS = 0x00000002,
    DBG_FLAGS_SHOWTIMEAVG = 0x00000004,
    DBG_FLAGS_SHOWTIMEAVGEXCL = 0x00000008,
    DBG_FLAGS_SHOWTIMESTD = 0x00000010,
    DBG_FLAGS_SHOWTIMESTDEXCL = 0x00000020,
    DBG_FLAGS_SHOWTIMEMIN = 0x00000040,
    DBG_FLAGS_SHOWTIMEMINEXCL = 0x00000080,
    DBG_FLAGS_SHOWTIMEMAX = 0x00000100,
    DBG_FLAGS_SHOWTIMEMAXEXCL = 0x00000200,
    DBG_FLAGS_SHOWTIMETOTAL = 0x00000400,
    DBG_FLAGS_SHOWTIMETOTALEXCL = 0x00000800,
    DBG_FLAGS_SHOWNAME = 0x00001000,
    DBG_FLAGS_SHOWCOUNT = 0x00002000,
    DBG_FLAGS_SORTNEGATIVE = 0x80000000,
    DBG_FLAGS_ALL = 0x0FFFFFFF
};

// Define functions

// Initializes debugging
// Returns nothing
// Flags: The settings of this debugging session
// LogName: The name of the log file without the extension
// StatsName: The name of the stats file without the extension
void __DBG_Init(const char *LogName, const char *StatsName, uint32_t Flags);

// Ends debugging
// Returns nothing
// Sort: What the stats list should be sorted by
void __DBG_Quit(enum DBG_Flags Sort);

// Starts a new session for debugging (recommended to use at the start of every function that needs keeping track of)
// Returns nothing
// ID: The id of the session
void __DBG_SessionStart(const char *ID);

// Ends a session for debugging (recommended to use at the end of every function that needs keeping track of)
// Returns nothing
void __DBG_SessionEnd(void);

// malloc() with a hook to keep track of used memory
// Returns the requested pointer and NULL on error
// Size: The number of bytes for the allocated memory
void *__DBG_Malloc(size_t Size);

// realloc() with a hook to keep track of used memory
// Returns the requested pointer and NULL on error (unless Size is 0)
// Pointer: The pointer to reallocate
// Size: The new size of the allocated memory
void *__DBG_Realloc(void *Pointer, size_t Size);

// free() with a hook to keep track of used memory
// Returns nothing
// Pointer: The pointer to free
void __DBG_Free(void *Pointer);

// Opens a file with the name "[Name]_[Number].txt", with number large enough to be unique
// Returns the file or NULL on error
// Name: The base name of the file
// Extension: The extension of the file, like .txt
FILE *_DBG_OpenFile(const char *Name, const char *Extension);

// Writes the stats file
// Returns nothing
// Sort: What the file is sorted after
void _DBG_WriteStats(enum DBG_Flags Sort);

// Initializes a session structure
// Returns nothing
// Session: The structure to initialize
void _DBG_InitSession(DBG_Session *Session);

// Creates a session struct
// Returns a pointer to the struct
// ID: The name/ID of the stats
DBG_Session *_DBG_CreateSession(const char *ID, uint32_t Pos);

// Destroys a session struct
// Returns nothing
// Session: The session struct to destroy
void _DBG_DestroySession(DBG_Session *Session);

// Compares the names of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareName(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the count of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareCount(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the total time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeTotal(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the exclusive total time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeTotalExcl(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the average time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeAvg(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the exclusive average time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeAvgExcl(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the standard deviation of time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeStd(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the exclusive standard deviation of time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeStdExcl(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the min time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeMin(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the exclusive min time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeMinExcl(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the max time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeMax(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Compares the exclusive max time of the 2 stats
// Returns integer, negative if Stats1 is smaller than Stats2, 0 if they are equal and positive if Stats1 is larger than Stats2
// Stats1: The first stats struct to compare
// Stats2: The second stats struct to compare
int32_t _DBG_CompareTimeMaxExcl(DBG_Stats *Stats1, DBG_Stats *Stats2);

// Macro functions

// malloc() with a hook to keep track of used memory
// Returns the requested pointer and NULL on error
// Size: The number of bytes for the allocated memory
#define DBG_Malloc(Size) __DBG_Malloc(Size)

// realloc() with a hook to keep track of used memory
// Returns the requested pointer and NULL on error (unless Size is 0)
// Pointer: The pointer to reallocate
// Size: The new size of the allocated memory
#define DBG_Realloc(Pointer, Size) __DBG_Realloc(size)

// free() with a hook to keep track of used memory
// Returns nothing
// Pointer: The pointer to free
#define DBG_Free(Pointer) __DBG_Free(Pointer)

// Starts a new session for debugging (recommended to use at the start of every function that needs keeping track of)
// Returns nothing
// ID: The id of the session
#define DBG_SessionStart(ID) __DBG_SessionStart(ID)

// Ends a session for debugging (recommended to use at the end of every function that needs keeping track of)
// Returns nothing
#define DBG_SessionEnd() __DBG_SessionEnd()

// Initializes debugging
// Returns nothing
// Flags: The settings of this debugging session
// LogName: The name of the log file without the extension
// StatsName: The name of the stats file without the extension
#define DBG_Init(LogName, StatsName, Flags) __DBG_Init(LogName, StatsName, Flags)

// Ends debugging
// Returns nothing
// Sort: What the stats list should be sorted by
#define DBG_Quit(Sort) __DBG_Quit(Sort)

// Definitions

// File names
#define _DBG_FILENAME_LOG "DebugLog"
#define _DBG_FILENAME_STATS "DebugStats"

// File extensions
#define _DBG_FILEEXTENSION_LOG ".txt"
#define _DBG_FILEEXTENSION_STATS ".csv"
#define _DBG_FILEEXTENSION_DATA ".csv"

// Generel file convensions
#define _DBG_FOLDERDELIMITER '/'
#define _DBG_CURRENTFOLDER "."
#define _DBG_NUMBERDELIMITER "_"
#define _DBG_NUMBERS "0123456789"
#define _DBG_CSVDELIMITER ";"

// Category
#define _DBG_CATEGORYNAME_NAME "Name"
#define _DBG_CATEGORYNAME_COUNT "Count"
#define _DBG_CATEGORYNAME_TIMETOTAL "Total Time"
#define _DBG_CATEGORYNAME_TIMETOTALEXCL "Total Time Exclusive"
#define _DBG_CATEGORYNAME_TIMEAVG "Average Time"
#define _DBG_CATEGORYNAME_TIMEAVGEXCL "Average Time Exclusive"
#define _DBG_CATEGORYNAME_TIMESTD "Standard Deviation of Time"
#define _DBG_CATEGORYNAME_TIMESTDEXCL "Standard Deviation of Time Exclusive"
#define _DBG_CATEGORYNAME_TIMEMIN "Minimum Time"
#define _DBG_CATEGORYNAME_TIMEMINEXCL "Minimum Time Exclusive"
#define _DBG_CATEGORYNAME_TIMEMAX "Maximum Time"
#define _DBG_CATEGORYNAME_TIMEMAXEXCL "Maximum Time Exclusive"

// Log text
#define _DBG_LOG_INDENT " "
#define _DBG_LOG_LINESTART "%lu: "
#define _DBG_LOG_STARTSESSION "Starting session: %s\n"
#define _DBG_LOG_ENDSESSION "Ending session: %s\n"
#define _DBG_LOG_MALLOC "Allocated memory: %lX (%lu)\n"
#define _DBG_LOG_REALLOC "Reallocated memory: %lX -> %lX (%lu)\n"
#define _DBG_LOG_FREE "Freed memory: %lX\n"
#define _DBG_LOG_MEMORYLEAK "Memory leak: %lX\n"

// Error messages
#define _DBG_ERROR_INITIALIZED "Debugging has already been initialized\n"
#define _DBG_ERROR_MEMORY "Unable to initialize memory: %s\n"
#define _DBG_ERROR_OPENDIR "Unable to open directory: %s, %s\n"
#define _DBG_ERROR_OPENFILE "Unable to open file: %s, %s\n"
#define _DBG_ERROR_WRONGSORT "Unable to sort with ID: %u\n"
#define _DBG_ERROR_NOFILE "File is not open\n"
#define _DBG_ERROR_NOOPENSESSIONS "No open sessions\n"
#define _DBG_ERROR_NOTSTARTED "This session has not been started: %s\n"

// Debugging running name
#define _DBG_RUNNING_NAME "Debugging Running"

// Global variables

// List of open sessions
static DBG_Session **_DBG_OpenSession = NULL;
static uint32_t _DBG_OpenSessionCount = 0;

// List of all the stats
static DBG_Session **_DBG_AllSession = NULL;
static uint32_t _DBG_AllSessionCount = 0;

// The flags used
static uint32_t _DBG_Flags = DBG_FLAGS_NONE;

// Files
static FILE *_DBG_FileLog = NULL;
static FILE *_DBG_FileStats = NULL;

// If it has been initialized
static bool _DBG_IsInitialized = false;

// Functions
void __DBG_Init(const char *LogName, const char *StatsName, uint32_t Flags)
{
    // Make sure that it has not been initialized before
    extern bool _DBG_IsInitialized;

    if (_DBG_IsInitialized)
    {
        printf(_DBG_ERROR_INITIALIZED);
        exit(0);
    }

    // Set flags
    extern uint32_t _DBG_Flags;

    _DBG_Flags = Flags;

    // Open files
    extern FILE *_DBG_FileLog;
    extern FILE *_DBG_FileStats;

    if (Flags & DBG_FLAGS_WRITELOG)
        _DBG_FileLog = _DBG_OpenFile(((LogName == NULL) ? (_DBG_FILENAME_LOG) : (LogName)), _DBG_FILEEXTENSION_LOG);

    if (Flags & DBG_FLAGS_WRITESTATS)
        _DBG_FileStats = _DBG_OpenFile(((StatsName == NULL) ? (_DBG_FILENAME_STATS) : (StatsName)), _DBG_FILEEXTENSION_STATS);

    // Set it to have been initialized
    _DBG_IsInitialized = true;

    // Start the running session
    DBG_SessionStart(_DBG_RUNNING_NAME);
}

void __DBG_Quit(enum DBG_Flags Sort)
{
    // End running session
    DBG_SessionEnd();

    // Write the stats and data file
    extern uint32_t _DBG_Flags;
    
    if (_DBG_Flags & DBG_FLAGS_WRITESTATS)
        _DBG_WriteStats(Sort);

    // Free used memory and write the rest of log
    extern DBG_Session **_DBG_OpenSession;
    extern DBG_Session **_DBG_AllSession;
    extern uint32_t _DBG_OpenSessionCount;
    extern uint32_t _DBG_AllSessionCount;
    extern FILE *_DBG_FileLog;
    extern FILE *_DBG_FileStats;

    if (_DBG_OpenSession != NULL)
        free(_DBG_OpenSession);

    if (_DBG_AllSession != NULL)
    {
        for (DBG_Session **Var = _DBG_AllSession, **EndVar = Var + _DBG_AllSessionCount; Var < EndVar; ++Var)
        {
            // Write to log what memory was not freed
            if (_DBG_Flags & DBG_FLAGS_WRITELOG && _DBG_FileLog != NULL)
                for (void **Var2 = (*Var)->memory, **VarEnd2 = (*Var)->memory + (*Var)->memoryCount; Var2 < VarEnd2; ++Var2)
                    fprintf(_DBG_FileLog, _DBG_LOG_MEMORYLEAK, (uint64_t)(*Var2));

            _DBG_DestroySession(*Var);
        }

        free(_DBG_AllSession);
    }

    _DBG_OpenSession = NULL;
    _DBG_AllSession = NULL;

    _DBG_OpenSessionCount = 0;
    _DBG_AllSessionCount = 0;

    // Close files
    if (_DBG_FileLog != NULL)
        fclose(_DBG_FileLog);

    if (_DBG_FileStats != NULL)
        fclose(_DBG_FileStats);

    _DBG_FileLog = NULL;
    _DBG_FileStats = NULL;

    // Set it to not be initialized
    extern bool _DBG_IsInitialized;

    _DBG_IsInitialized = false;
}

void __DBG_SessionStart(const char *ID)
{
    // Get time stamp
    uint64_t Time = clock();

    // Find session
    extern DBG_Session **_DBG_AllSession;
    extern uint32_t _DBG_AllSessionCount;
    DBG_Session **SessionMin = _DBG_AllSession - 1;
    DBG_Session **SessionMax = _DBG_AllSession + _DBG_AllSessionCount;
    DBG_Session **SessionPos;
    int32_t Status;

    // Go through and locate the session
    while (SessionMin + 1 < SessionMax)
    {
        // Find the middle
        SessionPos = SessionMin + (SessionMax - SessionMin) / 2;

        // Test if it is too high, too low or it has found the session
        Status = strcmp(ID, (*SessionPos)->name);

        // If it is too high
        if (Status < 0)
            SessionMax = SessionPos;

        // If it is too low
        else if (Status > 0)
            SessionMin = SessionMax;

        // If it has found it
        else
            break;
    }

    DBG_Session* Session;

    // If it found it
    if (SessionMin + 1 < SessionMax)
        Session = *SessionPos;

    // If it did not find it, create new session
    else
        Session = _DBG_CreateSession(ID, SessionMax - _DBG_AllSession);

    // Get memory to add time
    Session->timeStart = (uint64_t *)realloc(Session->timeStart, sizeof(uint64_t) * ++Session->openCount);
    Session->timeStartExcl = (uint64_t *)realloc(Session->timeStartExcl, sizeof(uint64_t) * Session->openCount);

    if (Session->timeStart == NULL || Session->timeStartExcl == NULL)
    {
        printf(_DBG_ERROR_MEMORY, strerror(errno));
        exit(0);
    }

    // Set the times
    Session->timeStart[Session->openCount - 1] = Time;
    Session->timeStartExcl[Session->openCount - 1] = Time;

    // Add session to the open session list
    extern DBG_Session **_DBG_OpenSession;
    extern uint32_t _DBG_OpenSessionCount;

    // Extend list
    _DBG_OpenSession = (DBG_Session **)realloc(_DBG_OpenSession, sizeof(DBG_Session *) * ++_DBG_OpenSessionCount);

    if (_DBG_OpenSession == NULL)
    {
        printf(_DBG_ERROR_MEMORY, strerror(errno));
        exit(0);
    }

    // Add to list
    _DBG_OpenSession[_DBG_OpenSessionCount - 1] = Session;

    // Write log
    extern uint32_t _DBG_Flags;
    extern FILE *_DBG_FileLog;

    if ((_DBG_Flags & DBG_FLAGS_WRITELOG) && _DBG_FileLog != NULL)
    {
        // Write indent
        for (uint32_t Var = 1; Var < _DBG_OpenSessionCount; ++Var)
            fprintf(_DBG_FileLog, _DBG_LOG_INDENT);

        // Write start
        fprintf(_DBG_FileLog, _DBG_LOG_LINESTART, Time);

        // Write main line
        fprintf(_DBG_FileLog, _DBG_LOG_STARTSESSION, ID);
    }
}

void __DBG_SessionEnd(void)
{
    // Get time stamp
    uint64_t Time = clock();

    // Get memory for times
    extern DBG_Session **_DBG_OpenSession;
    extern uint32_t _DBG_OpenSessionCount;

    if (_DBG_OpenSession  == NULL)
    {
        printf(_DBG_ERROR_NOOPENSESSIONS);
        exit(0);
    }

    DBG_Session *Session = _DBG_OpenSession[--_DBG_OpenSessionCount];

    if (Session->openCount <= 0)
    {
        printf(_DBG_ERROR_NOTSTARTED, Session->name);
        exit(0);
    }

    Session->time = (uint64_t *)realloc(Session->time, sizeof(uint64_t) * ++Session->count);
    Session->timeExcl = (uint64_t *)realloc(Session->timeExcl, sizeof(uint64_t) * Session->count);

    if (Session->time == NULL || Session->timeExcl == NULL)
    {
        printf(_DBG_ERROR_MEMORY, strerror(errno));
        exit(0);
    }

    // Calculate times
    Session->time[Session->count - 1] = Time - Session->timeStart[--Session->openCount];
    Session->timeExcl[Session->count - 1] = Time - Session->timeStartExcl[Session->openCount];

    // Remove the starting times
    Session->timeStart = (uint64_t *)realloc(Session->timeStart, sizeof(uint64_t) * Session->openCount);
    Session->timeStartExcl = (uint64_t *)realloc(Session->timeStartExcl, sizeof(uint64_t) * Session->openCount);

    // Update exclusive time for next session
    if (_DBG_OpenSessionCount > 0 && _DBG_OpenSession[_DBG_OpenSessionCount - 1]->timeStartExcl != NULL)
        _DBG_OpenSession[_DBG_OpenSessionCount - 1]->timeStartExcl[_DBG_OpenSession[_DBG_OpenSessionCount - 1]->openCount - 1] -= Session->time[Session->count - 1];

    // Remove from open session list
    _DBG_OpenSession = (DBG_Session **)realloc(_DBG_OpenSession, sizeof(DBG_Session *) * _DBG_OpenSessionCount);

    // Write log
    extern uint32_t _DBG_Flags;
    extern FILE *_DBG_FileLog;

    if ((_DBG_Flags & DBG_FLAGS_WRITELOG) && _DBG_FileLog != NULL)
    {
        // Write indent
        for (uint32_t Var = 0; Var < _DBG_OpenSessionCount; ++Var)
            fprintf(_DBG_FileLog, _DBG_LOG_INDENT);

        // Write start
        fprintf(_DBG_FileLog, _DBG_LOG_LINESTART, Time);

        // Write main line
        fprintf(_DBG_FileLog, _DBG_LOG_ENDSESSION, Session->name);
    }
}

void *__DBG_Malloc(size_t Size)
{
    // Get time
    uint64_t Time = clock();

    // Allocate memory
    void *Memory = malloc(Size);

    // Add it to allocated memory list
    extern DBG_Session **_DBG_OpenSession;
    extern uint32_t _DBG_OpenSessionCount;

    DBG_Session *Session = _DBG_OpenSession[_DBG_OpenSessionCount - 1];

    if (Memory != NULL)
    {
        Session->memory = (void **)realloc(Session->memory, sizeof(void *) * ++Session->memoryCount);

        if (Session->memory == NULL)
        {
            printf(_DBG_ERROR_MEMORY, strerror(errno));
            exit(0);
        }

        Session->memory[Session->memoryCount - 1] = Memory;
    }

    // Write log
    extern uint32_t _DBG_Flags;
    extern FILE *_DBG_FileLog;

    if ((_DBG_Flags & DBG_FLAGS_WRITELOG) && _DBG_FileLog != NULL)
    {
        // Write indent
        for (uint32_t Var = 0; Var < _DBG_OpenSessionCount; ++Var)
            fprintf(_DBG_FileLog, _DBG_LOG_INDENT);

        // Write start
        fprintf(_DBG_FileLog, _DBG_LOG_LINESTART, Time);

        // Write main line
        fprintf(_DBG_FileLog, _DBG_LOG_MALLOC, (uint64_t)Memory, Size);
    }

    return Memory;
}

void *__DBG_Realloc(void *Pointer, size_t Size)
{
    // Send to free if size is 0
    if (Size == 0)
    {
        __DBG_Free(Pointer);
        return NULL;
    }

    // Get time
    uint64_t Time = clock();

    // Reallocate memory
    void *Memory = realloc(Pointer, Size);

    // Remove it from old memory list
    extern DBG_Session **_DBG_AllSession;
    extern uint32_t _DBG_AllSessionCount;

    // Go through all sessions
    for (DBG_Session **Var = _DBG_AllSession, **VarEnd = _DBG_AllSession + _DBG_AllSessionCount; Var < VarEnd; ++Var)
    {
        bool Stop = false;

        // Go through all the memory
        for (void **Var2 = (*Var)->memory, **VarEnd2 = (*Var)->memory + (*Var)->memoryCount; Var2 < VarEnd2; ++Var2)
            if (*Var2 == Pointer)
            {
                // Remove it from the list
                for (++Var2; Var2 < VarEnd2; ++Var2)
                    *(Var2 - 1) = *Var2;

                (*Var)->memory = (void **)realloc((*Var)->memory, sizeof(void *) * --(*Var)->memoryCount);

                Stop = true;
                break;
            }

        if (Stop)
            break;
    }

    // Add it to allocated memory list
    extern DBG_Session **_DBG_OpenSession;
    extern uint32_t _DBG_OpenSessionCount;

    DBG_Session *Session = _DBG_OpenSession[_DBG_OpenSessionCount - 1];

    if (Memory != NULL)
    {
        Session->memory = (void **)realloc(Session->memory, sizeof(void *) * ++Session->memoryCount);

        if (Session->memory == NULL)
        {
            printf(_DBG_ERROR_MEMORY, strerror(errno));
            exit(0);
        }

        Session->memory[Session->memoryCount - 1] = Memory;
    }

    // Write log
    extern uint32_t _DBG_Flags;
    extern FILE *_DBG_FileLog;

    if ((_DBG_Flags & DBG_FLAGS_WRITELOG) && _DBG_FileLog != NULL)
    {
        // Write indent
        for (uint32_t Var = 0; Var < _DBG_OpenSessionCount; ++Var)
            fprintf(_DBG_FileLog, _DBG_LOG_INDENT);

        // Write start
        fprintf(_DBG_FileLog, _DBG_LOG_LINESTART, Time);

        // Write main line
        fprintf(_DBG_FileLog, _DBG_LOG_REALLOC, (uint64_t)Pointer, (uint64_t)Memory, Size);
    }

    return Memory;
}

void __DBG_Free(void *Pointer)
{
    // Get time
    uint64_t Time = clock();

    // Free memory
    free(Pointer);

    // Remove it from old memory list
    extern DBG_Session **_DBG_AllSession;
    extern uint32_t _DBG_AllSessionCount;

    // Go through all sessions
    for (DBG_Session **Var = _DBG_AllSession, **VarEnd = _DBG_AllSession + _DBG_AllSessionCount; Var < VarEnd; ++Var)
    {
        bool Stop = false;

        // Go through all the memory
        for (void **Var2 = (*Var)->memory, **VarEnd2 = (*Var)->memory + (*Var)->memoryCount; Var2 < VarEnd2; ++Var2)
            if (*Var2 == Pointer)
            {
                // Remove it from the list
                for (++Var2; Var2 < VarEnd2; ++Var2)
                    *(Var2 - 1) = *Var2;

                (*Var)->memory = (void **)realloc((*Var)->memory, sizeof(void *) * --(*Var)->memoryCount);

                Stop = true;
                break;
            }

        if (Stop)
            break;
    }

    // Write log
    extern uint32_t _DBG_Flags;
    extern FILE *_DBG_FileLog;

    if ((_DBG_Flags & DBG_FLAGS_WRITELOG) && _DBG_FileLog != NULL)
    {
        // Write indent
        for (uint32_t Var = 0; Var < _DBG_OpenSessionCount; ++Var)
            fprintf(_DBG_FileLog, _DBG_LOG_INDENT);

        // Write start
        fprintf(_DBG_FileLog, _DBG_LOG_LINESTART, Time);

        // Write main line
        fprintf(_DBG_FileLog, _DBG_LOG_FREE, (uint64_t)Pointer);
    }
}

FILE *_DBG_OpenFile(const char *Name, const char *Extension)
{
    // Figure out what directory to use
    const char *DirectoryEnd = strrchr(Name, _DBG_FOLDERDELIMITER);

    // Get the start of the name
    const char *NameStart = ((DirectoryEnd == NULL) ? (Name) : (DirectoryEnd + 1));
    uint32_t NameLength = strlen(NameStart);

    // Get length of directory name
    uint32_t Length = ((DirectoryEnd == NULL) ? (strlen(_DBG_CURRENTFOLDER)) : (DirectoryEnd - Name));

    // Get name of directory
    char *DirectoryName = (char *)malloc(sizeof(char) * (Length + 1));

    if (DirectoryName == NULL)
    {
        printf(_DBG_ERROR_MEMORY, strerror(errno));
        exit(0);
    }

    // Copy the name
    strncpy(DirectoryName, ((DirectoryEnd == NULL) ? (_DBG_CURRENTFOLDER) : (Name)), Length);
    DirectoryName[Length] = '\0';

    // Open directory
    DIR *Directory = opendir(DirectoryName);
    free(DirectoryName);

    if (Directory == NULL)
    {
        printf(_DBG_ERROR_OPENDIR, DirectoryName, strerror(errno));
        exit(0);
    }

    // Go through all the files and find same name
    struct dirent *Dir;
    uint32_t MaxNumber = 0;

    while ((Dir = readdir(Directory)) != NULL)
        if (Dir->d_type == DT_REG && strncmp(NameStart, Dir->d_name, NameLength) == 0 && strlen(Dir->d_name) > NameLength + strlen(_DBG_NUMBERDELIMITER) && strncmp(Dir->d_name + NameLength, _DBG_NUMBERDELIMITER, strlen(_DBG_NUMBERDELIMITER)) == 0)
        {
            uint32_t NumberLength = strspn(Dir->d_name + NameLength + strlen(_DBG_NUMBERDELIMITER), _DBG_NUMBERS);

            if (strncmp(Dir->d_name + NameLength + strlen(_DBG_NUMBERDELIMITER) + NumberLength, Extension, strlen(Extension)) == 0)
            {
                uint32_t Number = atoi(Dir->d_name + NameLength + strlen(_DBG_NUMBERDELIMITER));

                if (Number > MaxNumber)
                    MaxNumber = Number;
            }
        }

    closedir(Directory);

    // Create the file name
    char *FileName = (char *)malloc(sizeof(char) * (strlen(Name) + strlen(_DBG_NUMBERDELIMITER) + (uint32_t)(log10(MaxNumber + 1) + 1) + strlen(Extension) + 1));

    if (FileName == NULL)
    {
        printf(_DBG_ERROR_MEMORY, strerror(errno));
        exit(0);
    }

    sprintf(FileName, "%s%s%u%s", Name, _DBG_NUMBERDELIMITER, MaxNumber + 1, Extension);

    // Open the file
    FILE *File = fopen(FileName, "w");

    if (File == NULL)
    {
        printf(_DBG_ERROR_OPENFILE, FileName, strerror(errno));
        exit(0);
    }

    free(FileName);

    return File;
}

void _DBG_WriteStats(enum DBG_Flags Sort)
{
    // Sort the sessions depending on stats
    // Go through all the sessions
    extern DBG_Session **_DBG_AllSession;
    extern uint32_t _DBG_AllSessionCount;

    // Get the compare function
    int32_t (*CompareFunc)(DBG_Stats *Stats1, DBG_Stats *Stats2);
    int32_t Sign = 1;

    if (Sort & DBG_FLAGS_SORTNEGATIVE)
    {
        Sign = -1;
        Sort -= DBG_FLAGS_SORTNEGATIVE;
    }

    switch (Sort)
    {
        case (DBG_FLAGS_SHOWNAME):
            CompareFunc = &_DBG_CompareName;
            break;

        case (DBG_FLAGS_SHOWCOUNT):
            CompareFunc = &_DBG_CompareCount;
            break;

        case (DBG_FLAGS_SHOWTIMETOTAL):
            CompareFunc = &_DBG_CompareTimeTotal;
            break;

        case (DBG_FLAGS_SHOWTIMETOTALEXCL):
            CompareFunc = &_DBG_CompareTimeTotalExcl;
            break;

        case (DBG_FLAGS_SHOWTIMEAVG):
            CompareFunc = &_DBG_CompareTimeAvg;
            break;

        case (DBG_FLAGS_SHOWTIMEAVGEXCL):
            CompareFunc = &_DBG_CompareTimeAvgExcl;
            break;

        case (DBG_FLAGS_SHOWTIMESTD):
            CompareFunc = &_DBG_CompareTimeStd;
            break;

        case (DBG_FLAGS_SHOWTIMESTDEXCL):
            CompareFunc = &_DBG_CompareTimeStdExcl;
            break;

        case (DBG_FLAGS_SHOWTIMEMIN):
            CompareFunc = &_DBG_CompareTimeMin;
            break;

        case (DBG_FLAGS_SHOWTIMEMINEXCL):
            CompareFunc = &_DBG_CompareTimeMinExcl;
            break;

        case (DBG_FLAGS_SHOWTIMEMAX):
            CompareFunc = &_DBG_CompareTimeMax;
            break;

        case (DBG_FLAGS_SHOWTIMEMAXEXCL):
            CompareFunc = &_DBG_CompareTimeMaxExcl;
            break;

        default:
            printf(_DBG_ERROR_WRONGSORT, Sort);
            exit(0);
            break;
    }

    // Initialize memory
    DBG_Stats *StatsList[_DBG_AllSessionCount];
    DBG_Stats **StatsEnd = StatsList;

    // Calculate stats
    for (DBG_Session **Var = _DBG_AllSession, **VarEnd = Var + _DBG_AllSessionCount; Var < VarEnd; ++Var)
    {
        // Create stats struct
        DBG_Stats *Stats = (DBG_Stats *)malloc(sizeof(DBG_Stats));

        if (Stats == NULL)
        {
            printf(_DBG_ERROR_MEMORY, strerror(errno));
            exit(0);
        }

        // Set name
        Stats->name = (*Var)->name;
        
        // Set count
        Stats->count = (*Var)->count;

        // Find stats
        Stats->timeTotal = 0;
        Stats->timeStd = 0;
        Stats->timeMin = -1;
        Stats->timeMax = 0;
        
        for (uint64_t *Var2 = (*Var)->time, *VarEnd2 = Var2 + (*Var)->count; Var2 < VarEnd2; ++Var2)
        {
            // Add to total time
            Stats->timeTotal += *Var2;

            // Add to variance
            Stats->timeStd += (*Var2) * (*Var2);

            // Find min
            if (*Var2 < Stats->timeMin)
                Stats->timeMin = *Var2;

            // Find max
            if (*Var2 > Stats->timeMax)
                Stats->timeMax = *Var2;
        }

        // Find the average
        Stats->timeAvg = Stats->timeTotal / Stats->count;

        // Find standard deviation
        Stats->timeStd = sqrt((double)Stats->timeStd / (double)Stats->count - (double)Stats->timeAvg * (double)Stats->timeAvg);

        Stats->timeTotalExcl = 0;
        Stats->timeStdExcl = 0;
        Stats->timeMinExcl = -1;
        Stats->timeMaxExcl = 0;

        for (uint64_t *Var2 = (*Var)->timeExcl, *VarEnd2 = Var2 + (*Var)->count; Var2 < VarEnd2; ++Var2)
        {
            // Add to total time
            Stats->timeTotalExcl += *Var2;

            // Add to variance
            Stats->timeStdExcl += (*Var2) * (*Var2);

            // Find min
            if (*Var2 < Stats->timeMinExcl)
                Stats->timeMinExcl = *Var2;

            // Find max
            if (*Var2 > Stats->timeMaxExcl)
                Stats->timeMaxExcl = *Var2;
        }

        // Find the average
        Stats->timeAvgExcl = Stats->timeTotalExcl / Stats->count;

        // Find standard deviation
        Stats->timeStdExcl = sqrt((double)Stats->timeStdExcl / (double)Stats->count - (double)Stats->timeAvgExcl * (double)Stats->timeAvgExcl);
    
        // Sort list
        DBG_Stats **StatsPos;

        for (StatsPos = StatsEnd; StatsPos > StatsList && Sign * CompareFunc(Stats, *(StatsPos - 1)) > 0; --StatsPos)
            *StatsPos = *(StatsPos - 1);

        *StatsPos = Stats;
        ++StatsEnd;
    }

    // Write the data
    extern FILE *_DBG_FileStats;
    extern uint32_t _DBG_Flags;

    if (_DBG_FileStats == NULL)
    {
        printf(_DBG_ERROR_NOFILE);
        exit(0);
    }

    // Write the categories
    fprintf(_DBG_FileStats, "%s", _DBG_CATEGORYNAME_NAME);

    if (_DBG_Flags & DBG_FLAGS_SHOWCOUNT)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_COUNT);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMETOTAL)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMETOTAL);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMEAVG)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMEAVG);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMESTD)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMESTD);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMEMIN)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMEMIN);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMEMAX)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMEMAX);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMETOTALEXCL)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMETOTALEXCL);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMEAVGEXCL)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMEAVGEXCL);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMESTDEXCL)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMESTDEXCL);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMEMINEXCL)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMEMINEXCL);

    if (_DBG_Flags & DBG_FLAGS_SHOWTIMEMAXEXCL)
        fprintf(_DBG_FileStats, "%s%s", _DBG_CSVDELIMITER, _DBG_CATEGORYNAME_TIMEMAXEXCL);

    // Go through all stats and write it
    for (DBG_Stats **Stats = StatsList; Stats < StatsEnd; ++Stats)
    {
        fprintf(_DBG_FileStats, "%s", (*Stats)->name);

        if (_DBG_Flags & DBG_FLAGS_SHOWCOUNT)
            fprintf(_DBG_FileStats, "%s%u", _DBG_CSVDELIMITER, (*Stats)->count);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMETOTAL)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeTotal);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMEAVG)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeAvg);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMESTD)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeStd);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMEMIN)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeMin);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMEMAX)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeMax);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMETOTALEXCL)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeTotalExcl);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMEAVGEXCL)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeAvgExcl);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMESTDEXCL)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeStdExcl);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMEMINEXCL)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeMinExcl);

        if (_DBG_Flags & DBG_FLAGS_SHOWTIMEMAXEXCL)
            fprintf(_DBG_FileStats, "%s%lu", _DBG_CSVDELIMITER, (*Stats)->timeMaxExcl);
    }

    // Free all memory again
    for (DBG_Stats **Var = StatsList, **VarEnd = StatsList + _DBG_AllSessionCount; Var < VarEnd; ++Var)
        free(*Var);
}

void _DBG_InitSession(DBG_Session *Session)
{
    Session->name = NULL;
    Session->time = NULL;
    Session->timeExcl = NULL;
    Session->timeStart = NULL;
    Session->timeStartExcl = NULL;
    Session->count = 0;
    Session->openCount = 0;
}

DBG_Session *_DBG_CreateSession(const char *ID, uint32_t Pos)
{
    // Get memory for session
    DBG_Session *Session = (DBG_Session *)malloc(sizeof(DBG_Session));

    if (Session == NULL)
    {
        printf(_DBG_ERROR_MEMORY, strerror(errno));
        exit(0);
    }

    // Initialize
    _DBG_InitSession(Session);

    // Get memory for name
    Session->name = (char *)malloc(sizeof(char) * (strlen(ID) + 1));

    if (Session->name == NULL)
    {
        printf(_DBG_ERROR_MEMORY, strerror(errno));
        exit(0);
    }

    // Set name
    sprintf(Session->name, "%s", ID);

    // Add it to the session list
    extern DBG_Session **_DBG_AllSession;
    extern uint32_t _DBG_AllSessionCount;

    // Get space
    _DBG_AllSession = realloc(_DBG_AllSession, sizeof(DBG_Session *) * ++_DBG_AllSessionCount);

    if (_DBG_AllSession == NULL)
    {
        printf(_DBG_ERROR_MEMORY, strerror(errno));
        exit(0);
    }

    // Move other sessions
    for (DBG_Session **Var = _DBG_AllSession + _DBG_AllSessionCount - 1, **VarEnd = _DBG_AllSession + Pos; Var > VarEnd; --Var)
        *Var = *(Var - 1);

    // Set the new session at the location
    _DBG_AllSession[Pos] = Session;

    return Session;
}

void _DBG_DestroySession(DBG_Session *Session)
{
    // Remove name
    if (Session->name != NULL)
        free(Session->name);

    // Remove start times
    if (Session->timeStart != NULL)
        free(Session->timeStart);

    if (Session->timeStartExcl != NULL)
        free(Session->timeStartExcl);

    // Remove times
    if (Session->time != NULL)
        free(Session->time);

    if (Session->timeExcl != NULL)
        free(Session->timeExcl);

    // Free the struct
    free(Session);
}

int32_t _DBG_CompareName(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return strcmp(Stats1->name, Stats2->name);
}

int32_t _DBG_CompareCount(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->count - Stats2->count;
}

int32_t _DBG_CompareTimeTotal(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeTotal - Stats2->timeTotal;
}

int32_t _DBG_CompareTimeTotalExcl(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeTotalExcl - Stats2->timeTotalExcl;
}

int32_t _DBG_CompareTimeAvg(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeAvg - Stats2->timeAvg;
}

int32_t _DBG_CompareTimeAvgExcl(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeAvgExcl - Stats2->timeAvgExcl;
}

int32_t _DBG_CompareTimeStd(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeStd - Stats2->timeStd;
}

int32_t _DBG_CompareTimeStdExcl(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeStdExcl - Stats2->timeStdExcl;
}

int32_t _DBG_CompareTimeMin(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeMin - Stats2->timeMin;
}

int32_t _DBG_CompareTimeMinExcl(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeMinExcl - Stats2->timeMinExcl;
}

int32_t _DBG_CompareTimeMax(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeMax - Stats2->timeMax;
}

int32_t _DBG_CompareTimeMaxExcl(DBG_Stats *Stats1, DBG_Stats *Stats2)
{
    return Stats1->timeMaxExcl - Stats2->timeMaxExcl;
}

// If it is inactive
#else

// malloc() with a hook to keep track of used memory
// Returns the requested pointer and NULL on error
// Size: The number of bytes for the allocated memory
#define DBG_Malloc(Size) malloc(Size)

// realloc() with a hook to keep track of used memory
// Returns the requested pointer and NULL on error (unless Size is 0)
// Pointer: The pointer to reallocate
// Size: The new size of the allocated memory
#define DBG_Realloc(Pointer, Size) realloc(Pointer, Size)

// free() with a hook to keep track of used memory
// Returns nothing
// Pointer: The pointer to free
#define DBG_Free(Pointer) free(Pointer)

// Starts a new session for debugging (recommended to use at the start of every function that needs keeping track of)
// Returns nothing
// ID: The id of the session
#define DBG_SessionStart(ID)

// Ends a session for debugging (recommended to use at the end of every function that needs keeping track of)
// Returns nothing
#define DBG_SessionEnd()

// Initializes debugging
// Returns nothing
// Flags: The settings of this debugging session
// LogName: The name of the log file without the extension
// StatsName: The name of the stats file without the extension
// DataName: The name of the data file without the extension
#define DBG_Init(LogName, StatsName, DataName, Flags)

// Ends debugging
// Returns nothing
// Sort: What the stats list should be sorted by
#define DBG_Quit(Sort)

#endif
#endif