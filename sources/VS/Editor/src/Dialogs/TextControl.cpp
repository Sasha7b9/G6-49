#include "Dialogs/TextControl.h"


TextControl::TextControl(int _min, int _max, wxDialog *dlg, wxWindowID id, const wxString &value, const wxPoint &position, const wxSize &size) : wxTextCtrl(dlg, id, value, position, size),
    min(_min), max(_max)
{
    Bind(wxEVT_KEY_DOWN, &TextControl::OnKeyDown, this);
}


void TextControl::OnKeyDown(wxKeyEvent &event)
{
    int code = event.GetKeyCode();

    if (code >= '0' && code <= '9')
    {
        long start = 0;
        long end = 0;

        GetSelection(&start, &end);

        char buffer[50];

        if (end - start != 0)
        {
            buffer[0] = static_cast<char>(code);
            buffer[1] = 0;

            SetValue("");
            this->AppendText(buffer);

            event.Skip(false);
        }
        else
        {
            std::strcpy(buffer, GetValue().c_str());
            uint length = std::strlen(buffer);
            buffer[length] = static_cast<char>(code);
            buffer[length + 1] = 0;
            int value = atoi(buffer);

            event.Skip(value >= min && value <= max);
        }
    }
    else if (code == WXK_RETURN || code == WXK_BACK || code == WXK_LEFT || code == WXK_RIGHT || code == WXK_TAB || code == WXK_DELETE || code == WXK_INSERT)
    {
        event.Skip(true);
    }
    else
    {
        event.Skip(false);
    }
}
