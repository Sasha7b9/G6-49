#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Menu/Pages/Include/PageSignals.h"
#include "Display/Painter.h"
#include "FDrive/FDrivePanel.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageLoad;
Page *PageSignals::PageLoad::pointer = (Page *)&pageLoad;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_BUTTON( bUp,                                                                                                                              //--- ��������� �������� - �������� - ����� ---
    "�����", "����",
    "", "",
    pageLoad, Item::FuncActive, FDrive::PressUp, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON( bDown,                                                                                                                             //--- ��������� �������� - �������� - ���� ---
    "����", "Down",
    "", "",
    pageLoad, Item::FuncActive, FDrive::PressDown, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON( bEnter,                                                                                                                           //--- ��������� �������� - �������� - Enter ---
    "�����", "Enter",
    "", "",
    pageLoad, Item::FuncActive, FuncPress, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON( bChoose,                                                                                                                     //--- ��������� �������� - �������� - ���������� ---
    "�������", "Choose",
    "", "",
    pageLoad, Item::FuncActive, FDrive::PressChoose, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool OnKey(Control event)
{
    if (event.value == Control::RegLeft)
    {
        FDrive::PressDown();
        return true;
    }
    else if (event.value == Control::RegRight)
    {
        FDrive::PressUp();
        return true;
    }
    else
    {
        // ����� ������
    }

    return false;
}

DEF_PAGE_4( pageLoad, //-V641 //-V1027                                                                                                                //--- ��������� �������� - �������� ---
    "��������", "LOAD",
    "", "",
    //&bUp,       ///< ��������� �������� - �������� - �����
    //&bDown,     ///< ��������� �������� - �������� - ����
    //&bEnter,    ///< ��������� �������� - �������� - Enter
    &bChoose,   ///< ��������� �������� - �������� - ����������
    0,
    0,
    0,
    Page::Settings_Load, PageSignals::pointer, Item::FuncActive, FuncPress, OnKey, FDrive::Draw
)
