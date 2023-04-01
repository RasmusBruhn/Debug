#ifndef DEBUG_DEFINED2
#define DEBUG_DEFINED2

#include <stdio.h>
#include <stdlib.h>
#include "Debug2.h"

#define ERR_PREFIX DBG
#include "Error2.c"
#define _DBG_ErrorSet(Format, ...) __DBG_ErrorSet(__FILE__, __LINE__ __VA_OPT__(, ) Format, __VA_ARGS__)
#define _DBG_ErrorAdd(Format, ...) __DBG_ErrorAdd(__FILE__, __LINE__ __VA_OPT__(, ) Format, __VA_ARGS__)
#define _DBG_ErrorAddExternal(ExternalMessage, Format, ...) __DBG_ErrorAddExternal(__FILE__, __LINE__, ExternalMessage, Format __VA_OPT__(, ) __VA_ARGS__)

#ifdef __cplusplus
extern "C"
{
#endif

#define _DBG_ERRORMES_MALLOC "Unable to allocate memory (size: %lu)"
#define _DBG_ERRORMES_REALLOC "Unable to reallocate memory (size: %lu)"
#define _DBG_ERRORMES_LOCATEPOINTER "Unable to find pointer in list: %p"

// The number of bytes to pad memory blocks with to check for overflows
#define _DBG_CHECKVALUESIZE 8

typedef struct __DBG_MemoryBlock _DBG_MemoryBlock;
typedef struct __DBG_OverflowBlock _DBG_OverflowBlock;
typedef enum __DBG_OverflowType _DBG_OverflowType;

enum __DBG_OverflowType
{
    _DBG_OVERFLOW_NONE,
    _DBG_OVERFLOW_END,
    _DBG_OVERFLOW_START
};

struct __DBG_MemoryBlock
{
    const void *addr;
    const char *file;
    _DBG_MemoryBlock *next;
    size_t size;
    size_t line;
    uint8_t checkValue;
};

struct __DBG_OverflowBlock
{
    const void *addr;
    const char *file;
    size_t size;
    size_t line;
    _DBG_OverflowType type;
    uint8_t value[_DBG_CHECKVALUESIZE];
    uint8_t checkValue;
};

// Adds a memory block to be kept track of
// File: The name of the file
// Line: The line number of this function call
// Pointer: The pointer to the memory block
void _DBG_MemoryAdd(const char *File, size_t Line, void *Pointer, size_t Size);

// Removes a memory block from the list of traked memory
// Pointer: The pointer to the memory block
void _DBG_MemoryRemove(const void *Pointer);

#ifdef __cplusplus
}
#endif

#endif