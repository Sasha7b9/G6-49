#pragma once

/*
    ����� ������������� ����� �������
*/


class Form
{
public:
    Form();
    void SetPoint(int mouseX, int mouseY);
    /// ���������� true, ���� ������ ���� ��������� ��� ������������ ������
    bool ExistPoint(int mouseX, int mouseY);
    /// ������� ����� � ������� ����
    void RemovePoint();
    /// ����������� ����� � ����� �������
    void MovePoint(int mouseX, int mouseY);

    void Draw();
};


extern Form *TheForm;
