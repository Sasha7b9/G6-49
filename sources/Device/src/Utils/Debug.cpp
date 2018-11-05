#include "stdafx.h"
#ifndef WIN32
#include "Debug.h"
#include "Log.h"
#include "Hardware/Timer.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint  Debug::timeStart = 0;
uint  Debug::timeCounter = 0;
uint  Debug::timeStartCounter = 0;
char *Debug::file = 0;
int   Debug::line = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Debug::StartProfiling()
{
    timeStart = TIME_US;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::PointProfiling(char *)
{
    //LOG_WRITE("%s %d", name, TIME_US - timeStart);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::StartIncreaseCounter()
{
    timeStartCounter = TIME_US;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Debug::StopIncreaseCounter()
{
    timeCounter += (TIME_US - timeStartCounter);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}
