/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// constants
#define DIM_MIN 3
#define DIM_MAX 9
#define MAX 1000
// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;
//34125876412587641241235476123748648578564567865478
// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen dxd\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

         //check for win
        if (won())
        {
            printf("Bloody Hell You Won!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tiles[MAX];
        int tile;
        string number= get_string("Enter String: ");
        if(strlen(number)>1000)
        {
            printf("too many inputs, replay!\n");
            return 3;
        }

        else if (strlen(number)==2)
        {
                tile = atoi(number);
                printf("%i\n",tile);
                fprintf(file, "%i\n", tile);
                fflush(file);

                // move if possible, else report illegality
                if (!move(tile))
                {
                    printf("\nIllegal move.\n");
                    usleep(500000);
                }
                else
                {
                    printf("nice!\n");
                }

        }
        else
        {
            for (int i=0, n= strlen(number); i<n; i++)
            {
                tiles[i] = putchar(number[i])-'0';
                printf("\n");

                tile=tiles[i];
                fprintf(file, "%i\n", tile);
                fflush(file);

                // move if possible, else report illegality
                if (!move(tile))
                {
                    printf("\nIllegal move.\n");
                    usleep(500000);
                }
                else
                {
                    printf("nice!\n");
                }

            }
        }
        if (tile == 0)
                {
                    break;
                }

                // log move (for testing)
                //fprintf(file, "%i\n", tile);
                //fflush(file);

                // move if possible, else report illegality
                //if (!move(tile))
                //{
                   // printf("\nIllegal move.\n");
                    //usleep(500000);
                //}
                //else
                //{
                    //printf("nice!\n");
                //}



        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int c=1;
    //for iterating over the i rows.
    for (int i=0; i<d; i++)
    {// for iterating over the j rows
        for (int j=0; j<d; j++)
        {//for usage of c to fill up the boxes of the board with correct numbers leaving the last one out.
        board[i][j]= d*d-c;//formaulae to insert values within the box.
        c++;

        }
    }//the last character is given a number even though is wud be hidden bu cant be left black as board is int.
    board[d-1][d-1]=0;
    //for the board having even d*d we have 1 and 2 swapped.
    if ( d*d%2 == 0)
    {
        board[d-1][d-2]=2;
        board[d-1][d-3]=1;
    }

}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {    if (board[i][j] == 0)
            {
            // print the blank tile
                printf(" _ ");
            }
            else
            {    // the tiles in descending order
                printf("%2d ", board[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    // establish algorithm for checking bordering tiles
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                //check if the blank is to the right
                if (j+1 <= d-1 && board[i][j+1] == 0)
                {
                    board[i][j+1] = tile;
                    board[i][j] = 0;
                    return true;
                }
                // check if the blank is to the left
                else if (j-1 >= 0 && board[i][j-1] == 0)
                {
                    board[i][j-1] = tile;
                    board[i][j] = 0;
                    return true;
                }
                // check if the blank is above
                else if (i-1 >= 0 && board[i-1][j] == 0)
                {
                    board[i-1][j] = tile;
                    board[i][j] = 0;
                    return true;
                }
                // check if the blank is below
                else if (i+1 <= d-1 && board[i+1][j] == 0)
                {
                    board[i+1][j] = tile;
                    board[i][j] = 0;
                    return true;
                }

            }
        }
    }
return false;
}


/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    // check if board is sorted
    int n = 1;

    // check to see if last tile is blank and return false if it is not
    if (board[d-1][d-1] != 0)
        return false;

    // set up for loops to check if numbers are in ascending order beginning with 1
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // check last grid position first, if blank, return true
            if (i == d - 1 && j == d - 1)
            {
                return true;
            }

            // check the numbers on the rest of the tiles
            if (n != board[i][j])
            {
                return false;
            }
            n++;
        }
    }

    return false;
}

