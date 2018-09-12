#pragma once
#include "Hardware/Controls.h"
#include "Command.h"
#include "Menu/MenuItems.h"
#include <string.h>


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_DIGITS          5

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *NameUnit(char buffer[10], Order order, Parameter parameter);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���� �������������� ���������� �������� ���������
class InputWindow
{
public:
    static void Init();

    static void Draw();

    static void KeyLeft();

    static void KeyRight();

    static void KeyEnter();

    static void ProcessContorl(Control control);

private:
    static void DrawAmplitude(int x, int y);

    static void DrawFrequency(int x, int y);

    static void DrawOffset(int x, int y);

    static void DrawPhase(int x, int y);

    static void DrawDelay(int x, int y);

    static void DrawPeriod(int x, int y);

    static void DrawDutyRatio(int x, int y);

    static void DrawDuration(int x, int y);

    static void DrawDigits(int x, int y);

    static void DrawDepthModulation(int x, int y);

    static void DrawPolarity(int x, int y);

    static void DrawDurationRise(int x, int y);

    static void DrawDurationFall(int x, int y);

    static void DrawDurationStady(int x, int y);

    static void DrawDutyFactor(int x, int y);
    /// �����, ��� �������� �������� ��������
    static Chan         chan;
    /// ��������, �������� �������� ��������
    static Parameter    *param;

public:
    class Struct
    {
    public:

        void Fill(Chan ch, Form *form, Parameter *param);

        char *StringValue();

        float Value();

        void DrawInputField(int x, int y);

        void KeyLeft();

        void KeyRight();

        void RegLeft();

        void RegRight();

        void PressKey(Control key);

        void SaveValue();

        static Form        *form;
        static Parameter   *param;
        static Chan        ch;

        Struct(char s, char *buff = "10000", int8 comma = 0, int8 hd = NUM_DIGITS - 1) :
            sign(s), posComma(comma), hightLightDigit(hd)
        {
            strcpy(buffer, buff);
        }
        char     sign;
        char     buffer[NUM_DIGITS + 1];    ///< � ���� ������ �������� ���� �����, � ����� ����� � ������
        int8     posComma;            ///< ����� ������ ���������� �������� �������
        int8     hightLightDigit;     ///< ������� ������������� ����������
        //uint8    order       : 3;

    private:
        /// ����������� �������� � �������� �������. ���������� false, ���� �������� �������� �� ����������.
        bool IncreaseDigit(int num);
        /// ��������� �������� � �������� �������. ���������� false, ���� �������� �������� �� ����������
        bool DecreaseDigit(int num);
        /// ���������� true, ���� ��� ����� ����� �� num ����� ����. � num ����
        bool All0LeftWithThis(int num);
        /// ���������� true, ���� ��� ����� ����� � ��� �������� ���������
        bool All9LeftWithThis(int num);
        /// �������� ��� ������� ������
        void ShiftToRight();
        /// ����� ���� �������� �����
        void ShiftToLeft();
        /// ���������� true, ���� ���� ������ ���� ������� ������ �����
        bool OnlyOneRigthDigit();
        /// ���������� ����� �� �������
        int ValueBeforeComma();
        /// ���������� ����� ����� �������
        float ValueAfterComma();
        /// ����������� ������� �� ��������� �� �����������
        void IncreaseOrder();
        /// ��������� iws �� inputBuffer
        void FillFromInputBuffer();
        /// ������� ������� �������� � ���������
        void SendToGenerator();
    };

    static Struct m_iws;
};


#ifdef WIN32
#pragma warning(pop)
#endif
