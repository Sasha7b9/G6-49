#include "defines.h"
#include "AD5697.h"
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static I2C_HandleTypeDef hI2C =
{
    I2C1,
    {
        100000,                     // ClockSpeed
        I2C_DUTYCYCLE_2,            // DutyCycle
        0,                          // OwnAddress1
        I2C_ADDRESSINGMODE_7BIT,    // AddressingMode
        I2C_DUALADDRESS_DISABLE,    // DualAddressMode
        0,                          // OwnAddress2
        I2C_GENERALCALL_DISABLE,    // GeneralCallMode
        I2C_NOSTRETCH_DISABLE       // NoStretchMode
    },
    0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_I2C_STATE_RESET, HAL_I2C_MODE_NONE, 0, 0, 0, 0, 0
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AD5697::Init(void)
{
    Reset(A);
    Reset(B);
    
    GPIO_InitTypeDef isGPIO =
    {//    SCL          SDA
        GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_OD,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF4_I2C1
    };
    HAL_GPIO_Init(GPIOB, &isGPIO);

    HAL_I2C_Init(&hI2C);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::SetAmplitude(Channel ch, float amplitude)
{
    WriteParameter(ch, Amplitude, amplitude);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::SetOffset(Channel ch, float offset)
{   
    if(offset > 4096.0f)
    {
        offset = 4096.0f;
    }
    WriteParameter(ch, Offset, offset);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::WriteParameter(Channel ch, Type_WaveParameter param, float value_)
{
    static const uint8 address[NumChannels] = {BINARY_U8(00001100), BINARY_U8(00001101)};

    uint16 value = (uint16)((uint16)value_ << 4);
    uint8 buffer[3] = {CreateCommandByte(param), (uint8)(value >> 8), (uint8)value};
    TransmitI2C(address[ch], buffer);
    CPU::WritePin(PinLDAC(ch), false);
    CPU::WritePin(PinLDAC(ch), true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::TransmitI2C(uint8 address, uint8 data[3])
{
    // �������� �� ���� ��� ����� - �������� �����. ���� �� �����, ����� �������� ����� �������
    HAL_I2C_Master_Transmit(&hI2C, (uint16)(address << 1), data, 3, 100);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 AD5697::CreateCommandByte(Type_WaveParameter param)
{
    return (uint8)(BINARY_U8(00010000) | ((param == Amplitude) ? 0x01 : 0x08));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD5697::Reset(Channel ch)
{
    static const GeneratorPin pinRS[NumChannels] = {AD5697_D_RSA, AD5697_D_RSB};

    CPU::WritePin(PinLDAC(ch), true);
    CPU::WritePin(pinRS[ch], true);
    CPU::WritePin(pinRS[ch], false);
    CPU::WritePin(pinRS[ch], true);

    //SetOffset(ch, 0.0f);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
GeneratorPin AD5697::PinLDAC(Channel ch)
{
    static const GeneratorPin pinLDAC[NumChannels] = {AD5697_LDACA, AD5697_LDACB};

    return pinLDAC[ch];
}
