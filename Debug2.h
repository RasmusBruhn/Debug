#ifndef DEBUG_DEFINED
#define DEBUG_DEFINED

#include <stdio.h>
#include <stdlib.h>

#ifndef DEBUG_DEFINED2
#define ERR_PREFIX DBG
#include <Error2.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

// Allocates memory with malloc but keeps track of allocated memory
// Returns the allocated buffer
// File: The name of the file
// Line: The line number of this function call
// Size: The size of the memory in bytes
void *_DBG_Malloc(const char *File, size_t Line, size_t Size);

// Reallocates memory with realloc but keeps track of allocated memory
// Returns the allocated buffer
// File: The name of the file
// Line: The line number of this function call
// Pointer: The pointer to the memory to be reallocated
// Size: The size of the memory in bytes
void *_DBG_Realloc(const char *File, size_t Line, void *Pointer, size_t Size);

// Frees memory with free but keeps track of allocated memory
// Returns nothing
// Pointer: The pointer to the memory to be freed
void _DBG_Free(void *Pointer);

// Writes a list of all still allocated memory to a file, cannot be run twice
// File: The file to write to
void DBG_MemoryLog(FILE *File);

// Prints a list of all still allocated memory, cannot be run twice
#define DBG_MemoryPrint() DBG_MemoryLog(stdout)

// Overwrite standard functions
#ifndef DEBUG_DEFINED2
#define malloc(Size) _DBG_Malloc(__FILE__, __LINE__, Size)
#define realloc(Pointer, Size) _DBG_Realloc(__FILE__, __LINE__, Pointer, Size)
#define free(Pointer) _DBG_Free(Pointer)
#endif

#ifdef __cplusplus
}
#endif

#endif