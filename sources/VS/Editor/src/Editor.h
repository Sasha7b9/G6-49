#pragma once
#pragma warning(push, 0)
#include "wx/wx.h"
#pragma warning(pop)


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);
    ~Frame();

    void OnQuit(wxCommandEvent &);
    void OnTimer(wxTimerEvent &);
    void OnRepaint(wxPaintEvent &);

    // ������� ����������� ���� � ������� pos
    void ShowContextMenu(const wxPoint &pos);

    // ������������� ����� (����� ����� ������ ������� �����)
    void BlockCanvas();

    // �������������� �����
    void UnBlockCanvas();

    // ���������� true, ���� ����� ������������ (������ ������� ����� ������)
    bool IsBlockingCanvas() { return isBlockingCanvas; }

private:
    wxTimer timer;

    wxToolBar *toolBar = nullptr;

    bool isBlockingCanvas = false;  // ���� true - ������ ������� ����� ������

    void SetSizeAndPosition();

    wxRect GetMaxDisplay();

    void CreateMenu();

    void OnAlignLeft(wxCommandEvent &);

    void OnAlignRight(wxCommandEvent &);

    void OnAlignLeftTop(wxCommandEvent &);

    void OnAlignLeftDown(wxCommandEvent &);

    void OnAlignRightTop(wxCommandEvent &);

    void OnAlignRightDown(wxCommandEvent &);

    void OnDeletePoint(wxCommandEvent &);

    void OnParametersPoint(wxCommandEvent &);

	void OnUndo(wxCommandEvent &);

	void OnRedo(wxCommandEvent &);

    void OnOpenFile(wxCommandEvent &);

    void OnSaveFile(wxCommandEvent &);

    void OnNewFile(wxCommandEvent &);

    void CreateSine(wxCommandEvent &);

    void CreateTriangle(wxCommandEvent &);

    void CreateTrapeze(wxCommandEvent &);

    void CreateExponent(wxCommandEvent &);

    void CreateSinX(wxCommandEvent &);

    void InsertPoints(wxCommandEvent &);

    void OnKeyDown(wxKeyEvent &);
};


extern Frame *TheFrame;
