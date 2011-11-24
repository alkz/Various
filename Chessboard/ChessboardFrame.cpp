#include "ChessboardFrame.h"


BEGIN_EVENT_TABLE(ChessboardFrame, wxFrame)
    EVT_BUTTON(10, ChessboardFrame::ButtonSetBombClick)
    EVT_BUTTON(11, ChessboardFrame::ButtonShowBombClick)
    EVT_BUTTON(12, ChessboardFrame::ButtonHideBombClick)
    EVT_BUTTON(13, ChessboardFrame::ButtonChangeColorsClick)
    EVT_BUTTON(14, ChessboardFrame::ButtonTimerClick)
    EVT_TIMER(20, ChessboardFrame::timeoutBombTimer)
END_EVENT_TABLE()



ChessboardFrame::ChessboardFrame(Chessboard* CB, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, 
                                 const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
    myCB = CB;
    sizer = new wxBoxSizer(wxHORIZONTAL);
    drawPane = new wxPanel( (ChessboardFrame* ) this , 1);
    drawPane->SetBackgroundColour( wxColour(255,255,255));
    sizer->Add(drawPane, 1, wxEXPAND);


    ButtonSetBomb = new wxButton();
    ButtonSetBomb->Create(drawPane, 10, wxT("Set Bomb"));

    ButtonShowBomb = new wxButton();
    ButtonShowBomb->Create(drawPane, 11, wxT("Show Bomb"));

    ButtonHideBomb = new wxButton();
    ButtonHideBomb->Create(drawPane, 12, wxT("Hide Bomb"));

    ButtonChangeColors = new wxButton();
    ButtonChangeColors ->Create(drawPane, 13, wxT("Change Colors"));

    ButtonTimer = new wxButton();
    ButtonTimer ->Create(drawPane, 14, wxT("Bomb Timer Start"));

    this->SetSizer(sizer);
    this->SetAutoLayout(true);

    bombTimer = new wxTimer();
    bombTimer->SetOwner(this, 20);
}



void 
ChessboardFrame::paintNIGGER(void)
{
    wxPaintDC dc(drawPane);
    dc.SetPen( wxPen( wxColor(0,0,0), 1 ) );
    int k = GetVirtualSize().GetWidth()/2 - myCB->getNumCells()/2*CELL_WIDTH;
    int k1 = GetVirtualSize().GetHeight()/2 - myCB->getNumCells()/2*CELL_HEIGHT;

    ButtonSetBomb->SetPosition( wxPoint(GetVirtualSize().GetWidth()/2 + myCB->getNumCells()/2*CELL_WIDTH + CELL_WIDTH*2,  GetVirtualSize().GetHeight()/2 - myCB->getNumCells()/2*CELL_HEIGHT) );
    ButtonShowBomb->SetPosition( wxPoint(GetVirtualSize().GetWidth()/2 + myCB->getNumCells()/2*CELL_WIDTH + CELL_WIDTH*2, GetVirtualSize().GetHeight()/2 - myCB->getNumCells()/2*CELL_HEIGHT+30) );
    ButtonHideBomb->SetPosition( wxPoint(GetVirtualSize().GetWidth()/2 + myCB->getNumCells()/2*CELL_WIDTH + CELL_WIDTH*2, GetVirtualSize().GetHeight()/2 - myCB->getNumCells()/2*CELL_HEIGHT+60) );
    ButtonChangeColors->SetPosition( wxPoint(GetVirtualSize().GetWidth()/2 + myCB->getNumCells()/2*CELL_WIDTH + CELL_WIDTH*2, GetVirtualSize().GetHeight()/2 - myCB->getNumCells()/2*CELL_HEIGHT+90 ) );
    ButtonTimer->SetPosition( wxPoint(GetVirtualSize().GetWidth()/2 + myCB->getNumCells()/2*CELL_WIDTH + CELL_WIDTH*2, GetVirtualSize().GetHeight()/2 - myCB->getNumCells()/2*CELL_HEIGHT+120 ) );

    ButtonSetBomb->Show();
    ButtonShowBomb->Show();
    ButtonHideBomb->Show();
    ButtonChangeColors->Show();
    ButtonTimer->Show();

    Colors color;
    for (unsigned int i = 0; i < myCB->getNumCells(); i++)  {
        for (unsigned int j = 0; j < myCB->getNumCells(); j++)  {
            color = myCB->getCellColor(i,j);
            dc.SetBrush( *getBrushColor(color) );
            dc.DrawRectangle(i*CELL_WIDTH+k, j*CELL_HEIGHT+k1, CELL_WIDTH, CELL_HEIGHT);
            if(!myCB->isBombHidden())  {
                drawBomb();
            }
        }
    }
}



const wxBrush*
ChessboardFrame::getBrushColor(Colors c)
{
    wxBrush* tmp;
    switch(c)  {
        case BLUE:
            return wxBLUE_BRUSH;
        case RED:
            return wxRED_BRUSH;
        case YELLOW:
            return wxYELLOW_BRUSH; 
        case BLACK:
            return wxBLACK_BRUSH;
        case MAGENTA:
            tmp = new wxBrush(wxColor(255,0,255));
            return tmp;
        case GREEN:
            return wxGREEN_BRUSH;
        case BROWN:
            tmp = new wxBrush(wxColor(92,52,23));
            return tmp;
    }
}



wxColour 
ChessboardFrame::getColor(Colors c)
{
    switch(c)  {
        case BLUE:
            return *wxBLUE;
        case RED:
            return *wxRED;
        case YELLOW:
            return *wxYELLOW;
        case BLACK:
            return *wxBLACK;
        case MAGENTA:
            return (wxColor(255,0,255));
        case GREEN:
            return *wxGREEN;
        case BROWN:
            return (wxColor(92,52,23));
    }
}



void
ChessboardFrame::drawBomb(void)
{
    wxPaintDC dc(drawPane);
    int k = GetVirtualSize().GetWidth()/2 - myCB->getNumCells()/2*CELL_WIDTH;
    int k1 = GetVirtualSize().GetHeight()/2 - myCB->getNumCells()/2*CELL_HEIGHT;

    Cell* bombCell = myCB->getBombCell();
    dc.SetPen( wxPen( wxColor(255,0,0), 4 ) );
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawCircle( wxPoint( bombCell->getX() * CELL_WIDTH+CELL_WIDTH/2+k, bombCell->getY() * CELL_HEIGHT+CELL_HEIGHT/2+k1 ), CELL_WIDTH/3 );
}



void
ChessboardFrame::eraseBomb(void)
{
    wxPaintDC dc(drawPane);
    int k = GetVirtualSize().GetWidth()/2 - myCB->getNumCells()/2*CELL_WIDTH;
    int k1 = GetVirtualSize().GetHeight()/2 - myCB->getNumCells()/2*CELL_HEIGHT;

    Cell* bombCell = myCB->getBombCell();
    Colors color = myCB->getCellColor(bombCell->getX(),bombCell->getY());
    dc.SetPen( wxPen( getColor(color), 5 ) );
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawCircle( wxPoint( bombCell->getX() * CELL_WIDTH+CELL_WIDTH/2+k, bombCell->getY() * CELL_HEIGHT+CELL_HEIGHT/2+k1 ), CELL_WIDTH/3 );
}



void 
ChessboardFrame::ButtonSetBombClick(wxCommandEvent &event)
{
    if(!myCB->setBomb())  {
        wxMessageBox("Bomb already set", "Chessboard");
    }
}



void 
ChessboardFrame::ButtonShowBombClick(wxCommandEvent &event)
{
    if(!myCB->showBomb())  {
        wxMessageBox("Bomb has not been set", "Chessboard");
    } else  {
        drawBomb();
    }
}



void 
ChessboardFrame::ButtonHideBombClick(wxCommandEvent &event)
{
    if(!myCB->hideBomb())  {
         wxMessageBox("Bomb has not been set", "Chessboard");
    } else  {
        eraseBomb();
    }
}



void 
ChessboardFrame::ButtonChangeColorsClick(wxCommandEvent &event)
{
    myCB->changeColors();
    paintNIGGER();
}



void 
ChessboardFrame::ButtonTimerClick(wxCommandEvent &event)
{
    if(!bombTimer->IsRunning())  {
        bombTimer->Start(20000);
    }
}



void
ChessboardFrame::timeoutBombTimer(wxTimerEvent& event)
{
    if(!myCB->isBombHidden())  {
        eraseBomb();
    }
    myCB->moveBomb();
    if(!myCB->isBombHidden())  {
        drawBomb();
    }
}
