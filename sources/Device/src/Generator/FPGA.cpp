#include "stdafx.h"
#ifndef WIN32
#include "FPGA.h"
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Modules/EEPROM.h"
#include "Utils/Math.h"
#include "Generator/GeneratorDevice.h"
#include "Settings/CalibrationSettings.h"
#include <stdio.h>
#include <cstdlib>
#include <math.h>
#else
#pragma warning(push)
#pragma warning(disable:4310)
#endif


namespace FPGA
{
    struct RG0
    {
        enum E
        {
            _0_WriteData,           ///< � ���� ��� ���������� 0, ����� ��������� ������ ������� � ����
            _1_ImpulseA,            ///< 1, ���� � ������ � ���� ��������� ��������/�������������
            _2_ImpulseB,            ///< 1, ���� � ������ B ���� ��������� ��������/�������������
            _3_ManipulationOSK2,    ///< ����� 0, ���� ����� ������ 1 ������ ���������������� �������� OSK2 ("�����" �� AD9952 ������� ������)
            _4_ManipulationFPGA1,   ///< ����� 0, ���� ����� ������ 1 ������ ���������������� �������������� ��������� ������ 1
            _5_ManipulationOSK1,    ///< ����� 0, ���� ����� ������ 2 ������ ���������������� �������� OSK1 ("�����" �� AD9952 ������� ������)
            _6_ManipulationFPGA2,   ///< ����� 0, ���� ����� ������ 2 ������ ���������������� �������������� ��������� ������ 2
            _7_Freq_MHz,            ///< 1, ���� �������� ������� 1���
            _8_MeanderA,            ///< 1, ���� ������ �� ������ A
            _9_MeanderB,            ///< 1, ���� ������ �� ������ B
            _10_HandStartA,         ///< ���� ��� ���������� � 1, �� ������ ����� �������
            _11_HandStartB,
            _12_HandStartPacket,    ///< 1, ���� ������� �������� ����� ���������
            _13_StartMode0,         ///< ������� ��� ���������� ������� �������
            _14_StartMode1          ///< ������� ��� ���������� ������� �������
        };
    };

    void SetModeSine(Chan::E ch);
    /// ���������� ����� ����+
    void SetModeRampPlus(Chan::E ch);
    /// ���������� ����� ����-
    void SetModeRampMinus(Chan::E ch);
    /// ���������� ����� �����������
    void SetModeTriangle(Chan::E ch);
    /// ���������� ����� ������������� �������, ������������ � ������
    void SetModeDDS(Chan::E ch);

    void SetModeMeander(Chan::E ch);

    void SetModeImpulse(Chan::E ch);

    void SetModePackedImpulse(Chan::E ch);
    /// ������� ������������ ����� � ����
    void SendData(uint8 *data);
    /// �������� ���� � ����
    void WriteByte(uint8 byte);
    /// ���������� �� A0_RG...A3_RG �����, ������������� ��������
    void WriteAddress(RG::E reg);
    /// ������ ������������ ��������
    void WriteControlRegister();
    /// ���������� ����, ��������������� ������������� � ������������ ��������
    void WriteMaxAmplitude(Chan::E ch);
    /// ����������� �������� � ������ ���, ��������� ��� ������ � �������
    uint OffsetToCode(Chan::E ch);
    /// ���������� ����, ��������������� ������ �������
    uint16 SetBitsStartMode(uint16 data);
    /// ����� �������
    StartMode startMode[Chan::Count] = { StartMode::Auto, StartMode::Auto };

    float amplitude[Chan::Count] = { 10.0F, 10.0F };

    float offset[Chan::Count] = { 5.0F, 5.0F };
    /// ����� �������� ���������� � �������� ��������
    uint64 registers[RG::Count] = { 0 };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_VALUE ((1 << 14) - 1)
#define MIN_VALUE (0)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FPGA::ModeWork::E       FPGA::modeWork[Chan::Count] = { FPGA::ModeWork::None, FPGA::ModeWork::None };;
FPGA::ClockFrequency::E FPGA::clock = FPGA::ClockFrequency::_100MHz;
ParamValue              FPGA::PacketImpulse::periodImpulse(static_cast<uint64>(0));
ParamValue              FPGA::PacketImpulse::durationImpulse(static_cast<uint64>(0));

/// \brief ����� �������� ��������, ��������������� ��������������� ��� ������� � ����. ������� ���� ������� 8 ���, � ����� ������� 6 ���
/// ������ ������ ���� �������� � ������ ���� - 0 � ������� ������� ���������� ������������� �����, � 1 - �������������
static uint8 dataDDS[Chan::Count][FPGA::NUM_POINTS * 2] __attribute__((section("CCM_DATA")));


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{
    // ����������� ������ ��� ������ � �������� ����

    CPU::WritePin(GeneratorWritePin::FPGA_WR_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_CLK_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_DT_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_A0_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_A1_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_A2_RG, false);
    CPU::WritePin(GeneratorWritePin::FPGA_A3_RG, false);

    Multiplexor::Init();

    WriteMaxAmplitude(Chan::A);
    WriteMaxAmplitude(Chan::B);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetWaveForm(Chan::E ch, Form::E form)
{
    typedef void(*pFuncFpgaVU8)(Chan::E);

    static const pFuncFpgaVU8 funcs[Form::Count] =
    {
        SetModeSine,            ///< ����� ���������� ����� ����������� �����������
        SetModeRampPlus,
        SetModeRampMinus,
        SetModeTriangle,
        SetModeMeander,
        SetModeImpulse,
        SetModePackedImpulse,
        SetModeDDS
    };
    
    funcs[form](ch);
    
    Multiplexor::SetMode(ch, form);

    WriteControlRegister();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModeMeander(Chan::E ch)
{
    modeWork[ch] = ModeWork::Meander;
    WriteControlRegister();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteMaxAmplitude(Chan::E ch)
{
    // �������� ������������ ������ ������� � ��������

    uint64 data = (static_cast<uint64>(16383) << 14) + (8191);

    static const RG::E regs[Chan::Count] = { RG::_3_RectA, RG::_4_RectB };

    WriteRegister(regs[ch], data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModeSine(Chan::E ch)
{
    modeWork[ch] = ModeWork::Sine;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModeDDS(Chan::E ch)
{
    modeWork[ch] = ModeWork::DDS;
    SendData(DataFlash(ch));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModeRampPlus(Chan::E ch)
{
    modeWork[ch] = ModeWork::DDS;
    SendData(&dataDDS[Chan::A][0]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModeRampMinus(Chan::E ch)
{
    modeWork[ch] = ModeWork::DDS;
    SendData(&dataDDS[Chan::A][0]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModeTriangle(Chan::E ch)
{
    modeWork[ch] = ModeWork::DDS;
    SendData(&dataDDS[Chan::A][0]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModePackedImpulse(Chan::E)
{
    modeWork[Chan::A] = ModeWork::PackedImpulse;
    WriteControlRegister();

    uint64 data = (16383 << 14) + 8191;

    WriteRegister(RG::_3_RectA, data);

    SetPolarity(Chan::A, 0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetPolarity(Chan::E ch, uint8 polarity)
{
    uint64 data = (8191 << 14) + 16383;     // ������������� ����������

    if(polarity == 1)
    {
        data = (16383 << 14) + 8191;        // ������������� ����������
    }

    static const RG::E regs[Chan::Count] = {RG::_3_RectA, RG::_4_RectB};

    WriteRegister(regs[ch], data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModeImpulse(Chan::E ch)
{
    modeWork[ch] = ModeWork::Impulse;
    WriteControlRegister();

    SetPolarity(ch, 0);         // ������������� ������������� ����������
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetFrequency(Chan::E ch, ParamValue frequency)
{
    WriteControlRegister();
    
    if(modeWork[ch] == ModeWork::Meander)
    {
        AD9952::SetFrequency(ch, frequency);
    }
    else if (modeWork[ch] == ModeWork::DDS)
    {
        uint64 N = static_cast<uint64>((frequency.ToFloat() * (static_cast<uint64>(1) << 40)) / 1E8F);
        WriteRegister(RG::_1_Freq, N);
    }
    else if(modeWork[ch] == ModeWork::Impulse || modeWork[ch] == ModeWork::Impulse2)
    {
        if ((ch == Chan::B) && (modeWork[ch] == ModeWork::Impulse2))
        {
            modeWork[ch] = ModeWork::Impulse;
            WriteControlRegister();
        }
        uint N = static_cast<uint>(1E8F / frequency.ToFloat() + 0.5F);
        WriteRegister(ch == Chan::A ? RG::_5_PeriodImpulseA : RG::_7_PeriodImpulseB, N);
    }
    else
    {
        // ����� ������
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetDurationImpulse(Chan::E ch, ParamValue duration)
{
    PacketImpulse::durationImpulse = duration;

    RG::E reg = Chan(ch).IsA() ? RG::_6_DurationImpulseA : RG::_8_DurationImpulseB;
    if(Chan(ch).IsA() && (modeWork[Chan::A] == ModeWork::PackedImpulse))
    {
        reg = RG::_8_DurationImpulseB;
    }

    uint64 value = duration.ToUINT64() / 10;

    WriteRegister(reg, value);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::PacketImpulse::SetPeriodPacket(ParamValue period)
{
    uint64 value = period.ToUINT64() / 10 - 2;

    WriteRegister(RG::_5_PeriodImpulseA, value);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::PacketImpulse::SetNumberImpules(uint value)
{
    uint64 n = static_cast<uint64>(((value - 1) * periodImpulse.ToFloat() + durationImpulse.ToFloat()) / 10E-9F);

    WriteRegister(RG::_6_DurationImpulseA, n);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetPeriodImpulse(Chan::E ch, ParamValue period)
{
    // ��� ��������� � ���������� ���������� ������� ������ ������� �����. ������� ��������� �������� ������� ���������, ����� ������������ ���
    // � �������� ������ ��� �������������

    PacketImpulse::periodImpulse = period;

    RG::E reg = Chan(ch).IsA() ? RG::_5_PeriodImpulseA : RG::_7_PeriodImpulseB;
    if(Chan(ch).IsA() && (modeWork[Chan::A] == ModeWork::PackedImpulse))
    {
        reg = RG::_7_PeriodImpulseB;
    }

    uint64 value = period.ToUINT64() / 10 - 2;

    WriteRegister(reg, value);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetStartMode(Chan::E ch, StartMode mode)
{
    startMode[ch] = mode;
    WriteControlRegister();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetClockAD992(ClockFrequency::E _clock)
{
    clock = _clock;
    WriteControlRegister();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteControlRegister()
{
    uint16 data = BIN_U16(00000000, 01111000);       // ���� 3, 4, 5, 6 ������������� � ��������� ���������. �� �������� ��������� - �������

    Bit::Set(data, RG0::_0_WriteData);                    // � ������� ���� 0 ���������� ������ ��� ������ ������ � ������

    if((modeWork[Chan::A] == ModeWork::Sine) && AD9952::Manipulation::IsEnabled(Chan::A))
    {
        switch(AD9952::Manipulation::GetType(Chan::A))
        {
            case AD9952::Manipulation::Type::OSK:
                Bit::Clear(data, RG0::_3_ManipulationOSK2);
                break;
            case AD9952::Manipulation::Type::FPGA:
                Bit::Clear(data, RG0::_4_ManipulationFPGA1);
                break;
        }
    }

    if ((modeWork[Chan::B] == ModeWork::Sine) && AD9952::Manipulation::IsEnabled(Chan::B))
    {
        switch (AD9952::Manipulation::GetType(Chan::B))
        {
            case AD9952::Manipulation::Type::OSK:
                Bit::Clear(data, RG0::_5_ManipulationOSK1);
                break;
            case AD9952::Manipulation::Type::FPGA:
                Bit::Clear(data, RG0::_6_ManipulationFPGA2);
                break;
        }
    }

    switch(static_cast<uint8>(modeWork[Chan::A])) //-V2520
    {
        case ModeWork::Meander:     
            Bit::Set(data, RG0::_8_MeanderA);
        case ModeWork::Rectangle:
        case ModeWork::Impulse:
        case ModeWork::PackedImpulse:
            Bit::Set(data, RG0::_1_ImpulseA);
            break;
    }

    switch(static_cast<uint8>(modeWork[Chan::B])) //-V2520
    {
        case ModeWork::Meander:   
            Bit::Set(data, RG0::_9_MeanderB);
        case ModeWork::Rectangle:
        case ModeWork::Impulse:
            Bit::Set(data, RG0::_2_ImpulseB);
            break;
    }

    if(FPGA::clock == FPGA::ClockFrequency::_1MHz)
    {
        Bit::Set(data, RG0::_7_Freq_MHz);
    }

    if(modeWork[Chan::A] == ModeWork::PackedImpulse)
    {
        Bit::Set(data, RG0::_12_HandStartPacket);
    }

    data = SetBitsStartMode(data);

    WriteRegister(RG::_0_Control, data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 FPGA::SetBitsStartMode(uint16 data)
{
    ModeWork::E mode = modeWork[Chan::A];
    StartMode start = startMode[Chan::A];

    if((mode == ModeWork::Impulse) || (mode == ModeWork::PackedImpulse))
    {
        if(start.Is(StartMode::Single))
        {
            Bit::Set(data, RG0::_10_HandStartA);
        }
    }

    if(mode == ModeWork::DDS)
    {
        if(start.Is(StartMode::ComparatorA))
        {
            Bit::Set(data, RG0::_13_StartMode0);
        }
        else if(start.Is(StartMode::ShaperB))
        {
            Bit::Set(data, RG0::_14_StartMode1);
        }
        else if(start.Is(StartMode::Single))
        {
            Bit::Set(data, RG0::_13_StartMode0);
            Bit::Set(data, RG0::_14_StartMode1);
        }
        else
        {
            // ����� ������
        }
    }

    mode = modeWork[Chan::B];
    start = startMode[Chan::B];

    if((mode == ModeWork::Impulse) || (mode == ModeWork::PackedImpulse))
    {
        if(start.Is(StartMode::Single))
        {
            Bit::Set(data, RG0::_11_HandStartB);
        }
    }

    if (mode == ModeWork::DDS)
    {
        if (start.Is(StartMode::ComparatorA))
        {
            Bit::Set(data, RG0::_13_StartMode0);
        }
        else if (start.Is(StartMode::ShaperB))
        {
            Bit::Set(data, RG0::_14_StartMode1);
        }
        else if (start.Is(StartMode::Single))
        {
            Bit::Set(data, RG0::_13_StartMode0);
            Bit::Set(data, RG0::_14_StartMode1);
        }
        else
        {
            // ����� ������
        }
    }

    return data;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::Start()
{
    if(startMode[Chan::A] != StartMode::Auto || startMode[Chan::B] != StartMode::Auto)
    {
        WriteRegister(RG::_11_Start, 2);
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SendData(uint8 *data)
{
    WriteRegister(RG::_0_Control, 0);
    
    uint8 *pointer = data;

    /// \todo ��� �������� ��� �������� ������������
    __IO int i = 0;

    for(i = 0; i < 10; i++)
    {
        *pointer = data[0];
    }

    for(i = 0; i < FPGA::NUM_POINTS * 4; i++)
    {
        WriteByte(*pointer++);

        /// \todo ��� �������� ��� �������� ������������
        __IO int j = 0;
        for(j = 0; j < 10; j++) {}
        CPU::WritePin(GeneratorWritePin::FPGA_WR_DATA, true);
        for(j = 0; j < 10; j++) {}
        CPU::WritePin(GeneratorWritePin::FPGA_WR_DATA, false);
    }

    WriteRegister(RG::_0_Control, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteByte(uint8 byte)
{
    //                                                                                 ���� 0,1                                    ���� 2,3
    GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(byte) & 0x03) << 14) + ((static_cast<uint16>(byte & 0x0c)) >> 2);;
    //                                                                                ���� 4,5,6
    GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(byte) & 0xf0) << 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteRegister(RG::E reg, uint64 value)
{
    static const int numBits[RG::Count] =
    {
        16, // _0_Control,
        40, // _1_Freq,
        20, // _2_Amplitude,
        28, // _3_RectA,
        28, // _4_RectB,
        32, // _5_PeriodImpulseA,
        32, // _6_DurationImpulseA,
        32, // _7_PeriodImpulseB,
        32, // _8_DurationImpulseB,
        13, // _9_FreqMeter
        28, // _10_Offset
        2   // _11_Start
    };

    registers[reg] = value;

    WriteAddress(reg);

    for(int i = 0; i < 2; i++)
    {
        for (int bit = numBits[reg] - 1; bit >= 0; bit--)
        {
            CPU::WritePin(GeneratorWritePin::FPGA_DT_RG, Bit::Get(value, bit));     // ������������� ��� ���������� ��������������� ���
            CPU::WritePin(GeneratorWritePin::FPGA_CLK_RG, true);                    // � ���������� ��� � ����
            CPU::WritePin(GeneratorWritePin::FPGA_CLK_RG, false);
        }
    }

    CPU::WritePin(GeneratorWritePin::FPGA_WR_RG, true);                         // ������ ������������ ������ �� ����������� �������� � FPGA
    CPU::WritePin(GeneratorWritePin::FPGA_WR_RG, false);
    Timer::PauseOnTime(10);                                                     // ��� 10 �����������, ���� ������ ����������� � FPGA
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteAddress(RG::E reg)
{
    static const GeneratorWritePin pins[] = 
    {
        GeneratorWritePin::FPGA_A0_RG, 
        GeneratorWritePin::FPGA_A1_RG, 
        GeneratorWritePin::FPGA_A2_RG, 
        GeneratorWritePin::FPGA_A3_RG
    };

    for (int i = 0; i < 4; i++)
    {
        CPU::WritePin(pins[i], Bit::Get(reg, i));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::OffsetToCode(Chan::E ch)
{
    float off = offset[ch];

    uint max = 0x1fff;

    float k = 1.0F + CAL_DDS_MAX(ch) / 1023.0F;

    int code = ~(static_cast<int>(off / 5.0F * max * k)) + 1;

    return static_cast<uint>(code & 0x3fff);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetAmplitude(Chan::E ch, ParamValue ampl)
{
    amplitude[ch] = ampl.ToFloat();

    uint nA = (uint)((amplitude[Chan::A] * (1023 + CAL_DDS_MAX(Chan::A))) / 10);
    uint nB = (uint)((amplitude[Chan::B] * (1023 + CAL_DDS_MAX(Chan::B))) / 10);

    WriteRegister(RG::_2_Amplitude, nA + (nB << 10));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetOffset(Chan::E ch, ParamValue off)
{
    offset[ch] = -off.ToFloat();

    uint nA = OffsetToCode(Chan::A);
    uint nB = OffsetToCode(Chan::B);
    
    WriteRegister(RG::_10_Offset, nA + (nB << 14));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *FPGA::DataDDS(Chan::E ch)
{
    return &dataDDS[ch][0];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *FPGA::DataFlash(Chan::E ch)
{
    return reinterpret_cast<uint8 *>(EEPROM::Signal::Get(ch));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SaveExtSignal(Chan::E ch, uint8 *data)
{
    EEPROM::Signal::Save(ch, reinterpret_cast<uint16 *>(data));
}

#ifdef WIN32
#pragma warning(pop)
#endif
