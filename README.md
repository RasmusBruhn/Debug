# Debug documentation
This is the documentation for the "Debug.h" header file (DBG). This header file adds debugging and profiling tools which can be toggled on or off. When it is turned off it will not have any impact on performance. To toggle debugging on add the [macro](#DBG_ACTIVE)
```
#define DBG_ACTIVE
```
before including the Debug library.
Start the program by running [DBG_Init](#DBG_Init) to specify the settings and end the program with [DBG_Quit](#DBG_Quit).

## <a id="Profiling">Profiling</a>
----------------------------------------------------------------------------------------------------
The library adds profiling tools which will time functions and give a report of the time spent in each function. To use the profiling start all functions, you want to be timed, by calling [DBG_StartSession](#DBG_StartSession) and just before returning call [DBG_EndSession](#DBG_EndSession). After running [DBG_Quit](#DBG_Quit) a profiling file will be written.


----------------------------------------------------------------------------------------------------

## <a id="Debugging">Debugging</a>
----------------------------------------------------------------------------------------------------
The library adds debugging tools to catch memory errors like overflow errors (future update) and memory leaks which can be difficult to catch normally. The dowside is that the computer has to do extra work to check for these errors and it will therefor impact the performance of the program. It is therefor recommended that debugging is [turned on](#DBG_ACTIVE) while testing but turned off when the program is done to maximise performance.
To use the debugging exchange the normal malloc, realloc and free with [DBG_Malloc](#DBG_Malloc), [DBG_Realloc](#DBG_Realloc) and [DBG_Free](#DBG_Free) respectively. This will enable checks to see if all memory has been freed when [DBG_Quit](#DBG_Quit) is run. When debugging is not active all the debugging memory allocation functions will default back to the normal memory allocation functions.

----------------------------------------------------------------------------------------------------

## <a id="Flags">Flags</a>
----------------------------------------------------------------------------------------------------
The flags are used in the initialisation to tell the program what you want in the profiling report and how to sort it. To include more than one flag just bitwise or the flags together. Also either [DBG_FLAGS_TIME](#DBG_FLAGS_TIME) and/or [DBG_FLAGS_OWNTIME](#DBG_FLAGS_OWNTIME) must be included or nothing will be added to the report. The function will be written in the report in the order specified by the sort. To specify a sort flag use the macro function [DBG_SortFlag](#DBG_SortFlag) on one of the normal flags. One of either [DBG_FLAGS_TOTAL](#DBG_FLAGS_TOTAL), [DBG_FLAGS_AVG](#DBG_FLAGS_AVG), [DBG_FLAGS_STD](#DBG_FLAGS_STD), [DBG_FLAGS_MIN](#DBG_FLAGS_MIN) or [DBG_FLAGS_MAX](#DBG_FLAGS_MAX) and one of [DBG_FLAGS_TIME](#DBG_FLAGS_TIME) or [DBG_FLAGS_OWNTIME](#DBG_FLAGS_OWNTIME) should be used to specify how the functions in the report are to be sorted. The function with the highest value in the target stat will be the first function unless [DBG_FLAGS_INVERT](#DBG_FLAGS_INVERT) is included then it is the smallest value.

<a id="DBG_FLAGS_NOFLAG">**DBG_FLAGS_NOFLAG**</a>: Used if no other flags are to be included.

<a id="DBG_FLAGS_TOTAL">**DBG_FLAGS_TOTAL**</a>: Include the total time spent in each function.

<a id="DBG_FLAGS_AVG">**DBG_FLAGS_AVG**</a>: Include the average time spent in each function (total time divided by number of function calls).

<a id="DBG_FLAGS_STD">**DBG_FLAGS_STD**</a>: Include the standard deviation of the individual function call times from the average. This is a measure of how consistant the function call times are, a low standard deviation means that most function calls take roughly the same time.

<a id="DBG_FLAGS_LIST">**DBG_FLAGS_LIST**</a>: Adds a list of all the function call times to the report

<a id="DBG_FLAGS_MIN">**DBG_FLAGS_MIN**</a>: Include the smallest time spent in the function

<a id="DBG_FLAGS_MAX">**DBG_FLAGS_MAX**</a>: Include the longest time spent in the function

<a id="DBG_FLAGS_TIME">**DBG_FLAGS_TIME**</a>: Include what was specified from the earlier flags for the time spent in the function and in any subfunctions that are also being kept track of.

<a id="DBG_FLAGS_OWNTIME">**DBG_FLAGS_OWNTIME**</a>: Include what was specified from the earlier flags for the time spent in this function excluding the time spent in any subfunction which is also being kept track of.

<a id="DBG_FLAGS_INVERT">**DBG_FLAGS_INVERT**</a>: Only used for sorting to invert the sort order. Must beb passed through the [DBG_SortFlag](#DBG_SortFlag) function.

<a id="DBG_FLAGS_STATS">**DBG_FLAGS_STATS**</a>: Include all of the stats ([DBG_FLAGS_TOTAL](#DBG_FLAGS_TOTAL), [DBG_FLAGS_AVG](#DBG_FLAGS_AVG), [DBG_FLAGS_STD](#DBG_FLAGS_STD), [DBG_FLAGS_LIST](#DBG_FLAGS_LIST), [DBG_FLAGS_MIN](#DBG_FLAGS_MIN) and [DBG_FLAGS_MAX](#DBG_FLAGS_MAX)).

<a id="DBG_FLAGS_TYPE">**DBG_FLAGS_TYPE**</a>: Include all of the timing types ([DBG_FLAGS_TIME](#DBG_FLAGS_TIME) and [DBG_FLAGS_OWNTIME](#DBG_FLAGS_OWNTIME)).

<a id="DBG_FLAGS_ALL">**DBG_FLAGS_ALL**</a>: Include all flags ([DBG_FLAGS_STATS](#DBG_FLAGS_STATS), [DBG_FLAGS_TYPE](#DBG_FLAGS_TYPE) and [DBG_FLAGS_INVERT](#DBG_FLAGS_INVERT)).

----------------------------------------------------------------------------------------------------

## <a id="Macros">Macros</a>
----------------------------------------------------------------------------------------------------
<a id="DBG_ACTIVE">**DBG_ACTIVE**</a>: Define this macro before including the debugging library to activate it.

<a id="DBG_EXITFUNC">**DBG_EXITFUNC**</a>: This macro should be a function pointer to an exit function if a bad error has occured, read more about the syntax of this in the ErrorHandling documentation. It must be be defined before included the debugging library.

----------------------------------------------------------------------------------------------------

## <a id="Macrofunctions">Macro functions</a>
----------------------------------------------------------------------------------------------------
<a id="DBG_SortFlag">**uint32_t DBG_SortFlag(uint32_t Flag)**</a>:
- Converts a normal flag to a sort flags which is used to determine how to sort the profiling report. [See Flags](#Flags) for more info.
- **Return**: The converted flag.
- **Arguments**:
  - **Flags**: The [flags](#Flags) which whould be converted.

----------------------------------------------------------------------------------------------------

## <a id="Functions">Functions</a>
----------------------------------------------------------------------------------------------------
<a id="DBG_Init">**uint64_t DBG_Init(FILE *ProfileLog, FILE *RunningLog, FILE *ErrorLog, uint64_t Flags, uint32_t Precision)**</a>:
- Initialises debugging, this function must be at the start of the program and if debugging is inactive then this function does nothing.
- **Return**: DBG_ERRORID_NOERROR on success and an error ID on failure.
- **Arguments**:
  - **ProfileLog**: The file to write the profile report to when [DBG_Quit](#DBG_Quit) is run. The file must be in w mode.
  - **RunningLog**: A file to which the debugging program will log everything during runtime, parse NULL to disable the running log. This must also be in w mode.
  - **ErrorLog**: The file to write errors when they occur, parse NULL to disable error logging. This file must be in w+ mode.
  - **Flags**: All [flags](#Flags) used, the flags determine what to write in the profile log and how to sort it.
  - **Precision**: The profile log will print the stats in %.*g notation, this is how many decimal points to use (the number instead of the start).

<a id="DBG_Quit">**void DBG_Quit(void)**</a>:
- Writes the profile log, closes down everything and frees all allocated memory for this library. It also resets all values the the initial values in this library so it can be initialised again. If debugging is inactive then this function does nothing.
- **Return**: Void.

<a id="DBG_StartSession">**uint64_t DBG_StartSession(char *Name)**</a>:
- Starts a session (a function to time) and gives a name to it. This function should be run as the first thing in any funtion which should be timed. If debugging is inactive then this function does nothing. Remember to run [DBG_EndSession](#DBG_EndSession) at the end of the function.
- **Return**: DBG_ERRORID_NOERROR and success or an error ID on failure.
- **Arguments**:
  - **Name**: The name of the session, this name should be unique and it is the same name which will be written in the profile log.

<a id="DBG_EndSession">**uint64_t DBG_EndSession(void)**</a>:
- Ends a session. This function should be called before any return statement in a function where a [DBG_StartSession](#DBG_StartSession) has been used. If debugging is inactive then this function does nothing.
- **Return**: DBG_ERRORID_NOERROR on success or an error ID on failure.

<a id="DBG_Malloc">**void *DBG_Malloc(size_t Size, char *Name)**</a>:
- Allocates a chunk of memory just like malloc and keeps track of allocated memory to check for overlows and memory leaks. If debugging is inactive this function defaults to malloc(Size).
- **Return**: A pointer to the allocated memory, NULL on error.
- **Arguments**:
  - **Size**: The size, in bytes, of the memory which should be allocated.
  - **Name**: The name of the allocated memory, this does not have to be unique and can be NULL, it is only used to make the logs easier to read.

<a id="DBG_Realloc">**void *DBG_Realloc(void *Pointer, size_t Size, char *Name)**</a>:
- Reallocates a chunk of memory just like realloc and keeps track of allocated memory to check for overlows and memory leaks. If debugging is inactive this function defaults to realloc(Pointer, Size).
- **Return**: A pointer to the new reallocated memory, NULL on error.
- **Arguments**:
  - **Pointer**: The pointer to the original memory to be reallocated, if NULL it will allocate new memory.
  - **Size**: The size, in bytes, of the new block of memory which should be allocated, if 0 it will free the memory.
  - **Name**: The name of the allocated memory, this does not have to be unique, if NULL then it will keep its old name, it is only used to make the logs easier to read.

<a id="DBG_Free">**void DBG_Free(void *Pointer)**</a>:
- Frees a chunk of memory just like free and keeps track of allocated memory to check for overlows and memory leaks. If debugging is inactive this function defaults to free(Pointer).
- **Return**: Void.
- **Arguments**:
  - **Pointer**: The pointer to the memory to be freed.
  
----------------------------------------------------------------------------------------------------
