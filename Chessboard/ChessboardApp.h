
#include <wx/wx.h>

#include "Chessboard.h"
#include "ChessboardFrame.h"

class ChessboardApp : public wxApp
{
    private:
        virtual bool OnInit();

        Chessboard*      myCB;
        ChessboardFrame* frame;

    public:
        void paintEvent (wxPaintEvent& );
        void mouseMoved (wxMouseEvent& );

    private:

        DECLARE_EVENT_TABLE();
};

