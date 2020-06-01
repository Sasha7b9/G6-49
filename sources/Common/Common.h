#pragma once
#include "defines.h"


struct TypeForm
{
    enum E
    {
        Sine,           // �����
        RampPlus,       // ����+
        RampMinus,      // ����-
        Triangle,       // �����������
        Meander,        // ������
        Impulse,        // ��������
        PacketImpuls,   // ����� ���������
        Free,           // ������������ ������
        Count
    };
};


struct Order
{
    enum E
    {
        Mega,
        Kilo,
        One,
        Milli,
        Micro,
        Nano,
        Count
    };

    static pString Suffix(Order::E order);

    // ���������� �������, �������������� �������
    static int GetPow10(Order::E order);
};


struct Value
{
    explicit Value(double v);

    // ���� �������� �� ���������� �������������. ��� ���� ������ �������� ����� ���� ���� ("+" ��� "-"), ������� ����� ���������� �� ����� ������ ("."),
    // � order ��������, �� ������� ����� ��������� �������� ����� (3 - �������� �� 1000, -3 - ��������� �� 1000)
    explicit Value(const char *const buffer, int order = 0);

    void FromUnits(int units, uint mUnits, uint uUnits, uint nUnits, int sign);
    void FromDouble(double v);
    void FromUINT64(uint64 v) { value = v; }
    void FromString(const char * const buffer, int order);

    double ToDouble() const;
    uint64 ToUINT64() const { return value; }

    void Div(uint div);
    void Mul(uint mul);
    void Add(Value value);
    void Sub(Value value);
    // �������� �� 10 � ������� pow
    void MulPow10(int pow);

    void SetSign(int sign);

    // ���������� ����
    int Sign() const;

    // ���������� ����� �����
    int Integer() const;
      
    // ���������� ���������� ���������� � ������� �����
    int FractNano() const;

    uint64 Abs() const;

    // ����������:
    // Order::Mega  - Value::Integer() >= 1e6
    // Order::Kilo  - Value::Integer() >= 1e3
    // Order::One   - Value::Integer() >= 0
    // Order::Milli - Value::Integer() >= 1e-3
    // Order::Micro - Value::Integer() >= 1e-6
    Order::E GetOrder() const;

    // ��������� ������ ��������, ���������� � ������� order
    pString ToString(Order::E order, bool sign) const;

    bool operator<(const Value &);
    bool operator>(const Value &);
    bool operator<=(const Value &);
    bool operator>=(const Value &);
    bool operator==(const Value &);
    bool operator!=(const Value &);

private:

    uint64 value;       // �������� ��������� � �������� ��������� "����". ������������� � "1" ������� ��� ��������, ��� ����� �������������
};
