#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/timer.h>

#include "Chessboard.h"

#ifndef __CHESSBFRAME__
#define __CHESSBFRAME__

const unsigned int CELL_WIDTH  = 60;
const unsigned int CELL_HEIGHT = 60;

class ChessboardFrame : public wxFrame
{
    public:
       ChessboardFrame(Chessboard* CB, wxWindow* parent, wxWindowID id = 1, const wxString& title = wxT("Chessboard"),  
                       const wxPoint& pos = wxDefaultPosition, const wxSize& = wxSize(850, 650),           
                       long style = wxDEFAULT_FRAME_STYLE);

        virtual ~ChessboardFrame();

        void paintNIGGER(void);
        void drawBomb(void);
        void eraseBomb(void);

        void ButtonSetBombClick      (wxCommandEvent& );
        void ButtonShowBombClick     (wxCommandEvent& );
        void ButtonHideBombClick     (wxCommandEvent& );
        void ButtonChangeColorsClick (wxCommandEvent& );
        void ButtonTimerClick        (wxCommandEvent& );
        void timeoutBombTimer        (wxTimerEvent&   );

    private:

        wxPanel*     drawPane;
        wxBoxSizer*  sizer;
        wxTimer*     bombTimer;

        wxButton*    ButtonSetBomb;
        wxButton*    ButtonShowBomb;
        wxButton*    ButtonHideBomb;
        wxButton*    ButtonChangeColors;
        wxButton*    ButtonTimer;

        Chessboard*  myCB;

        const wxBrush* getBrushColor(Colors);
        wxColour getColor(Colors);

    private:

        DECLARE_EVENT_TABLE();
};

#endif
