#include "Editor.h"
#pragma warning(push, 0)
#include <SDL.h>
//#include <wx/display.h>
#include <wx/wx.h>
#include <wx/mstream.h>
#pragma warning(pop)

#undef main


#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include "resource.h"


extern void update();
extern void init();


enum
{
    MENU_FILE_QUIT = wxID_EXIT,

    CONTEXT_MENU_DELETE = wxID_HIGHEST + 1,     // ������� �����
    CONTEXT_MENU_PARAMETERS,                    // ��������� �����

    CONTEXT_MENU_CLEAR,                         // �������� �����

    ALIGN_LEFT,                                 // ��������� ����� �� �����
    ALIGN_RIGHT,                                // ��������� ����� �� ������
    ALIGN_LEFT_TOP,                             // ��������� ����� �� ����� �������
    ALIGN_LEFT_DOWN,                            // ��������� ����� �� ����� ������
    ALIGN_RIGHT_TOP,                            // ��������� ����� �� ������ �������
    ALIGN_RIGHT_DOWN,                           // ��������� ����� �� ������ ������
};

enum
{
    TIMER_ID = 1
};


wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU(MENU_FILE_QUIT, Frame::OnQuit)
    EVT_MENU(CONTEXT_MENU_DELETE, Frame::OnDeletePoint)
    EVT_MENU(CONTEXT_MENU_PARAMETERS, Frame::OnParametersPoint)
    EVT_MENU(CONTEXT_MENU_CLEAR, Frame::OnClearForm)
    EVT_MENU(ALIGN_LEFT, Frame::OnAlignLeft)
    EVT_MENU(ALIGN_RIGHT, Frame::OnAlignRight)
    EVT_MENU(ALIGN_LEFT_TOP, Frame::OnAlignLeftTop)
    EVT_MENU(ALIGN_LEFT_DOWN, Frame::OnAlignLeftDown)
    EVT_MENU(ALIGN_RIGHT_TOP, Frame::OnAlignRightTop)
    EVT_MENU(ALIGN_RIGHT_DOWN, Frame::OnAlignRightDown)
    EVT_TIMER(TIMER_ID, Frame::OnTimer)
    EVT_SIZE(Frame::OnResize)
wxEND_EVENT_TABLE()


/// ������ � ���� ����
static SDL_Cursor *cursorHand = nullptr;
/// true, ���� ��� ��������� � ������� ���������
static bool mouseIsDown = false;


wxIMPLEMENT_APP_NO_MAIN(Application);


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0U) //-V2517
    {
        std::cout << "SDL_Init Error:" << SDL_GetError() << std::endl;
    }

    return wxEntry(argc, argv);
}


bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    cursorHand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    return true;
}


Frame::Frame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title),
    timer(this, TIMER_ID)
{
    SetIcon(wxICON(sample));

    CreateMenu();

    CreateStatusBar(2);

    SetSizeAndPosition();

    Show(true);

    timer.Start(0);
}


void Frame::OnTimer(wxTimerEvent &)
{
    HandlerEvents();
    update();
    DrawFPS();
}


void Frame::OnResize(wxSizeEvent &)
{
    TheCanvas->Resize(this->GetClientSize());
}


void Frame::HandlerEvents()
{
    SDL_Event event;

    static int mouseX = 0;
    static int mouseY = 0;

    while (SDL_PollEvent(&event))
    {
        SDL_PumpEvents();
        switch (event.type)
        {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                Close(true);
            }
            break;

        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;

            if (mouseIsDown)
            {
                TheForm->MovePoint(mouseX, mouseY);
            }

            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseX = event.button.x;
            mouseY = event.button.y;
                
            if (event.button.button == 1)               // "1" ������������ ���
            {
                if (TheForm->ExistPoint(mouseX, mouseY, false))
                {
                    mouseIsDown = true;
                }
                else
                {
                    TheForm->SetPoint(mouseX, mouseY);
                }
                
            }
            else if (event.button.button == 3)          // "3" ������������� ���
            {
                mouseIsDown = false;

                ShowContextMenu({ mouseX, mouseY}, TheForm->ExistPoint(mouseX, mouseY, false));
            }
            else
            {
                // ��������� ������ �� �����������
            }
            break;

        case SDL_MOUSEBUTTONUP:
            mouseIsDown = false;
            break;

        default:
            // ������ �� ������
            break;
        }
    }

    if (TheForm->ExistPoint(mouseX, mouseY, mouseIsDown))
    {
        SDL_SetCursor(cursorHand);
    }
}


void Frame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close(true);
}


void Frame::SetSizeAndPosition()
{
    wxSize size = { 1024, 768 };

    SetSize(size);
    SetMinSize({ 640, 480 });

    wxRect rect = GetMaxDisplay();

    SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}


wxRect Frame::GetMaxDisplay()
{
    wxRect result = { 0, 0, 0, 0 };

    for (uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if (rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}


void Frame::DrawFPS()
{
    static int count = 0;
    static unsigned int prevTime = 0;

    count++;

    if (SDL_GetTicks() - prevTime > 1000)
    {
        float fps = static_cast<float>(count) / (SDL_GetTicks() - prevTime) * 1000.0F;

        char buffer[100];
        sprintf(buffer, "fps %f", fps);

        SetStatusText(buffer);

        prevTime = SDL_GetTicks();
        count = 0;
    }
}

wxBitmap* GetBitmapFromMemory(const char* t_data, const DWORD t_size)
{
	wxMemoryInputStream a_is(t_data, t_size);
	return new wxBitmap(wxImage(a_is), -1);
}

static bool LoadDataFromResource(char*& t_data, DWORD& t_dataSize, int name)
{
	bool     r_result = false;
	HGLOBAL  a_resHandle = 0;
	HRSRC    a_resource;

	a_resource = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(name), RT_RCDATA);

	if (0 != a_resource)
	{
		a_resHandle = LoadResource(NULL, a_resource);
		if (0 != a_resHandle)
		{
			t_data = (char*)LockResource(a_resHandle);
			t_dataSize = SizeofResource(NULL, a_resource);
			r_result = true;
		}
	}

	return r_result;
}

static wxBitmap* CreateBitmapFromPngResource(int name)
{
	wxBitmap* r_bitmapPtr = 0;

	char* a_data = 0;
	DWORD       a_dataSize = 0;

	if (LoadDataFromResource(a_data, a_dataSize, name))
	{
		r_bitmapPtr = GetBitmapFromMemory(a_data, a_dataSize);
	}

	return r_bitmapPtr;
}

void Frame::CreateMenu()
{
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(MENU_FILE_QUIT, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");

    SetMenuBar(menuBar);

	wxBitmap *imgUNDO = CreateBitmapFromPngResource(IDB_BMP_UNDO);
	wxBitmap* imgREDO = CreateBitmapFromPngResource(IDB_BMP_REDO);

	wxToolBar* toolBar = CreateToolBar();
	toolBar->AddTool(wxID_EDIT, wxT("��������"), *imgUNDO);
	toolBar->AddTool(wxID_EXIT, wxT("������������"), *imgREDO);
	toolBar->Realize();
}


void Frame::ShowContextMenu(const wxPoint &pos, bool underPoint)
{
    static wxMenu menuPoint;
    static wxMenu menuContext;
    static wxMenu *menuAlign = nullptr;

    if (menuAlign == nullptr)
    {
        menuAlign = new wxMenu();

        menuAlign->Append(ALIGN_LEFT, "�����");
        menuAlign->Append(ALIGN_RIGHT, "������");
        menuAlign->Append(ALIGN_LEFT_TOP, "����� ������");
        menuAlign->Append(ALIGN_LEFT_DOWN, "����� �����");
        menuAlign->Append(ALIGN_RIGHT_TOP, "������ ������");
        menuAlign->Append(ALIGN_RIGHT_DOWN, "������ �����");

        menuPoint.Append(CONTEXT_MENU_DELETE, "�������");
        menuPoint.Append(CONTEXT_MENU_PARAMETERS, "���������");
        menuPoint.AppendSubMenu(menuAlign, "���������");

        menuContext.Append(CONTEXT_MENU_CLEAR, "��������");
    }

    PopupMenu(underPoint ? &menuPoint : &menuContext, pos.x, pos.y);
}


void Frame::OnDeletePoint(wxCommandEvent &)
{
    TheForm->RemovePoint();
}


void Frame::OnClearForm(wxCommandEvent &)
{
    TheForm->Clear();
}


void Frame::OnParametersPoint(wxCommandEvent &)
{

}


void Frame::OnAlignLeft(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::Left);
}


void Frame::OnAlignRight(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::Right);
}


void Frame::OnAlignLeftTop(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::LeftTop);
}


void Frame::OnAlignLeftDown(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::LeftDown);
}


void Frame::OnAlignRightTop(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::RightTop);
}

void Frame::OnAlignRightDown(wxCommandEvent &)
{
    TheForm->AlignPoint(Align::RightDown);
}
