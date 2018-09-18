#include "Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Control::IsDigit() const
{
    return (value >= _0 && value <= _9);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char Control::ToChar() const
{
    if(IsDigit())
    {
        return (char)((value - _0) | 0x30);
    }
    else if(value == Dot)
    {
        return '.';
    }

    return '\0';
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsFunctional() const
{
    return value >= F1 && value <= F4;
}
