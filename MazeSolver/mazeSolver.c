#include <stdio.h>
#include <stdlib.h>

#define   CLS()         system("clear")
#define   SLEEP(s)      system("sleep " #s)

#define   SIZE_X  20
#define   SIZE_Y  20


/**************************** Mazes ****************************/

unsigned char field[SIZE_X][SIZE_Y] =  { 
                                               'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H','H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H',
                                               'H', ' ', 'H', 'H', ' ', ' ', ' ', ' ', 'H', ' ',' ', ' ', ' ', ' ', ' ', 'H', 'H', 'H', 'H', 'H',
                                               'H', ' ', 'H', 'H', ' ', 'H', 'H', 'H', 'H', ' ','H', 'H', 'H', 'H', ' ', 'H', 'H', 'H', 'H', 'H',
                                               'H', ' ', 'H', 'H', ' ', 'H', 'H', 'H', 'H', ' ','H', 'H', 'H', 'H', ' ', 'H', 'H', 'H', 'H', 'H',
                                               'H', ' ', 'H', 'H', ' ', 'H', 'H', ' ', 'H', ' ','H', 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H',
                                               'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ','H', 'H', ' ', 'H', 'H', 'H', 'H', 'H', ' ', 'H',
                                               'H', ' ', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ','H', 'H', ' ', 'H', ' ', ' ', ' ', 'H', ' ', 'H',
                                               'H', ' ', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ','H', 'H', ' ', 'H', ' ', 'H', ' ', 'H', ' ', 'H',
                                               'H', ' ', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ','H', 'H', ' ', 'H', ' ', 'H', ' ', 'H', ' ', 'H',
                                               'H', ' ', 'H', 'H', ' ', ' ', ' ', ' ', 'H', ' ','H', 'H', ' ', 'H', ' ', 'H', ' ', 'H', ' ', 'H',
                                               'H', ' ', 'H', 'H', ' ', 'H', 'H', ' ', 'H', ' ','H', 'H', ' ', 'H', 'H', 'H', ' ', 'H', ' ', 'H',
                                               'H', ' ', 'H', 'H', ' ', 'H', 'H', ' ', 'H', ' ','H', 'H', ' ', 'H', 'H', 'H', ' ', 'H', ' ', 'H',
                                               'H', ' ', ' ', ' ', ' ', 'H', 'H', ' ', 'H', ' ','H', 'H', ' ', 'H', 'H', 'H', ' ', 'H', ' ', 'H',
                                               'H', ' ', 'H', 'H', ' ', 'H', 'H', ' ', 'H', ' ','H', 'H', ' ', ' ', ' ', ' ', ' ', 'H', ' ', 'H',
                                               'H', ' ', 'H', 'H', ' ', 'H', 'H', ' ', 'H', ' ','H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ', 'H',
                                               'H', ' ', 'H', 'H', ' ', 'H', 'H', ' ', 'H', ' ','H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ', 'H',
                                               'H', ' ', ' ', 'H', ' ', ' ', ' ', ' ', 'H', ' ','H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ', 'H',
                                               'H', 'F', 'H', 'H', 'H', 'H', ' ', 'H', 'H', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H',
                                               'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ','H', 'H', 'H', 'H', 'H', 'H', ' ', 'H', 'H', 'H',
                                               'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H','H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H',
                                           };
                                           
/********************************************************************/

void
printM(unsigned char M[SIZE_X][SIZE_Y])
{
    short i, j;

    int res = CLS();

    for(i = 0; i < SIZE_Y; i++)  {
        for(j = 0; j < SIZE_X; j++)  {
            printf("%3c", M[i][j]);
        }
        printf("\n");
    }
}


unsigned char
intodeep(short i, short j)
{
    if(field[i][j] == 'F')  {
        return 1;
    }

    if(field[i][j] == 'H') {
        return 0;
    }

    if(field[i][j] == 'A') {
        return 0;
    }

    field[i][j] = 'A';
    int res = SLEEP(0.03);

    if(! intodeep(i, j-1))  {
        printM(field);
        if(! intodeep(i-1, j))  {
            printM(field);
            if(! intodeep(i, j+1))  {
                printM(field);
                if(! intodeep(i+1, j))  {
                    printM(field);
                    field[i][j] = ' ';
                    res = SLEEP(0.03);
                    return 0;
                }
            }
        }
    }

}


int
main(int argc, char** argv)
{
    if(! intodeep(1, 1))  {
        printf("\nCannot find exit :(\n");
        return -1;
    } else  {
        printf("\nYeeeeee :3\n");
        printM(field);
    }

    return 0;
}
