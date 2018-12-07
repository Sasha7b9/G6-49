#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "File.h"
#include "Utils/String.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_STRINGS 10
/// ����� �������� ������ �����
static String strings[NUM_STRINGS];
/// ������ ��� �����
static String name;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������� ������ �� ������ i-� ������ � �����
static void SendRequestForString(int i);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void File::Draw(int x, int y)
{
    for (int i = 0; i < NUM_STRINGS; i++)
    {
        if (strings[i].IsEmpty())
        {
            SendRequestForString(i);
        }
        else
        {
            strings[i].Draw(x, y + i * 10);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendRequestForString(int i)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void File::Open(char *fullName)
{
    Close();
    name.From(fullName);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void File::Close()
{
    name.Release();

    for (int i = 0; i < NUM_STRINGS; i++)
    {
        strings[i].Release();
    }
}