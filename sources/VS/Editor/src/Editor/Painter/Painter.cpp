#include "defines.h"
#include "Editor/Painter/Painter.h"


Color Painter::currentColor(Color::NUMBER.value);
wxWindow *Painter::parent = nullptr;;
wxBitmap *Painter::bitmap = nullptr;
wxMemoryDC Painter::memDC;


void Painter::Init(wxWindow *_parent)
{
    parent = _parent;

    bitmap = new wxBitmap(parent->GetClientSize());
}


void Painter::DeInit()
{
    delete bitmap;
}


wxBitmap *Painter::GetBitmap()
{
    return bitmap;
}


void Painter::OnResizeEvent(wxSizeEvent &event)
{
    delete bitmap;
    bitmap = new wxBitmap(event.GetSize());
}


void Painter::BegineScene()
{
    memDC.SelectObject(*bitmap);
    wxBrush brush(*wxBLACK, wxBRUSHSTYLE_SOLID);
    memDC.SetBackground(brush);
    memDC.Clear();
}


void Painter::EndScene()
{
    memDC.SelectObject(wxNullBitmap);
}


void Painter::DrawPoint(int x, int y, int size, const Color &color)
{
    SetSolidBrush(color);

    SetColor(color);

    x -= size / 2;
    y -= size / 2;

    memDC.DrawRectangle({ x, y, size, size });
}


void Painter::FillRegion(int x, int y, int width, int height, const Color &color)
{
    SetSolidBrush(color);

    SetColor(color);

    memDC.DrawRectangle(x, y, width, height);
}


void Painter::DrawRectangle(int x, int y, int width, int height, const Color &color)
{
    SetTransparentBrush();

    SetColor(color);

    memDC.DrawRectangle(x, y, width, height);
}


void Painter::SetColor(const Color &color)
{
    if (color != Color::NUMBER)
    {
        currentColor = color;

        wxColour colorDraw = MakeColour(color);

        memDC.SetPen(wxPen(colorDraw));
    }
}


void Painter::SetSolidBrush(const Color &color)
{
    if (color != Color::NUMBER)
    {
        wxBrush brush(MakeColour(color), wxBRUSHSTYLE_SOLID);
        memDC.SetBrush(brush);
    }
    else
    {
        wxBrush brush(MakeColour(currentColor), wxBRUSHSTYLE_SOLID);
        memDC.SetBrush(brush);
    }
}


void Painter::SetTransparentBrush()
{
    wxBrush brush(*wxBLACK, wxBRUSHSTYLE_TRANSPARENT);
    memDC.SetBrush(brush);
}


wxColour Painter::MakeColour(const Color &color)
{
    uint value = COLOR(color.value);

    uint8 b = static_cast<uint8>(value);
    uint8 g = static_cast<uint8>(value >> 8);
    uint8 r = static_cast<uint8>(value >> 16);

    return wxColour(r, g, b);
}


void Painter::DrawLine(int x0, int y0, int x1, int y1, const Color &color)
{
    SetColor(color);

    memDC.DrawLine(x0, y0, x1, y1);
}