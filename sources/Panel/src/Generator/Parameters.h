#pragma once
#include "common/Common.h"
#include "Settings/SettingsTypes.h"


class Form;


struct Order
{
    enum E
    {
        Mega,
        Kilo,
        One,
        Milli,
        Micro,
        Nano
    };
};


class Parameter
{
public:

    enum E
    {
        Value,
        Exit,
        Choice,
        Complex,
        Page
    };

    Parameter(E k, const char *n) : kind(k), name(n) { }

    pString Name() const { return name; }

    virtual void SetForm(Form *form);

    void SetParent(Parameter *p) { parent = p; }
    /// ���������� ����� ������������� ���������
    Parameter *GetParent() { return parent; }

    bool IsValue() const
    {
        return (kind == Value);
    }

    bool IsComplex() const
    {
        return (kind == Complex);
    }

    bool IsChoice() const
    {
        return (kind == Choice);
    }
    /// ���������� true, ���� �������� ������� � ������
    bool IsOpened() const { return parent != 0; }

    virtual pString GetStringValue() const = 0;

    Form *GetForm() { return form; }

    pString NameUnit(char buffer[10]) const;

protected:
    /// �����, ��� ������� ���� ���� ��������
    Form *form;
    /// ���� �������� ���������, �� ����� ����� ��������
    Parameter *parent;

    E kind;

    const char *name;
};


class ParameterValue : public Parameter
{
public:

    enum E
    {
        Frequency,              ///< �������
        Period,                 ///< ������
        Amplitude,              ///< ���������
        Offset,                 ///< ��������
        Duration,               ///< ������������
        DutyRatio,              ///< ����������
        Phase,                  ///< ����� ����
        Delay,                  ///< ��������
        DurationRise,           ///< ������������ ����������
        DurationFall,           ///< ������������ �����
        DurationStady,          ///< ������������ ������������� ��������
        DutyFactor,             ///< ����������� ����������
        ManipulationDuration,   ///< ������������ ��������� � ������ �����������
        ManipulationPeriod,     ///< ������ ���������� ��������� � ������ �����������
        PacketPeriod,           ///< ������ ���������� ����� ��������� � �������� ������
        PacketNumber,           ///< ����� ��������� � ����� ��������� ������
        Exit,                   ///< ���� �������� ��������� �������� ��������
        Count
    };

    ParameterValue(E t, const char *name, const FloatValue &_min, const FloatValue &_max, const FloatValue &_value) :
        Parameter(Parameter::Value, name), type(t), min(_min), max(_max), value(_value) { }

    E Type() const { return type; }

    FloatValue GetValue() const { return value; };

    pString GetStringValue(Language::E lang) const;

    virtual pString GetStringValue() const;

    bool SetAndLoadValue(float val);

private:
    E type;

    FloatValue min;
    FloatValue max;
    FloatValue value;

    pString MainUnits(Language::E lang) const;
    /// ���������� true, ���� �������� ����� ��������� �������� v
    bool InRange(float v) const;


};


class ParameterVoltage : public ParameterValue
{
public:
    ParameterVoltage(ParameterValue::E type, const char *name, const FloatValue &min, const FloatValue &max, const FloatValue &value) :  ParameterValue(type, name, min, max, value) { }
};


class ParameterAmplitude : public ParameterVoltage
{
public:
    ParameterAmplitude(const FloatValue &min = FloatValue(0, 0), const FloatValue &max = FloatValue(10, 0), const FloatValue &value = FloatValue(10, 0)) :
        ParameterVoltage(ParameterValue::Amplitude, "������", min, max, value) { }
};


class ParameterOffset : public ParameterVoltage
{
public:
    ParameterOffset(const FloatValue &min = FloatValue(-5, 0), const FloatValue &max = FloatValue(5, 0), const FloatValue &value = FloatValue(0, 0)) :
        ParameterVoltage(ParameterValue::Offset, "��������", min, max, value) { }
};


class ParameterFrequency : public ParameterValue
{
public:
    ParameterFrequency(const FloatValue &min = FloatValue(0, 100), const FloatValue &max = FloatValue(100 * 1000 * 1000, 0), const FloatValue &value = FloatValue(1000, 0)) :
        ParameterValue(ParameterValue::Frequency, "�������", min, max, value) { }
};


class ParameterTime : public ParameterValue
{
public:
    ParameterTime(ParameterValue::E t, const char *name, const FloatValue &min, const FloatValue &max, const FloatValue &value) :  ParameterValue(t, name, min, max, value) { }
};


class ParameterPhase : public ParameterValue
{
public:
    ParameterPhase() : ParameterValue(ParameterValue::Phase, "����", FloatValue(0, 0), FloatValue(360, 0), FloatValue(0, 0)) { }
};


class ParameterPacketPeriod : public ParameterTime
{
public:
    ParameterPacketPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::PacketPeriod, "������", min, max, value) { }
};


class ParameterPeriod : public ParameterTime
{
public:
    ParameterPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::Period, "������", min, max, value) { }
};


class ParameterDuration : public ParameterTime
{
public:
    ParameterDuration(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::Duration, "������������", min, max, value) { }
};


class ParameterManipulationDuration : public ParameterTime
{
public:
    ParameterManipulationDuration(const FloatValue &min, const FloatValue &max, const FloatValue &value) :
        ParameterTime(ParameterValue::ManipulationDuration, "������������", min, max, value) { }
};


class ParameterManipulationPeriod : public ParameterTime
{
public:
    ParameterManipulationPeriod(const FloatValue &min, const FloatValue &max, const FloatValue &value) : ParameterTime(ParameterValue::ManipulationPeriod, "������", min, max, value) { }
};


class ParameterChoice : public Parameter
{
public:
	enum E
	{
        Polarity,                   ///< ���������� ���������
        ModeStart,                  ///< ����� ������� �������� DDS � ����������
        deleted_ManipulationMode,   ///< ����� ����������� ����������� - �� ������������ ������ ��� ���
        ManipulationEnabled,        ///< ������� ��� �������� ����� �����������
        Count
	};

    ParameterChoice(E t, const char *name) : Parameter(Parameter::Choice, name), type(t) { }

	E Type() { return type; }

    int GetChoice() const;

    void NextChoice();

    bool SetAndLoadChoice(int ch);

    bool DrawChoice(int x, int y) const;

    virtual pString GetStringValue() const;

private:
	E type;
    /// ������� �����
    int choice;

    const char **names;
    /// ���������� ��������� ������
    int NumChoices() const;
};


class ParameterModeStart : public ParameterChoice
{
public:
    //ParameterModeStart(pString choice0 = " ����", pString choice1 = " ������", pString choice2 = " ���� �", pString choice3 = " ���� B");

    ParameterModeStart() : ParameterChoice(ParameterChoice::ModeStart, "������") { }
};


class ParameterManipulationEnabled : public ParameterChoice
{
public:
    ParameterManipulationEnabled(pString, pString) : ParameterChoice(ParameterChoice::ManipulationEnabled, "�����������") { }
};


class ParameterPolarity : public ParameterChoice
{
public:
    ParameterPolarity() : ParameterChoice(ParameterChoice::Polarity, "����������") { }
};


class ParameterComplex : public Parameter
{
public:
    enum E
    {
        Manipulation,       ///< ��������� �������� / �������� / ����������� �� ����� �����
        Count
    } value;

    ParameterComplex(E v, const char *name, Parameter **parameters) : Parameter(Parameter::Complex, name), params(parameters), type(v) { }

    virtual void SetForm(Form *form);

    int NumParams() const { return numParams; }
    Parameter **Params() { return params; }

    ParameterValue *FindParameter(ParameterValue::E p);
    ParameterChoice *FindParameter(ParameterChoice::E p);

    virtual pString GetStringValue() const;
private:
    /// ����� ��������� �������������� ��������� � ������, ���� ��� ���������
    Parameter **params;
    /// ����� �������������� ���������� ��� 0, ���� ������� �� �������
    int numParams;

    E type;
};


class ParameterManipulation : public ParameterComplex
{
public:
    ParameterManipulation(Parameter **paramaters);
};
