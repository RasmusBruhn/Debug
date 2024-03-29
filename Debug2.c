#ifndef DEBUG_DEFINED3
#define DEBUG_DEFINED3

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <Defines.h>
#include "_Debug2.h"

#ifdef __cplusplus
extern "C"
{
#endif

// List of all allocated memory
_DBG_MemoryBlock *_DBG_MemoryList = NULL;

// List of all overflow errors
_DBG_OverflowBlock *_DBG_OverflowList = NULL;
size_t _DBG_OverflowLength = 0;

void *_DBG_Malloc(const char *File, size_t Line, size_t Size)
{
    // If size is 0
    if (Size == 0)
        return malloc(0);

    // Get memory
    void *Pointer = malloc(Size + 2 * _DBG_CHECKVALUESIZE);

    // Add memory if success
    if (Pointer != NULL)
        _DBG_MemoryAdd(File, Line, Pointer + _DBG_CHECKVALUESIZE, Size);

    return Pointer + _DBG_CHECKVALUESIZE;
}

void *_DBG_Realloc(const char *File, size_t Line, void *Pointer, size_t Size)
{
    if (Pointer != NULL)
        _DBG_MemoryRemove(Pointer);

    // If Size is 0
    if (Size == 0)
        return realloc(Pointer - _DBG_CHECKVALUESIZE, 0);

    // Get memory
    void *OriginPointer = ((Pointer == NULL) ? (NULL) : (Pointer - _DBG_CHECKVALUESIZE));
    void *NewPointer = realloc(OriginPointer, Size + 2 * _DBG_CHECKVALUESIZE);

    // Add new memory
    if (NewPointer != NULL)
        _DBG_MemoryAdd(File, Line, NewPointer + _DBG_CHECKVALUESIZE, Size);

    return NewPointer + _DBG_CHECKVALUESIZE;
}

void _DBG_Free(void *Pointer)
{
    // Remove it from the memory list
    _DBG_MemoryRemove(Pointer);

    // Free memory
    free(Pointer - _DBG_CHECKVALUESIZE);
}

void _DBG_MemoryAdd(const char *File, size_t Line, void *Pointer, size_t Size)
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
    NewMemory->checkValue = (uint8_t)clock();

    // Add the under- and overflow check values
    memset(Pointer - _DBG_CHECKVALUESIZE, NewMemory->checkValue, _DBG_CHECKVALUESIZE);
    memset(Pointer + Size, NewMemory->checkValue, _DBG_CHECKVALUESIZE);

    // Add to the list
    _DBG_MemoryList = NewMemory;
}

void _DBG_MemoryRemove(const void *Pointer)
{
    extern _DBG_MemoryBlock *_DBG_MemoryList;
    extern _DBG_OverflowBlock *_DBG_OverflowList;
    extern size_t _DBG_OverflowLength;

    // Find in the list
    _DBG_MemoryBlock *Block = NULL;

    for (_DBG_MemoryBlock *MemoryList = _DBG_MemoryList, *Prev = NULL; MemoryList != NULL; Prev = MemoryList, MemoryList = MemoryList->next)
        if (MemoryList->addr == Pointer)
        {
            if (Prev != NULL)
                Prev->next = MemoryList->next;

            else
                _DBG_MemoryList = MemoryList->next;

            Block = MemoryList;
            break;
        }

    // Make sure it found it
    if (Block == NULL)
    {
        _DBG_ErrorSet(_DBG_ERRORMES_LOCATEPOINTER, Pointer);
        return;
    }

    _DBG_OverflowType ErrorType = _DBG_OVERFLOW_NONE;

    // Check for underflow
    for (uint8_t *Mem = (uint8_t *)Block->addr - _DBG_CHECKVALUESIZE, *EndMem = (uint8_t *)Block->addr; Mem < EndMem; ++Mem)
        if (*Mem != Block->checkValue)
        {
            ErrorType = _DBG_OVERFLOW_START;
            break;
        }

    // Check for overflow
    for (uint8_t *Mem = (uint8_t *)Block->addr + Block->size, *EndMem = (uint8_t *)Block->addr + Block->size + _DBG_CHECKVALUESIZE; Mem < EndMem; ++Mem)
        if (*Mem != Block->checkValue)
        {
            ErrorType = _DBG_OVERFLOW_END;
            break;
        }

    // Add the overflow to the overflow list
    if (ErrorType != _DBG_OVERFLOW_NONE)
    {
        _DBG_OverflowBlock *NewOverflowList = (_DBG_OverflowBlock *)realloc(_DBG_OverflowList, sizeof(_DBG_OverflowBlock) * (_DBG_OverflowLength + 1));

        if (NewOverflowList == NULL)
        {
            _DBG_ErrorAddExternal(strerror(errno), _DBG_ERRORMES_REALLOC, sizeof(_DBG_OverflowBlock) * (_DBG_OverflowLength + 1));
            free(Block);
            return;
        }

        _DBG_OverflowList = NewOverflowList;

        // Set the values
        _DBG_OverflowList[_DBG_OverflowLength].addr = Block->addr;
        _DBG_OverflowList[_DBG_OverflowLength].file = Block->file;
        _DBG_OverflowList[_DBG_OverflowLength].line = Block->line;
        _DBG_OverflowList[_DBG_OverflowLength].size = Block->size;
        _DBG_OverflowList[_DBG_OverflowLength].type = ErrorType;
        _DBG_OverflowList[_DBG_OverflowLength].checkValue = Block->checkValue;

        switch (ErrorType)
        {
            case (_DBG_OVERFLOW_START):
                memcpy(_DBG_OverflowList[_DBG_OverflowLength].value, Block->addr - _DBG_CHECKVALUESIZE, _DBG_CHECKVALUESIZE);
                break;

            case (_DBG_OVERFLOW_END):
                memcpy(_DBG_OverflowList[_DBG_OverflowLength].value, Block->addr + Block->size, _DBG_CHECKVALUESIZE);
                break;

            default:
                break;
        }

        // Increment the length
        ++_DBG_OverflowLength;
    }

    // Free memory
    free(Block);
}

void DBG_MemoryLog(FILE *File)
{
    extern _DBG_MemoryBlock *_DBG_MemoryList;
    extern _DBG_OverflowBlock *_DBG_OverflowList;
    extern size_t _DBG_OverflowLength;

    // Go through each memory leak and print it
    fprintf(File, "Memory Leaks:\n");

    while (_DBG_MemoryList != NULL)
    {
        // Print it
        fprintf(File, "%p, size: " PRINT_UINT64 ", file: \"%s\", line: " PRINT_UINT64 "\n", _DBG_MemoryList->addr, _DBG_MemoryList->size, _DBG_MemoryList->file, _DBG_MemoryList->line);

        // Free memory
        _DBG_MemoryBlock *TempList = _DBG_MemoryList->next;
        free(_DBG_MemoryList);
        _DBG_MemoryList = TempList;
    }

    // Go through each over/underflow and print it
    fprintf(File, "\nOverflows:\n");

    for (_DBG_OverflowBlock *List = _DBG_OverflowList, *EndList = _DBG_OverflowList + _DBG_OverflowLength; List < EndList; ++List)
    {
        // Get type
        const char *Type = ((List->type == _DBG_OVERFLOW_START) ? ("underflow") : ("overflow"));

        // Print
        fprintf(File, "%p, size: " PRINT_UINT64 ", file: \"%s\", line: " PRINT_UINT64 ", type: %s, value: ", List->addr, List->size, List->file, List->line, Type);

        for (uint8_t *Value = List->value + _DBG_CHECKVALUESIZE - 1, *StartValue = (uint8_t *)List->value; Value >= StartValue; --Value)
            fprintf(File, "%02X", *Value);

        fprintf(File, " (%02X)\n", List->checkValue);
    }

    free(_DBG_OverflowList);
    _DBG_OverflowList = NULL;
    _DBG_OverflowLength = 0;
}

#ifdef __cplusplus
}
#endif

#endif