#ifndef DEBUG_DEFINED
#define DEBUG_DEFINED

#include <stdio.h>

// Allocates memory with malloc but keeps track of allocated memory
// Returns the allocated buffer
// File: The name of the file
// Line: The line number of this function call
// Size: The size of the memory in bytes
void *__DBG_Malloc(const char *File, size_t Line, size_t Size);

// Reallocates memory with realloc but keeps track of allocated memory
// Returns the allocated buffer
// File: The name of the file
// Line: The line number of this function call
// Pointer: The pointer to the memory to be reallocated
// Size: The size of the memory in bytes
void *__DBG_Realloc(const char *File, size_t Line, void *Pointer, size_t Size);

// Frees memory with free but keeps track of allocated memory
// Returns nothing
// File: The name of the file
// Line: The line number of this function call
// Pointer: The pointer to the memory to be freed
void __DBG_Free(const char *File, size_t Line, void *Pointer);

// Adds a memory block to be kept track of
// File: The name of the file
// Line: The line number of this function call
// Pointer: The pointer to the memory block
void _DBG_MemoryAdd(const char *File, size_t Line, void *Pointer);

// Removes a memory block from the list of traked memory
// Pointer: The pointer to the memory block
void _DBG_MemoryRemove(void *Pointer);

// Writes a list of all still allocated memory to a file
// File: The file to write to
void DBG_MemoryLog(FILE *File);

// Allocates memory with malloc but keeps track of allocated memory
// Returns the allocated buffer
// Size: The size of the memory in bytes
#define _DBG_Malloc(Size) __DBG_Malloc(__FILE__, __LINE__, Size)

// Reallocates memory with realloc but keeps track of allocated memory
// Returns the allocated buffer
// Pointer: The pointer to the memory to be reallocated
// Size: The size of the memory in bytes
#define _DBG_Realloc(Pointer, Size) __DBG_Realloc(__FILE__, __LINE__, Pointer, Size)

// Frees memory with free but keeps track of allocated memory
// Returns nothing
// Pointer: The pointer to the memory to be freed
#define _DBG_Free(Pointer) __DBG_Free(__FILE__, __LINE__, Pointer)

// Prints a list of all still allocated memory
#define DBG_MemoryPrint() DBG_MemoryLog(stdout)

// Overwrite standard functions
#define malloc(Size) _DBG_Malloc(Size)
#define realloc(Pointer, Size) _DBG_Realloc(Pointer, Size)
#define free(Pointer) _DBG_Free(Pointer)

#endif