#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "RawData.h"
#include <cstdlib>
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawData::RawData(uint size) : buffer(0), allocated(0), used(0)
{
    Alloc(size);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RawData::RawData(uint size, uint8 value0, uint8 value1) : buffer(0), allocated(0), used(0)
{
    Alloc(size);
    
    Put(value0);
    Put(value1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RawData::RawData(uint size, uint8 value0, uint8 value1, uint8 value2, uint8 value3) : buffer(0), allocated(0), used(0)
{
    Alloc(size);

    Put(value0);
    Put(value1);
    Put(value2);
    Put(value3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RawData::Alloc(uint size)
{
    std::free(buffer);
    allocated = 0;
    used = 0;
    buffer = (uint8 *)std::malloc(size);
    if (buffer)
    {
        allocated = size;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RawData::~RawData()
{
    std::free(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RawData::Put(uint8 value)
{
    buffer[used++] = value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RawData::Put(float value)
{
    std::memcpy(buffer + used, &value, 4);
    used += 4;
}
