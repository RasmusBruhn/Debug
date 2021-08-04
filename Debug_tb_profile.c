#include <stdio.h>

#define DBG_ACTIVE
#include "Debug.h"

uint64_t CountSquares(uint64_t MaxNum);
uint64_t SquareNum(uint64_t Num);
uint64_t GetNextNum(uint64_t PrevNum);

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
    
    // DBG_FLAGS_TIME | DBG_FLAGS_OWNTIME | DBG_FLAGS_TOTAL | DBG_FLAGS_AVG | DBG_FLAGS_STD | DBG_FLAGS_MIN | DBG_FLAGS_MAX | DBG_FLAGS_LIST;
    uint64_t Flags = DBG_FLAGS_TIME | DBG_FLAGS_OWNTIME | DBG_FLAGS_TOTAL | DBG_FLAGS_AVG | DBG_FLAGS_STD | DBG_FLAGS_MIN | DBG_FLAGS_MAX | DBG_SortFlag(DBG_FLAGS_AVG) | DBG_SortFlag(DBG_FLAGS_INVERT);

    uint64_t ErrorID = DBG_Init(ProfileLog, NULL, ErrorLog, Flags, 0);

    printf("Number = %lu\n", CountSquares(1000));

    // Close down
    DBG_Quit();

    fclose(ErrorLog);
    fclose(ProfileLog);

    printf("Done\n");

    return 0;
}

uint64_t CountSquares(uint64_t MaxNum)
{
    DBG_StartSession("CountSquares");

    uint64_t Total = 0;
    uint64_t Num = 0;

    while (Num <= MaxNum)
    {
        Total += SquareNum(Num);
        Num = GetNextNum(Num);
    }

    DBG_EndSession();

    return Total;
}

uint64_t SquareNum(uint64_t Num)
{
    DBG_StartSession("SquareNum");

    uint64_t Square = 0;
    uint64_t Prog = 0;

    while (Prog < Num)
    {
        Square += Num;
        Prog = GetNextNum(Prog);
    }
    
    DBG_EndSession();

    return Square;
}

uint64_t GetNextNum(uint64_t PrevNum)
{
    DBG_StartSession("GetNextNum");

    uint64_t NextNum = PrevNum + 1;

    DBG_EndSession();

    return NextNum;
}