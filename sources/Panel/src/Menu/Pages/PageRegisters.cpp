#include "PageRegisters.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Wave.h"
#include "Menu/Menu.h"
#include "Command.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pRegisters;
Page *PageRegisters::pointer = (Page *)&pRegisters;
Name_Register PageRegisters::currentRegister = FreqMeterLevel;
bool showInputWindow = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageRegisters::Draw()
{
    if(Menu::CurrentPage() != pointer)
    {
        return;
    }

    Painter::FillRegion(Wave::X(), Wave::Y(A) + 1, Wave::Width() - 1, Wave::Height() * 2, Color::BACK);

    DrawRegisters(Wave::X() + 4, Wave::Y(A) + 3);

    DrawInputWindow(Wave::X() + 4, Wave::Y(B));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageRegisters::DrawRegisters(int x, int y)
{
    for(uint8 i = 0; i < NumRegisters; i++)
    {
        Register reg(i);
        Color color = Color::FILL;
        if(i == currentRegister)
        {
            Painter::FillRegion(x - 1, y + i * 10, 127, 8, Color::FILL);
            color = Color::BACK;
        }
        Text::DrawText(x, y + i * 10, reg.Name(), color);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageRegisters::DrawInputWindow(int x, int y)
{
    if(!showInputWindow)
    {
        return;
    }

    Painter::FillRegion(x, y, 200, 50, Color::BLUE);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Prev()
{
    CircleDecrease<uint8>((uint8 *)&PageRegisters::currentRegister, 0, NumRegisters - 1);
}

DEF_BUTTON( bPrev,                                                                                                     //--- �������� - ���������� ---
    "����������", "Previous",
    "������� � ����������� ��������",
    "Go to the previous register",
    pRegisters, FuncActive, OnPress_Prev, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Next()
{
    CircleIncrease<uint8>((uint8 *)&PageRegisters::currentRegister, 0, NumRegisters - 1);
}

DEF_BUTTON(bNext,                                                                                                       //--- �������� - ��������� ---
    "���������", "Next",
    "������� � ���������� ��������",
    "Go to the next register",
    pRegisters, FuncActive, OnPress_Next, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Send()
{
}

DEF_BUTTON(bSend,                                                                                                         //--- �������� - ������� ---
    "�������", "Send",
    "��������� ���� ����� �������� ��������",
    "Opens the register value entry window",
    pRegisters, FuncActive, OnPress_Send, FuncDraw
)

static Item emptyItem = {Item_None};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool OnKey(StructControl strCntrl)
{
    if(KeyIsDigit(strCntrl.key) && !showInputWindow)
    {
        showInputWindow = true;
    }

    return false;
}

DEF_PAGE_4(pRegisters,                                                                                                              //--- �������� ---
    "��������", "REGISTERS",
    "",
    "",
    bPrev,
    bNext,
    bSend,
    emptyItem,
    Page_Registers, 0, FuncActive, FuncPress, OnKey
)
