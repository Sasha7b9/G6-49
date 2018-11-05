#pragma once
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interface
{
public:

    static void Init();
    /// ������� � ������ ���������� �������� �������
    static void SendFrequency(uint frequency);

    static void ReceiveCallback();

    static void Empty();

    static void EnableChannel();

    static void SetFormWave();

    static void ParameterValue();

    static void RunReset();

    static void ModeDebug();

    static void WriteRegister();
    /// ������� ������������ ������� ������������� �������
    static void CreateWave();

    static void SetManipulation();

    static void SetManipulationMode();

    static void SetStartMode();

    static void LoadFormDDS();

    static void SetPolarity();

private:
    /// ���������� ������ �������� ������ ������ size
    static void ResizeRecieveBuffer(uint16 size);

    static uint timeLastReceive;
};