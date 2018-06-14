#pragma once
#include "Settings/SettingsTypes.h"


/** @addtogroup Panel
 *  @{
 *  @defgroup Interface
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Generator
{
public:
    static void Reset();

    static void EnableChannel(Channel ch, bool enable);

    static void SetFormWave(Channel ch, WaveForm form);

    static void SetParameter(Channel ch, Type_WaveParameter param, float value);

    static void SetDebugMode(bool enable);

    static void TestSend();

private:
    
    static void SendToInterface(uint8 *buffer, int size);
    /// �������� ����� �� ���� ��� �����
    static void ShiftToLeft(uint8 *buffer, int length);

    static void MasterSynchro();
};

/** @}  @} */
