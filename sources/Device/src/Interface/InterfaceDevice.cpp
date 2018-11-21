#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Message.h"
#include "InterfaceDevice.h"
#include "FDrive/FDriveDevice.h"
#include "Utils/Console.h"
#include "Generator/GeneratorDevice.h"
#include "Generator/FPGA.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Modules/SPI.h"
#include "FreqMeter/FreqMeter.h"
#include "Settings/Settings.h"
#include "Utils/Array.h"
#include "Utils/Debug.h"
#include "Utils/StringUtils.h"
#include "Command.h"
#include "structs.h"
#include <string.h>
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��������� �������� ��������, ��� ��� ������� �������� � ������ ��� ���������� ������������ ��������
volatile static uint freqForSend = MAX_UINT;
uint  Interface::timeLastReceive = 0;

static const struct FuncInterface
{
    typedef void(*pFuncInterfaceVpU8)(uint8 *);             // ���������� ������� ��������� �� �������� ������
    pFuncInterfaceVpU8 func;
    FuncInterface(pFuncInterfaceVpU8 f) : func(f) {};
}
commands[Command::Number] =
{
/* RequestData             */ Interface::Empty,
/* EnableChannel           */ Interface::EnableChannel,
/* SetFormWave             */ Interface::SetFormWave,
/* SetFrequency            */ Interface::ParameterValue,
/* SetAmplitude            */ Interface::ParameterValue,
/* SetOffset               */ Interface::ParameterValue,
/* SetDuration             */ Interface::ParameterValue,
/* SetDutyRatio            */ Interface::ParameterValue,
/* SetPhase                */ Interface::ParameterValue,
/* RunReset                */ Interface::RunReset,
/* ModeDebug               */ Interface::ModeDebug,
/* SetDelay                */ Interface::ParameterValue,
/* WriteRegister           */ Interface::WriteRegister,
/* SetDurationRise         */ Interface::Empty,
/* SetDurationFall         */ Interface::Empty,
/* SetDurationStady        */ Interface::Empty,
/* SetDutyFactor           */ Interface::Empty,
/* SetManipulation         */ Interface::SetManipulation,
/* SetManipulationDuration */ Interface::ParameterValue,
/* SetManipulationPeriod   */ Interface::ParameterValue,
/* SetPacketPeriod         */ Interface::ParameterValue,
/* SetPacketNumber         */ Interface::ParameterValue,
/* SetStartMode            */ Interface::SetStartMode,
/* SetPeriod               */ Interface::ParameterValue,
/* SetPolarity             */ Interface::SetPolarity,
/* SetManipulationMode     */ Interface::SetManipulationMode,
/* LoadFromDDS             */ Interface::LoadFormDDS,
/* FreqMeasure             */ Interface::Empty,
/* Log                     */ Interface::Empty,
/* FDrive_NumDirsAndFiles  */ Interface::Empty,
/* FDrive_Mount            */ Interface::Empty,
/* FDrive_RequestDir       */ Interface::Empty,
/* FDrive_RequestFile      */ Interface::Empty,
/* Test                    */ Interface::Test,
/* SetKoeffCalibration     */ Interface::SetKoeffCalibration
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Update()
{
#define TIMEOUT 200

    CPU::SetReady();

    uint size = 0;

    if (SPI1_::Receive(&size, 4))                                                               // ����� ������ ������������ ���������
    {
        Message first;              // ���� ��������� ������ ���������
        Message second;             // ���� ��������� ������ ���������

        if (first.AllocateMemory(size))
        {
            if (SPI1_::Receive(first.Data(), first.Size(), TIMEOUT))                            // ��������� ������
            {
                if (SPI1_::Transmit(&size, 4, TIMEOUT))                                         // ������� ��� ������
                {
                    if (SPI1_::Transmit(first.Data(), first.Size(), TIMEOUT))                   // � ������
                    {
                        if (SPI1_::Receive(&size, 4))
                        {
                            if (second.AllocateMemory(size))                                    // ������ ��� ��������� ����� ��������� � ���� �����
                            {
                                if (SPI1_::Receive(second.Data(), second.Size(), TIMEOUT))      // ��� � ������
                                {
                                    size = second.Size();

                                    if (SPI1_::Transmit(&size, 4, TIMEOUT))
                                    {
                                        if (SPI1_::Transmit(second.Data(), second.Size(), TIMEOUT))
                                        {
                                            if (second.IsEquals(&first))                        // ���������, ������� �� ��� �������� ���������
                                            {
                                                uint8 *recv = first.Data();

                                                commands[recv[0]].func(recv);                   // �, ���� �������, ������� ��������� �� ���������
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    CPU::SetBusy();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::EnableChannel(uint8 *recv)
{
    Chan ch = (Chan::E)recv[1];
    bool enable = recv[2] == 1;
      
    Generator::EnableChannel(ch, enable);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetPolarity(uint8 *recv)
{
    Chan ch = (Chan::E)recv[1];
    FPGA::SetPolarity(ch, recv[2]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::LoadFormDDS(uint8 *recv)
{
    Chan ch = (Chan::E)recv[1];

    std::memcpy(FPGA::DataDDS(ch), recv + 2, FPGA_NUM_POINTS * 2);

    Generator::SetFormWave(ch, Form::DDS);
}

//---------------------------------------------------------------------------------------------------------------------
void Interface::Test(uint8 *)
{
    std::srand(TIME_MS);
    
    Array array(400);
    
    for(uint i = 0; i < array.Size(); i++)
    {
        array.Data()[i] = (uint8)std::rand();
    }
    
    uint size = 1 + 4 + 4 + array.Size();
    
    // �������� ���������� ����
    SPI1_::Transmit(&size, 2);
    
    // � ������ �������� ���� �����
    
    uint8 buffer[9] = {Command::Test};
    
    BitSet32 bsSize(array.Size());
    bsSize.WriteToBuffer(buffer + 1);
    
    BitSet32 bsCRC(array.CRC32());
    bsCRC.WriteToBuffer(buffer + 5);
    
    SPI1_::Transmit(buffer, 9);
    
    SPI1_::Transmit(array.Data(), array.Size());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetKoeffCalibration(uint8 *recv)
{
    Chan ch = (Chan::E)recv[1];

    static int16 * const values[] =
    {
        &CAL_AD9952_OFFSET_NEG(Chan::A),
        &CAL_AD9952_OFFSET_ZERO(Chan::A),
        &CAL_AD9952_AMPLITUDE(Chan::A),
        &CAL_DDS_MAX(Chan::A),
        &CAL_DDS_MIN(Chan::A)
    };

    values[recv[2]][ch] = (int16)recv[3];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetManipulation(uint8 *recv)
{
    Chan ch = (Chan::E)recv[1];
    AD9952::Manipulation::SetEnabled(ch, recv[2] != 0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetManipulationMode(uint8 *recv)
{
    Chan ch = (Chan::E)recv[1];
    AD9952::Manipulation::SetType(ch, recv[2]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetStartMode(uint8 *recv)
{
    Chan ch = (Chan::E)recv[1];
    StartMode mode = (StartMode)recv[2];
    FPGA::SetStartMode(ch, mode);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetFormWave(uint8 *recv)
{
    Chan ch = (Chan::E)recv[1];
    Form form = (Form::E)recv[2];
    Generator::SetFormWave(ch, form);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::WriteRegister(uint8 *recv)
{
    Register reg = (Register::E)recv[1];

    BitSet64 bs;
    for (int i = 0; i < 8; i++)
    {
        bs.byte[i] = recv[i + 2];
    }

    uint64 value = bs.dword;

    switch (reg.value)
    {
    case Register::OffsetA:
        AD5697::SetOffset(Chan::A, (float)value);
        break;

    case Register::OffsetB:
        AD5697::SetOffset(Chan::B, (float)value);
        break;

    case Register::FreqMeterLevel:
        AD5697::SetFreqLevel((float)value);
        break;

    case Register::FreqMeterHYS:
        AD5697::SetFreqHysteresys((float)value);
        break;

    case Register::FPGA_RG0_Control:
        FPGA::WriteRegister(FPGA::RG::_0_Control, value);
        break;

    case Register::FPGA_RG1_Freq:
        FPGA::WriteRegister(FPGA::RG::_1_Freq, value);
        break;

    case Register::FPGA_RG2_Amplitude:
        FPGA::WriteRegister(FPGA::RG::_2_Amplitude, value);
        break;

    case Register::FPGA_RG3_RectA:
        FPGA::WriteRegister(FPGA::RG::_3_RectA, value);
        break;

    case Register::FPGA_RG4_RectB:
        FPGA::WriteRegister(FPGA::RG::_4_RectB, value);
        break;

    case Register::FPGA_RG5_PeriodImpulseA:
        FPGA::WriteRegister(FPGA::RG::_5_PeriodImpulseA, value);
        break;

    case Register::FPGA_RG6_DurationImpulseA:
        FPGA::WriteRegister(FPGA::RG::_6_DurationImpulseA, value);
        break;

    case Register::FPGA_RG7_PeriodImpulseB:
        FPGA::WriteRegister(FPGA::RG::_7_PeriodImpulseB, value);
        break;

    case Register::FPGA_RG8_DurationImpulseB:
        FPGA::WriteRegister(FPGA::RG::_8_DurationImpulseB, value);
        break;

    case Register::FPGA_RG9_FreqMeter:
        FPGA::WriteRegister(FPGA::RG::_9_FreqMeter, value);
        freqForSend = MAX_UINT;
        break;

    case Register::FPGA_RG10_Offset:
        FPGA::WriteRegister(FPGA::RG::_10_Offset, value);
        break;

    case Register::FreqMeter_Resist:
        FreqMeter::SetResist((FreqResist::E)value);
        break;

    case Register::FreqMeter_Couple:
        FreqMeter::SetCouple((FreqCouple::E)value);
        break;

    case Register::FreqMeter_Filtr:
        FreqMeter::SetFiltr((FreqFiltr::E)value);
        break;

    case Register::Number:
    case Register::Multiplexor1:
    case Register::Multiplexor2:
    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ParameterValue(uint8 *recv)
{
    Chan ch = (Chan::E)recv[1];
    Command command = (Command::E)recv[0];
    Generator::SetParameter(ch, command, Buffer2Float(recv + 2));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::RunReset(uint8 *)
{
#ifndef WIN32

#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8000000
    typedef void(*pFunction)();
    __disable_irq();
    pFunction JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));
    __enable_irq();
    JumpToApplication();

#endif
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ModeDebug(uint8 *)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Empty(uint8 *)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SendFrequency(uint value)
{
    freqForSend = value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::UpdateOld()
{   
    /*
    uint16 numBytes = 0;
    CPU::SPI1_::Receive(&numBytes, 2);                  // ����� ���������� ����, ������� ����� �������� ������

    if(numBytes)
    {
        ResizeRecieveBuffer(numBytes);                  // ������������� ��������� ������ ������

        CPU::SPI1_::Receive(recv, numBytes);            // ��������� ������

        if(recv[0] < Command::Number)
        {
            if(recv[0] == Command::RequestData)         // ���� ������ �� �������� ������
            {
                SendData();                             // �� �������� ������
            }
            else
            {
                commands[recv[0]].func();               // ����� ������������ �������
            }
        }
        else
        {
            Console::AddFormatString("������� ������������ ������� %d", recv[0]);
        }
    }
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Send(void *buffer, uint size)
{
    SPI1_::Transmit(&size, 2);

    SPI1_::Transmit(buffer, size);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::Data(uint size, uint8 command) : data(0), size(0)
{
    Init(size);

    if (size)
    {
        data[0] = command;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::~Data()
{
    Release();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::Init(uint _size)
{
    if (data)
    {
        delete data;
    }

    size = _size;

    if (size)
    {
        data = (uint8 *)std::malloc(size);

        return data != 0;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Data::Release()
{
    if (data)
    {
        delete data;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::IsEmpty() const
{
    return data == 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Interface::Data::GetData()
{
    return data;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Interface::Data::GetSize() const
{
    return size;
}
