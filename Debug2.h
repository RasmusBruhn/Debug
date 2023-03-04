#ifndef DEBUG_DEFINED
#define DEBUG_DEFINED

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define ERR_PREFIX DBG
#include <Error2.h>
#define _DBG_ErrorSet(Format, ...) __DBG_ErrorSet(__FILE__, __LINE__ __VA_OPT__(, ) Format, __VA_ARGS__)
#define _DBG_ErrorAdd(Format, ...) __DBG_ErrorAdd(__FILE__, __LINE__ __VA_OPT__(, ) Format, __VA_ARGS__)
#define _DBG_ErrorAddExternal(ExternalMessage, Format, ...) __DBG_ErrorAddExternal(__FILE__, __LINE__, ExternalMessage, Format __VA_OPT__(, ) __VA_ARGS__)

#define _DBG_ERRORMES_MALLOC "Unable to allocate memory %lu"
#define _DBG_ERRORMES_LOCATEPOINTER "Unable to find pointer in list: %p"

typedef struct __DBG_MemoryBlock _DBG_MemoryBlock;

struct __DBG_MemoryBlock
{
    const void *addr;
    size_t size;
    const char *file;
    size_t line;
    _DBG_MemoryBlock *next;
};

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

// Adds a memory block to be kept track of
// File: The name of the file
// Line: The line number of this function call
// Pointer: The pointer to the memory block
void _DBG_MemoryAdd(const char *File, size_t Line, const void *Pointer, size_t Size);

// Removes a memory block from the list of traked memory
// Pointer: The pointer to the memory block
void _DBG_MemoryRemove(const void *Pointer);

// Writes a list of all still allocated memory to a file, cannot be run twice
// File: The file to write to
void DBG_MemoryLog(FILE *File);

// Prints a list of all still allocated memory, cannot be run twice
#define DBG_MemoryPrint() DBG_MemoryLog(stdout)

_DBG_MemoryBlock *_DBG_MemoryList = NULL;

void *_DBG_Malloc(const char *File, size_t Line, size_t Size)
{
    // Get memory
    void *Pointer = malloc(Size);

    // Add memory if success
    if (Pointer != NULL)
        _DBG_MemoryAdd(File, Line, Pointer, Size);

    return Pointer;
}

void *_DBG_Realloc(const char *File, size_t Line, void *Pointer, size_t Size)
{
    // Get memory
    void *NewPointer = realloc(Pointer, Size);

    // Remove old memory
    if (Pointer != NULL && (Size == 0 || NewPointer != NULL))
        _DBG_MemoryRemove(Pointer);

    // Add new memory
    if (NewPointer != NULL)
        _DBG_MemoryAdd(File, Line, NewPointer, Size);

    return NewPointer;
}

void _DBG_Free(void *Pointer)
{
    // Free memory
    free(Pointer);

    // Remove it from the memory list
    _DBG_MemoryRemove(Pointer);
}

void _DBG_MemoryAdd(const char *File, size_t Line, const void *Pointer, size_t Size)
{
    extern _DBG_MemoryBlock *_DBG_MemoryList;

    // Create memory for data
    _DBG_MemoryBlock *NewMemory = (_DBG_MemoryBlock *)malloc(sizeof(_DBG_MemoryBlock));

    if (NewMemory == NULL)
    {
        _DBG_ErrorAddExternal(strerror(errno), _DBG_ERRORMES_MALLOC, sizeof(_DBG_MemoryBlock));
        return;
    }

    // Fill values
    NewMemory->addr = Pointer;
    NewMemory->file = File;
    NewMemory->line = Line;
    NewMemory->size = Size;
    NewMemory->next = _DBG_MemoryList;

    // Add to the list
    _DBG_MemoryList = NewMemory;
}

void _DBG_MemoryRemove(const void *Pointer)
{
    extern _DBG_MemoryBlock *_DBG_MemoryList;

    // Find in the list
    bool Success = false;

    for (_DBG_MemoryBlock *MemoryList = _DBG_MemoryList, *Prev = NULL; MemoryList != NULL; Prev = MemoryList, MemoryList = MemoryList->next)
        if (MemoryList->addr == Pointer)
        {
            if (Prev != NULL)
                Prev->next = MemoryList->next;

            else
                _DBG_MemoryList = MemoryList->next;

            free(MemoryList);
            Success = true;
            break;
        }

    if (!Success)
        _DBG_ErrorSet(_DBG_ERRORMES_LOCATEPOINTER, Pointer);
}

void DBG_MemoryLog(FILE *File)
{
    extern _DBG_MemoryBlock *_DBG_MemoryList;

    // Go through each element and print it
    while (_DBG_MemoryList != NULL)
    {
        // Print it
        fprintf(File, "%p, size: %lu, file: \"%s\", line: %lu\n", _DBG_MemoryList->addr, _DBG_MemoryList->size, _DBG_MemoryList->file, _DBG_MemoryList->line);

        // Free memory
        _DBG_MemoryBlock *TempList = _DBG_MemoryList->next;
        free(_DBG_MemoryList);
        _DBG_MemoryList = TempList;
    }
}

// Overwrite standard functions
#define malloc(Size) _DBG_Malloc(__FILE__, __LINE__, Size)
#define realloc(Pointer, Size) _DBG_Realloc(__FILE__, __LINE__, Pointer, Size)
#define free(Pointer) _DBG_Free(Pointer)

#endif