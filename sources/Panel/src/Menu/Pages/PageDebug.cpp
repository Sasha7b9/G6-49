#include "PageDebug.h"
#include "Generator/Generator.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Display/font/Font.h"
#include "PageService.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pDebug;
Page *PageDebug::pointer = (Page *)&pDebug;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_DebugMode(bool)
{
    Generator::SetDebugMode(DEBUG_MODE_ENABLED);
}

DEF_CHOICE_2(cConsole,                                                                                                     //--- ������� - ������� ---
    "�������", "CONSOLE",
    "�������� � ��������� ����������� ���������� �������",
    "Turns on or off the display of the debug console",
    DISABLED_RU, DISABLED_EN, "����������� ������� ��������",
                              "Console display enabled",
    ENABLED_RU,  ENABLED_EN,  "����������� ������� ���������",
                              "Console display disabled",
    FLAG_1, BIT_CONSOLE, pDebug, FuncActive, OnPress_DebugMode, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SaveSettings()
{
    Settings::Save();
}

DEF_BUTTON( bSaveSettings,                                                                                     //--- ������� - ��������� ��������� ---
    "��������� ���������", "SAVE SETTINGS",
    "��������� ������� ���������",
    "Save current settings",
    pDebug, FuncActive, OnPress_SaveSettings, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cStatistics,                                                                                              //--- ������� - ���������� ---
    "����������", "STATISTICS",
    "��������/��������� ����� �� ������ ���������� �� ������� ������ � ���� ��� / ����� ��������� ������ ����� / �����, ���������� �� ��������� �� "
    "���� ����",
    "Enables / disables the display of statistics on the frame rate in the form of FPS / time of drawing one frame / time spent on rendering for the "
    "whole frame",
    DISABLED_RU, DISABLED_EN, "����� ���������� �������",
                              "Statistics show on",
    ENABLED_RU, ENABLED_EN,   "����� ���������� ��������",
                              "Statistics showing off",
    FLAG_1, BIT_STATISTICS, pDebug, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_BigSymbols(bool)
{
    Font::ToggleCharacterSize();
}

DEF_CHOICE_2 (  cBigSymbols,                                                                                       //--- ������� - ������� ������� ---
    "������� �������", "Large characters",
    "����������� ���������� ����� �������� ���������",
    "Display information with larger characters",
    DISABLED_RU, DISABLED_EN,   "����������� ���������� ������������ ���������",
                                "Displaying information with standard symbols",
    ENABLED_RU, ENABLED_EN,     "����������� ���������� ������������ ���������",
                                "Displaying information with enlarged symbols",
    FLAG_1, BIT_BIG_SYMBOLS, pDebug, FuncActive, OnPress_BigSymbols, FuncDraw
)


DEF_PAGE_4( pDebug,
    "�������", "DEBUG",
    "", "",
    cConsole,       ///< ������� - �������
    bSaveSettings,  ///< ������� - ��������� ���������
    cStatistics,    ///< ������� - ����������
    cBigSymbols,    ///< ������� - ������� �������
    Page::Debug, PageService::pointer, FuncActive, FuncPress, FuncOnKey
)
