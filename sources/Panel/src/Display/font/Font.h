#pragma once
#include "defines.h"
#include "Display/DisplayTypes.h"


struct Symbol
{
    uchar width;
    uchar bytes[8];
};


struct TypeFont
{
    enum E
    {
        _5,
        _7,
        _8,
        Count,
        None
    };
};


struct Font
{
    static int GetSize();
    static int GetLengthText(const char *text);
    static int GetHeightSymbol(char symbol);
    static int GetLengthSymbol(char symbol);
    static void SetType(TypeFont::E typeFont);
    static void ToggleCharacterSize();
    static int8 Width(char symbol);
    static int8 Width(uint8 symbol);
    static int8 Height();
    static bool ByteNotEmpty(int s, int byte);
    static bool BitIsExist(int s, int byte, int bit);
    
    int height;
    Symbol symbol[256];
};
