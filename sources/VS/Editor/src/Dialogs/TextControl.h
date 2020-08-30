#pragma once
#include "defines.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class TextControl : public wxTextCtrl
{
public:
    TextControl(int min, int max, wxDialog *dlg, wxWindowID id, const wxString &value, const wxPoint &position, const wxSize &size);
private:
    int min = 0;
    int max = 0;
    void OnKeyDown(wxKeyEvent &);
};