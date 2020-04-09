#pragma once
#pragma once
#include "common/Messages.h"


#define SIZE_CHUNK        512            /* ������ ������������ ������ ������ */


namespace Message
{
    // ���� ���������� ������ ������ ������ �� ����������
    class RequestUpgrade : public SimpleMessage
    {
    public:
        RequestUpgrade();
    };

    // ���� ���������� ���������� �������� ������ ���������� ����� �������� device
    class PortionUpgradeDevice : public SimpleMessage
    {
    public:
        PortionUpgradeDevice(int portion);
    };

    // ���� ���������� ���������� �������� ������ ���������� ����� �������� panel � ����� ������� ������
    class PortionUpgradePanel : public SimpleMessage
    {
    public:
        PortionUpgradePanel(int num, int portion, uint8 *data, int size);
    };


    class EraseSectors : public SimpleMessage
    {
    public:
        EraseSectors(int num);
    };

    // ���� ���������� device ��� ������ ���������� �� ������
    class StartMainApplication : public SimpleMessage
    {
    public:
        StartMainApplication();
    };
}
