#pragma once
#include "MenuItems.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_PAGES           4   ///< ���������� ������� � ����


extern const Page *menu[NUM_PAGES];

class Menu
{
public:
        
    static void Init();

    static void Update();

    static void Draw();
    /// ���������� true, ���� ����� ��������� �������������� ������� ����
    static bool RegIsControlPages();

    static char *NameCurrentParameter();

private:

    static void DrawTitle();

    static void DrawPagesUGO();

    static void ProcessControl(StructControl control);
};
