#pragma once
#include "defines.h"

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Language
{
    enum E
    {
        RU,
        EN,
        Number
    } value;
    Language(uint v = 0) : value((E)v)
    {
    };
    operator uint8() const
    {
        return (uint8)value;
    };
};

struct Chan
{
    enum E
    {
        A,
        B,
        Number
    } value;
    Chan(E v) : value(v) {};
    operator uint8() const { return (uint8)value; };
    bool IsA() const { return value == A; };
};

/// ������������� ����� �����������
struct FreqResist
{
    enum E
    {
        _1MOhm,
        _50Ohm
    } value;
    bool Is(E v) const { return value == v; };
    FreqResist(E v) : value(v) {};
    FreqResist(uint v) : value((E)v) {};
    operator uint() const { return (uint)value; };
};

/// ��������/�������� ���� �����������
struct FreqCouple
{
    enum E
    {
        AC,
        DC
    } value;
    FreqCouple(E v) : value(v) {};
    FreqCouple(uint v) : value((E)v) {};
    bool Is(E v) const { return v == value; };
    operator uint() const { return (uint)value; };
};

/// ��� �����������
struct FreqFiltr
{
    enum E
    {
        Enable,
        Disable
    } value;
    FreqFiltr(E v) : value(v) {};
    FreqFiltr(uint v) : value((E)v) {};
    bool Is(E v) const { return v == value; };
    operator uint() const { return (uint)value; };
};

/// ����� �������
struct StartMode
{
    enum E
    {
        Auto,           ///< ��������������
        ComparatorA,    ///< ���������� ������ A (� ������ �������� �� ���� � ���� ������ AD9952 ������ A
        ShaperB,        ///< ������������� ��������� ������ B (����� "���������� ������")
        Single          ///< ����������� ������ �� ������� ������
    } value;

    StartMode(float v) : value((E)((int)(v + 0.5f))) {};
    operator uint8() const { return (uint8)value; };
};

#ifdef WIN32
#pragma warning(pop)
#endif
