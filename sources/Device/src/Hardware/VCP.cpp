#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#include "defines.h"
#include "Message.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "usbd_desc.h"
#include "Utils/Math.h"
#include <stdarg.h>
#include <cstring>



USBD_HandleTypeDef VCP::handleUSBD;
bool               VCP::cableUSBisConnected = false;
bool               VCP::connectedToUSB = false;


void VCP::Init()
{
    CPU::PCD::Init();

    USBD_Init(&handleUSBD, &VCP_Desc, DEVICE_FS);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
    USBD_Start(&handleUSBD);
} 

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool VCP::PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(handleUSBD.pClassData);
    return pCDC->TxState == 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define SIZE_BUFFER_VCP 256     /// \todo ���� ��������� ������ ������ 512, �� �� ��207 �����
//static uint8 buffSend[SIZE_BUFFER_VCP];
//static int sizeBuffer = 0;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendData(const void *_buffer, uint size)
{
    volatile USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(handleUSBD.pClassData);

    while (pCDC->TxState == 1) {};
    USBD_CDC_SetTxBuffer(&handleUSBD, static_cast<uint8 *>(const_cast<void *>(_buffer)), static_cast<uint16>(size));
    USBD_CDC_TransmitPacket(&handleUSBD);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendString(char *data)
{
    SendData(reinterpret_cast<uint8 *>(data), std::strlen(data));
}


void VCP::SendStringEOF(char *data)
{
    SendString(data);
    //SendByte(0x0d);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendByte(uint8 byte)
{
    SendData(&byte, 1);
}


void VCP::Handler::Processing(SimpleMessage *msg)
{
    VCP::SendData(msg->Data(5), msg->TakeWord());
}
