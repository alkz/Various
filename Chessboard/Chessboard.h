#include <vector>
#include <cstdlib>
#include <ctime>


#ifndef __CHESSB__
#define __CHESSB__

enum Colors { BLUE, RED, YELLOW, BLACK, MAGENTA, GREEN, BROWN };

class Cell
{
    public:

        Cell(unsigned int x_, unsigned int y_) { x = x_, y = y_; } ;

        unsigned int getX(void) const { return x; } ;
        unsigned int getY(void) const { return y; } ;

        void setX(unsigned int x_) { x = x_; } ;
        void setY(unsigned int y_) { y = y_; } ;

    private:

        unsigned int x;
        unsigned int y;
};


class Chessboard
{
    public:
        typedef    std::vector < std::vector <Colors> >    ColorsMatrix;
    
        Chessboard(unsigned int, unsigned int);

        bool setBomb   (void);
        bool showBomb  (void);
        bool hideBomb  (void);
        bool moveBomb  (void);
        void changeColors (void);

        Cell* getBombCell(void) const { return ( (bombPlaced) ? bomb : (static_cast<Cell* >(0)) ); } ;

        Colors getCellColor(unsigned int x, unsigned int y) const { return field[x][y]; } ;

        unsigned int getNumCells(void) const { return field.size(); };

        bool isBombHidden(void) const { return bombHidden; } ;
        bool isBombPlaced(void) const { return bombPlaced; } ;

    private:
            
        ColorsMatrix field;
        Cell *bomb;
        bool bombPlaced;
        bool bombHidden;
};


#endif
