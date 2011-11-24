#include "ChessboardApp.h"

IMPLEMENT_APP(ChessboardApp)

BEGIN_EVENT_TABLE(ChessboardApp, wxApp)
    EVT_PAINT(ChessboardApp::paintEvent)
    EVT_MOTION(ChessboardApp::mouseMoved)
END_EVENT_TABLE()



bool
ChessboardApp::OnInit()
{
    myCB = new Chessboard(7,7);

    frame = new ChessboardFrame(myCB, NULL);
    frame->Show();

   return true;
}



void
ChessboardApp::paintEvent(wxPaintEvent & evt)
{
    frame->paintNIGGER();
}



void 
ChessboardApp::mouseMoved(wxMouseEvent& event)
{
    if(myCB->isBombPlaced())  {
        int k = frame->GetVirtualSize().GetWidth()/2 - myCB->getNumCells()/2*CELL_WIDTH;
        int k1 = frame->GetVirtualSize().GetHeight()/2 - myCB->getNumCells()/2*CELL_HEIGHT;
        wxPoint mousePos = event.GetPosition();
        Cell* bombCell = myCB->getBombCell();
        if( ( mousePos.x >= (bombCell->getX()*CELL_WIDTH+k ) ) && ( mousePos.x < (bombCell->getX()*CELL_WIDTH+k+CELL_WIDTH ) ) &&
            ( mousePos.y >= (bombCell->getY()*CELL_HEIGHT+k1) ) && ( mousePos.y < (bombCell->getY()*CELL_HEIGHT+k1+CELL_HEIGHT) )  )
        {

            wxMessageBox("Great you've found the bomb! PRRRRRR", "MA CHE OHHH!");
        }
    }
}
