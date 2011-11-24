#include "Chessboard.h"


Chessboard::Chessboard(unsigned int x, unsigned int y)
{
    field.assign(x, std::vector<Colors>(y));

    ColorsMatrix::iterator I_row;
    std::vector <Colors>::iterator I_col;

    srand ( time(NULL) );

    for(I_row = field.begin(); I_row < field.end(); I_row++)  {
        for(I_col = I_row->begin(); I_col < I_row->end(); I_col++)  {
            (*I_col) = (rand() % 2) ? YELLOW : BROWN;
        }
    }

    bombPlaced = false;
    bombHidden = true;
}



bool
Chessboard::setBomb(void)
{
    if(bombPlaced)  {
        return false; 
    
    } else  {
        srand ( time(NULL) );

        bomb = new Cell( (unsigned int)(rand() % field.size()), (unsigned int)(rand() % field.front().size()) );
        bombPlaced = true;

        return true;
    }
}



bool
Chessboard::showBomb(void)
{
    if(bombPlaced)  {
        bombHidden = false;

        return true;
    } else  {
        return false;
    }
}



bool
Chessboard::hideBomb(void)
{
    if(bombPlaced)  {
        bombHidden = true;
        
        return true;
    } else  {
        return false;
    }
}



bool
Chessboard::moveBomb()
{
    srand ( time(NULL) );
    
    if(bombPlaced)  {
        
        bomb->setX( (unsigned int)(rand() % field.size()) );
        bomb->setY( (unsigned int)(rand() % field.front().size()) );
        
        return true;
    } else  {
        return false;
    }
}



void
Chessboard::changeColors(void)
{
    ColorsMatrix::iterator I_row;
    std::vector <Colors>::iterator I_col;

    srand ( time(NULL) );

    for(I_row = field.begin(); I_row < field.end(); I_row++)  {
        for(I_col = I_row->begin(); I_col < I_row->end(); I_col++)  {
            (*I_col) = Colors(rand() % 7);
        }
    }

}
