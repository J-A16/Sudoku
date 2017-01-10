/**
  Name: Sudoku
  Copyright:
  Author: Jeremy Alexandre
  Date: 5/23/2016
  Description: Sudoku solver that will solve any Sudoku puzzle. Input is
  received through infile stream, from a file named "data.txt". The format
  of the input file is space for an empty square and number for a square with
  that number.

  First it uses certain logic methods to try to solve the puzzle. If that fails,
  it uses brute force to solve the puzzle.

  Here is an example "data.txt" file, copy from after open quotation mark to
  before close quotation. Don't forget to add spaces so you have 9 sections
  to a row:

"53  7
6  195
 98    6
8   6   3
4  8 3  1
7   2   6
 6    28
   419  5
    8  79"

  The following is an example of a puzzle that will cause brute force to be
  applied:

"   74
    1 563
2    94
9     31
  6   8
 32     7
  82    4
371 9
    57   "

*/

#include <cstdlib>
#include <iostream>
#include <cctype>
#include <fstream>
#include <ctime>

using namespace std;

ifstream infile;	//creates an instream file



/******************************************************************************/
/**

 Fills the array from infile, placing numbers for numbers and zeros for
 spaces. When ever a space is encountered, toSolve also get incremented.
 Function then returns true. If there are missing numbers or spaces, sends
 the row and number of missing numbers and/or spaces to cout and returns
 false. If there is an invalid character, sends the invalid character to
 cout and returns false.

**/
/******************************************************************************/



bool fillBoard(int arr[][9], int &toSolve)
{
    int c;

    infile.open("data.txt");	//links infile to
    //data.txt

    //tests to see if infile is open
    if ( !infile.is_open() )
    {
        cout << endl << "ERROR: unable to open infile" << endl;
        system ("PAUSE");
        exit(1);
    }


    /// Fills the array from infile, placing numbers for numbers and zeros for
    /// spaces. When ever a space is encountered, toSolve also get incremented.
    /// If there are missing numbers or spaces, sends the row and
    /// number of missing numbers and/or spaces to cout and returns false.
    /// If there is an invalid character, sends the invalid character to cout
    /// and returns false.
    for (int j = 0; j < 9; j++)
    {
        for (int k = 0; k < 9; k++)
        {
            c = infile.get();
            if (c >= '1' && c <= '9')
            {
                arr[j][k] = c - 48;
            }
            else if (c == ' ')
            {
                arr[j][k] = 0;
                toSolve++;
            }
            else if ((c == 10) || (c == -1))
            {
                cout << "Row " << j + 1 << " missing " << 9 - k
                     << " number(s) and/or space(s)\n";
                return false;
            }
            else
            {
                cout.put(c) << endl;
                cout << "invalid character." << endl;
                return false;
            }
        }// end for (int k = 0; k < 9; k++)

        /// Ignore the endl character
        infile.ignore();

    }// end for (int j = 0; j < 9; j++)

    return true;
}// end bool fillBoard(int arr[][9], int &toSolve)



/******************************************************************************/
/**

 Receives the array filled with the puzzle and attempts to solve it using
 logic. If an answer to a square is discovered it is added to the array. If
 the puzzle is solved, returns true. If the puzzle cannot be solved using
 these logic methods, returns false.

**/
/******************************************************************************/



bool solve(int arr[][9], int &leftToSolve)
{
    bool change, potential[9], elimination, gridPotential[9][9];
    int potentialLeft;

    void display(int [][9]);
    void squareEliminator(bool [][9], int, int);
    void actualAddAndElimPotentialElim(int arr[][9], bool gridPotential[][9],
                                       bool &elimination, bool &change,
                                       int &leftToSolve, int &i);



    do
    {
        change = false;

        for (int j = 0; j < 9; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                if (arr[j][k] == 0)
                {
                    /// Before any numbers are eliminated, there are 9 potential
                    /// numbers, the numbers 1-9
                    for (int i = 0; i < 9; i++)
                        potential[i] = true;

                    potentialLeft = 9;


                    /// Checks the row of the square to see which numbers where
                    /// already used and therefore not a potential number for
                    /// the square
                    for (int i = 0; i < 9; i++)
                    {
                        if (arr[j][i] > 0)
                        {
                            potential[arr[j][i] - 1] = false;

                            potentialLeft--;
                        }// end if (arr[j][i] > 0)

                    }// end for (int i = 0; i < 9; i++)



                    /// Checks the column of the square to see which numbers
                    /// where already used and therefore not a potential number
                    /// for the square. It also checks to make sure the number
                    /// has not already been eliminated.
                    for (int i = 0; i < 9; i++)
                    {
                        if (arr[i][k] > 0)
                            if (potential[arr[i][k] - 1] == true)
                            {
                                potential[arr[i][k] - 1] = false;

                                potentialLeft--;
                            }// end if (arr[i][k] > 0)

                    }// end for (int i = 0; i < 9; i++)



                    /// This section checks within the 3 x 3 grid of the square
                    /// being tested to see which numbers are taken, and
                    /// therefore not a potential number for the square.
                    /// As before, it also checks to make sure the number
                    /// has not already been eliminated.

                    /// Top left 3x3 grid
                    if (j <= 2 && k <=2)
                    {
                        for (int m = 0; m < 3; m++)
                            for (int n = 0; n < 3; n++)
                                if (arr[m][n] > 0)
                                    if (potential[arr[m][n] - 1] == true)
                                    {
                                        potential[arr[m][n] - 1] = false;

                                        potentialLeft--;
                                    }
                    }
                    /// Top center 3x3 grid
                    else if (j <= 2 && (k >= 3 && k <= 5))
                    {
                        for (int m = 0; m < 3; m++)
                            for (int n = 3; n < 6; n++)
                                if (arr[m][n] > 0)
                                    if (potential[arr[m][n] - 1] == true)
                                    {
                                        potential[arr[m][n] - 1] = false;

                                        potentialLeft--;
                                    }
                    }
                    /// Top right 3x3 grid
                    else if (j <= 2 && (k >= 6 && k <= 8))
                    {
                        for (int m = 0; m < 3; m++)
                            for (int n = 6; n < 9; n++)
                                if (arr[m][n] > 0)
                                    if (potential[arr[m][n] - 1] == true)
                                    {
                                        potential[arr[m][n] - 1] = false;

                                        potentialLeft--;
                                    }
                    }
                    /// Middle left 3x3 grid
                    else if ((j >= 3 && j <= 5) && k <=2)
                    {
                        for (int m = 3; m < 6; m++)
                            for (int n = 0; n < 3; n++)
                                if (arr[m][n] > 0)
                                    if (potential[arr[m][n] - 1] == true)
                                    {
                                        potential[arr[m][n] - 1] = false;

                                        potentialLeft--;
                                    }
                    }
                    /// Middle center 3x3 grid
                    else if ((j >= 3 && j <= 5) && (k >= 3 && k <= 5))
                    {
                        for (int m = 3; m < 6; m++)
                            for (int n = 3; n < 6; n++)
                                if (arr[m][n] > 0)
                                    if (potential[arr[m][n] - 1] == true)
                                    {
                                        potential[arr[m][n] - 1] = false;

                                        potentialLeft--;
                                    }
                    }
                    /// Middle right 3x3 grid
                    else if ((j >= 3 && j <= 5) && (k >= 6 && k <= 8))
                    {
                        for (int m = 3; m < 6; m++)
                            for (int n = 6; n < 9; n++)
                                if (arr[m][n] > 0)
                                    if (potential[arr[m][n] - 1] == true)
                                    {
                                        potential[arr[m][n] - 1] = false;

                                        potentialLeft--;
                                    }
                    }
                    /// Bottom left 3x3 grid
                    else if ((j >= 6 && j <= 8) && k <=2)
                    {
                        for (int m = 6; m < 9; m++)
                            for (int n = 0; n < 3; n++)
                                if (arr[m][n] > 0)
                                    if (potential[arr[m][n] - 1] == true)
                                    {
                                        potential[arr[m][n] - 1] = false;

                                        potentialLeft--;
                                    }
                    }
                    /// Bottom center 3x3 grid
                    else if ((j >= 6 && j <= 8) && (k >= 3 && k <= 5))
                    {
                        for (int m = 6; m < 9; m++)
                            for (int n = 3; n < 6; n++)
                                if (arr[m][n] > 0)
                                    if (potential[arr[m][n] - 1] == true)
                                    {
                                        potential[arr[m][n] - 1] = false;

                                        potentialLeft--;
                                    }
                    }
                    /// Bottom right 3x3 grid
                    else
                    {
                        for (int m = 6; m < 9; m++)
                            for (int n = 6; n < 9; n++)
                                if (arr[m][n] > 0)
                                    if (potential[arr[m][n] - 1] == true)
                                    {
                                        potential[arr[m][n] - 1] = false;

                                        potentialLeft--;
                                    }
                    }


                    if (potentialLeft == 1)
                    {
                        for (int i = 0; i < 9; i++)
                            if(potential[i] == true)
                            {
                                arr[j][k] = i + 1;

                                change = true;

                                leftToSolve--;

                                display(arr);

                                cout << leftToSolve << endl;
                            }
                    }


                }// end if (arr[j][k] == 0)

            }//end for (int k = 0; k < 9; k++)

        }// end for (int j = 0; j < 9; j++)



        for (int i = 1; i <= 9; i++)
        {

            /// Initialize all the squares to false if they are occupied
            /// and true if they are empty
            for (int m = 0; m < 9; m++)
                for (int n = 0; n < 9; n++)
                    if (arr[m][n] > 0)
                    {
                        gridPotential[m][n] = false;
                    }
                    else
                    {
                        gridPotential[m][n] = true;
                    }




            for (int j = 0; j < 9; j++)
                for (int k = 0; k < 9; k++)
                {

                    if (arr[j][k] == i)
                    {
                        /// Eliminates the column, row and 3x3 grid the
                        /// number occupies
                        squareEliminator(gridPotential, j, k);



                        /// This row has already been eliminated, start at the
                        /// beginning of the next row. k is negative one
                        /// because it will be incremented to 0 at the start of
                        /// it's for loop. If j equals 8, incrementing j would
                        /// cause the next set of test parameters to be
                        /// 9 and 0 which is outside the range of the array
                        if (j != 8)
                        {
                            k = -1;
                            j++;
                        }
                    }// end if (arr[j][k] == i)
                }


            do
            {
                elimination = false;

                actualAddAndElimPotentialElim(arr, gridPotential, elimination,
                                              change, leftToSolve, i);

            }
            while (elimination);

        }// end for (int i = 1; i <= 9; i++)


    }
    while (change);

    if (leftToSolve == 0)
    {
        cout << "Solved!" << endl;
        return true;
    }
    else
    {
        cout << "Could not solve!" << endl;
        return false;
    }

}// end bool solve(int arr[][9], int &leftToSolve)



/******************************************************************************/
/**

 Displays the puzzle.

**/
/******************************************************************************/



void display(int arr[][9])
{
    cout << "=========================" << endl;

    for (int j = 0; j < 9; j++)
    {
        cout << "~ ";
        for (int k = 0; k < 9; k++)
        {
            if (arr[j][k] == 0)
                cout << " ";
            else
                cout << arr[j][k];
            if ((k % 3) != 2)
                cout << "|";
            else if (k != 8 )
                cout << " ~ ";
            else
                cout << " ~";
        }// end for (int k = 0; k < 9; k++)

        cout << endl;

        if ((j % 3) != 2)
            cout << "-------------------------" << endl;
        else cout << "=========================" << endl;

    }// end for (int j = 0; j < 3; j++)


}



/******************************************************************************/
/**

 Eliminates the column, row and 3x3 square the number occupies

**/
/******************************************************************************/



void squareEliminator(bool gridPotential[][9], int j, int k)
{
    /// Eliminates both the column and row the number
    /// occupies
    for (int m = 0; m < 9; m++)
    {
        gridPotential[j][m] = false;
        gridPotential[m][k] = false;
    }

    /// Eliminates the 3x3 grid the number occupies

    /// Top left 3x3 grid
    if (j <= 2 && k <=2)
    {
        for (int m = 0; m < 3; m++)
            for (int n = 0; n < 3; n++)
                gridPotential[m][n] = false;
    }
    /// Top center 3x3 grid
    else if (j <= 2 && (k >= 3 && k <= 5))
    {
        for (int m = 0; m < 3; m++)
            for (int n = 3; n < 6; n++)
                gridPotential[m][n] = false;
    }
    /// Top right 3x3 grid
    else if (j <= 2 && (k >= 6 && k <= 8))
    {
        for (int m = 0; m < 3; m++)
            for (int n = 6; n < 9; n++)
                gridPotential[m][n] = false;
    }
    /// Middle left 3x3 grid
    else if ((j >= 3 && j <= 5) && k <=2)
    {
        for (int m = 3; m < 6; m++)
            for (int n = 0; n < 3; n++)
                gridPotential[m][n] = false;
    }
    /// Middle center 3x3 grid
    else if ((j >= 3 && j <= 5) && (k >= 3 && k <= 5))
    {
        for (int m = 3; m < 6; m++)
            for (int n = 3; n < 6; n++)
                gridPotential[m][n] = false;
    }
    /// Middle right 3x3 grid
    else if ((j >= 3 && j <= 5) && (k >= 6 && k <= 8))
    {
        for (int m = 3; m < 6; m++)
            for (int n = 6; n < 9; n++)
                gridPotential[m][n] = false;
    }
    /// Bottom left 3x3 grid
    else if ((j >= 6 && j <= 8) && k <=2)
    {
        for (int m = 6; m < 9; m++)
            for (int n = 0; n < 3; n++)
                gridPotential[m][n] = false;
    }
    /// Bottom center 3x3 grid
    else if ((j >= 6 && j <= 8) && (k >= 3 && k <= 5))
    {
        for (int m = 6; m < 9; m++)
            for (int n = 3; n < 6; n++)
                gridPotential[m][n] = false;
    }
    /// Bottom right 3x3 grid
    else
    {
        for (int m = 6; m < 9; m++)
            for (int n = 6; n < 9; n++)
                gridPotential[m][n] = false;
    }
}

/******************************************************************************/
/**

 If there is only one potential space a number can occupy within a 3x3 grid,
 adds that number to the array and deletes that numbers row, column and 3x3
 grid from the list of potential squares. If there are 2 potential spaces
 within a 3x3 grid and they line up on a row or column, that number can only
 occupy that row or column within that 3x3 grid, therefore that row or column
 is eliminated.

**/
/******************************************************************************/

void actualAddAndElimPotentialElim(int arr[][9], bool gridPotential[][9],
                                   bool &elimination, bool &change,
                                   int &leftToSolve, int &i)
{
    void display(int [][9]);
    void squareEliminator(bool [][9], int, int);

    int potentialSpaces = 0;


    /// Top left 3x3 grid
    for (int m = 0; m < 3; m++)
        for (int n = 0; n < 3; n++)
            if (gridPotential[m][n] == true)
            {
                potentialSpaces++;
            }

    if (potentialSpaces == 1)
    {
        for (int m = 0; m < 3; m++)
            for (int n = 0; n < 3; n++)
                if (gridPotential[m][n] == true)
                {
                    arr[m][n] = i;

                    change = true;

                    leftToSolve--;

                    squareEliminator(gridPotential, m, n);

                    elimination = true;

                    display(arr);

                    cout << leftToSolve << endl;

                }

    }// end if (potentialSpaces == 1)
    else if (potentialSpaces == 2)
    {
        potentialSpaces = 0;

        /// If the 2 potential spaces line up in a row within a
        /// 3x3 grid, that row is eliminated because it is already
        /// taken by the number, i, in that 3x3 grid
        for (int m = 0; m < 3; m++)
        {
            for (int n = 0; n < 3; n++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[m][j] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }



        /// If the 2 potential spaces line up in a column within a
        /// 3x3 grid, that column is eliminated because it is
        /// already taken by the number, i, in that 3x3 grid
        for (int n = 0; n < 3; n++)
        {
            for (int m = 0; m < 3; m++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[j][n] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }

    }// end else if (potentialSpaces == 2)



    potentialSpaces = 0;



    /// Top center 3x3 grid
    for (int m = 0; m < 3; m++)
        for (int n = 3; n < 6; n++)
            if (gridPotential[m][n] == true)
            {
                potentialSpaces++;
            }

    if (potentialSpaces == 1)
    {
        for (int m = 0; m < 3; m++)
            for (int n = 3; n < 6; n++)
                if (gridPotential[m][n] == true)
                {
                    arr[m][n] = i;

                    change = true;

                    leftToSolve--;

                    squareEliminator(gridPotential, m, n);

                    elimination = true;

                    display(arr);

                    cout << leftToSolve << endl;

                }

    }// end if (potentialSpaces == 1)
    else if (potentialSpaces == 2)
    {
        potentialSpaces = 0;

        /// If the 2 potential spaces line up in a row within a
        /// 3x3 grid, that row is eliminated because it is already
        /// taken by the number, i, in that 3x3 grid
        for (int m = 0; m < 3; m++)
        {
            for (int n = 3; n < 6; n++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[m][j] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }



        /// If the 2 potential spaces line up in a column within a
        /// 3x3 grid, that column is eliminated because it is
        /// already taken by the number, i, in that 3x3 grid
        for (int n = 3; n < 6; n++)
        {
            for (int m = 0; m < 3; m++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[j][n] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }

    }// end else if (potentialSpaces == 2)



    potentialSpaces = 0;



    /// Top right 3x3 grid
    for (int m = 0; m < 3; m++)
        for (int n = 6; n < 9; n++)
            if (gridPotential[m][n] == true)
            {
                potentialSpaces++;
            }

    if (potentialSpaces == 1)
    {
        for (int m = 0; m < 3; m++)
            for (int n = 6; n < 9; n++)
                if (gridPotential[m][n] == true)
                {
                    arr[m][n] = i;

                    change = true;

                    leftToSolve--;

                    squareEliminator(gridPotential, m, n);

                    elimination = true;

                    display(arr);

                    cout << leftToSolve << endl;

                }

    }// end if (potentialSpaces == 1)
    else if (potentialSpaces == 2)
    {
        potentialSpaces = 0;

        /// If the 2 potential spaces line up in a row within a
        /// 3x3 grid, that row is eliminated because it is already
        /// taken by the number, i, in that 3x3 grid
        for (int m = 0; m < 3; m++)
        {
            for (int n = 6; n < 9; n++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[m][j] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }



        /// If the 2 potential spaces line up in a column within a
        /// 3x3 grid, that column is eliminated because it is
        /// already taken by the number, i, in that 3x3 grid
        for (int n = 6; n < 9; n++)
        {
            for (int m = 0; m < 3; m++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[j][n] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }

    }// end else if (potentialSpaces == 2)



    potentialSpaces = 0;



    /// Middle left 3x3 grid
    for (int m = 3; m < 6; m++)
        for (int n = 0; n < 3; n++)
            if (gridPotential[m][n] == true)
            {
                potentialSpaces++;
            }

    if (potentialSpaces == 1)
    {
        for (int m = 3; m < 6; m++)
            for (int n = 0; n < 3; n++)
                if (gridPotential[m][n] == true)
                {
                    arr[m][n] = i;

                    change = true;

                    leftToSolve--;

                    squareEliminator(gridPotential, m, n);

                    elimination = true;

                    display(arr);

                    cout << leftToSolve << endl;

                }

    }// end if (potentialSpaces == 1)
    else if (potentialSpaces == 2)
    {
        potentialSpaces = 0;

        /// If the 2 potential spaces line up in a row within a
        /// 3x3 grid, that row is eliminated because it is already
        /// taken by the number, i, in that 3x3 grid
        for (int m = 3; m < 6; m++)
        {
            for (int n = 0; n < 3; n++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[m][j] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }



        /// If the 2 potential spaces line up in a column within a
        /// 3x3 grid, that column is eliminated because it is
        /// already taken by the number, i, in that 3x3 grid
        for (int n = 0; n < 3; n++)
        {
            for (int m = 3; m < 6; m++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[j][n] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }

    }// end else if (potentialSpaces == 2)



    potentialSpaces = 0;



    /// Middle center 3x3 grid
    for (int m = 3; m < 6; m++)
        for (int n = 3; n < 6; n++)
            if (gridPotential[m][n] == true)
            {
                potentialSpaces++;
            }

    if (potentialSpaces == 1)
    {
        for (int m = 3; m < 6; m++)
            for (int n = 3; n < 6; n++)
                if (gridPotential[m][n] == true)
                {
                    arr[m][n] = i;

                    change = true;

                    leftToSolve--;

                    squareEliminator(gridPotential, m, n);

                    elimination = true;

                    display(arr);

                    cout << leftToSolve << endl;

                }

    }// end if (potentialSpaces == 1)
    else if (potentialSpaces == 2)
    {
        potentialSpaces = 0;

        /// If the 2 potential spaces line up in a row within a
        /// 3x3 grid, that row is eliminated because it is already
        /// taken by the number, i, in that 3x3 grid
        for (int m = 3; m < 6; m++)
        {
            for (int n = 3; n < 6; n++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[m][j] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }



        /// If the 2 potential spaces line up in a column within a
        /// 3x3 grid, that column is eliminated because it is
        /// already taken by the number, i, in that 3x3 grid
        for (int n = 3; n < 6; n++)
        {
            for (int m = 3; m < 6; m++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[j][n] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }

    }// end else if (potentialSpaces == 2)



    potentialSpaces = 0;



    /// Middle right 3x3 grid
    for (int m = 3; m < 6; m++)
        for (int n = 6; n < 9; n++)
            if (gridPotential[m][n] == true)
            {
                potentialSpaces++;
            }

    if (potentialSpaces == 1)
    {
        for (int m = 3; m < 6; m++)
            for (int n = 6; n < 9; n++)
                if (gridPotential[m][n] == true)
                {
                    arr[m][n] = i;

                    change = true;

                    leftToSolve--;

                    squareEliminator(gridPotential, m, n);

                    elimination = true;

                    display(arr);

                    cout << leftToSolve << endl;

                }

    }// end if (potentialSpaces == 1)
    else if (potentialSpaces == 2)
    {
        potentialSpaces = 0;

        /// If the 2 potential spaces line up in a row within a
        /// 3x3 grid, that row is eliminated because it is already
        /// taken by the number, i, in that 3x3 grid
        for (int m = 3; m < 6; m++)
        {
            for (int n = 6; n < 9; n++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[m][j] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }



        /// If the 2 potential spaces line up in a column within a
        /// 3x3 grid, that column is eliminated because it is
        /// already taken by the number, i, in that 3x3 grid
        for (int n = 6; n < 9; n++)
        {
            for (int m = 3; m < 6; m++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[j][n] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }

    }// end else if (potentialSpaces == 2)



    potentialSpaces = 0;



    /// Bottom left 3x3 grid
    for (int m = 6; m < 9; m++)
        for (int n = 0; n < 3; n++)
            if (gridPotential[m][n] == true)
            {
                potentialSpaces++;
            }

    if (potentialSpaces == 1)
    {
        for (int m = 6; m < 9; m++)
            for (int n = 0; n < 3; n++)
                if (gridPotential[m][n] == true)
                {
                    arr[m][n] = i;

                    change = true;

                    leftToSolve--;

                    squareEliminator(gridPotential, m, n);

                    elimination = true;

                    display(arr);

                    cout << leftToSolve << endl;

                }

    }// end if (potentialSpaces == 1)
    else if (potentialSpaces == 2)
    {
        potentialSpaces = 0;

        /// If the 2 potential spaces line up in a row within a
        /// 3x3 grid, that row is eliminated because it is already
        /// taken by the number, i, in that 3x3 grid
        for (int m = 6; m < 9; m++)
        {
            for (int n = 0; n < 3; n++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[m][j] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }



        /// If the 2 potential spaces line up in a column within a
        /// 3x3 grid, that column is eliminated because it is
        /// already taken by the number, i, in that 3x3 grid
        for (int n = 0; n < 3; n++)
        {
            for (int m = 6; m < 9; m++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[j][n] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }

    }// end else if (potentialSpaces == 2)



    potentialSpaces = 0;



    /// Bottom center 3x3 grid
    for (int m = 6; m < 9; m++)
        for (int n = 3; n < 6; n++)
            if (gridPotential[m][n] == true)
            {
                potentialSpaces++;
            }

    if (potentialSpaces == 1)
    {
        for (int m = 6; m < 9; m++)
            for (int n = 3; n < 6; n++)
                if (gridPotential[m][n] == true)
                {
                    arr[m][n] = i;

                    change = true;

                    leftToSolve--;

                    squareEliminator(gridPotential, m, n);

                    elimination = true;

                    display(arr);

                    cout << leftToSolve << endl;

                }

    }// end if (potentialSpaces == 1)
    else if (potentialSpaces == 2)
    {
        potentialSpaces = 0;

        /// If the 2 potential spaces line up in a row within a
        /// 3x3 grid, that row is eliminated because it is already
        /// taken by the number, i, in that 3x3 grid
        for (int m = 6; m < 9; m++)
        {
            for (int n = 3; n < 6; n++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[m][j] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }



        /// If the 2 potential spaces line up in a column within a
        /// 3x3 grid, that column is eliminated because it is
        /// already taken by the number, i, in that 3x3 grid
        for (int n = 3; n < 6; n++)
        {
            for (int m = 6; m < 9; m++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[j][n] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }

    }// end else if (potentialSpaces == 2)



    potentialSpaces = 0;



    /// Bottom right 3x3 grid
    for (int m = 6; m < 9; m++)
        for (int n = 6; n < 9; n++)
            if (gridPotential[m][n] == true)
            {
                potentialSpaces++;
            }

    if (potentialSpaces == 1)
    {
        for (int m = 6; m < 9; m++)
            for (int n = 6; n < 9; n++)
                if (gridPotential[m][n] == true)
                {
                    arr[m][n] = i;

                    change = true;

                    leftToSolve--;

                    squareEliminator(gridPotential, m, n);

                    elimination = true;

                    display(arr);

                    cout << leftToSolve << endl;

                }

    }// end if (potentialSpaces == 1)
    else if (potentialSpaces == 2)
    {
        potentialSpaces = 0;

        /// If the 2 potential spaces line up in a row within a
        /// 3x3 grid, that row is eliminated because it is already
        /// taken by the number, i, in that 3x3 grid
        for (int m = 6; m < 9; m++)
        {
            for (int n = 6; n < 9; n++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[m][j] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }



        /// If the 2 potential spaces line up in a column within a
        /// 3x3 grid, that column is eliminated because it is
        /// already taken by the number, i, in that 3x3 grid
        for (int n = 6; n < 9; n++)
        {
            for (int m = 6; m < 9; m++)
            {
                if (gridPotential[m][n] == true)
                {
                    potentialSpaces++;

                    if (potentialSpaces == 2)
                    {
                        for (int j = 0; j < 9; j++)
                        {
                            gridPotential[j][n] = false;

                            elimination = true;
                        }// end for (int j = 0; j < 9; j++)
                    }// end if (potentialSpaces == 2)
                }// end if (gridPotential[m][n] == true)
            }

            potentialSpaces = 0;
        }

    }// end else if (potentialSpaces == 2)



}// end void actualAddAndElimPotentialElim(int arr[][9],
//                                        bool gridPotential[][9],
//                                        bool &elimination, bool &change,
//                                        int &leftToSolve)



/******************************************************************************/
/**

 Applies brute force to the array to solve it. Will solve any puzzle.

 At each empty square, takes a randomly selected potential number of
 that square and tests to see if it was already used in the row,
 column or 3x3 grid. If it was not used, adds it to the square and goes
 to the next empty square. If it was used, picks another randomly
 selected potential number of that square and tests it. If all the
 potential numbers of an empty square are already taken, a new brute
 force attempt is started. Brute force is applied until the puzzle
 is solved.

**/
/******************************************************************************/



bool bruteForce(int arr[][9])
{
    int numberOfPotentials[9][9], potentialNumbers[9][9][9], randomPotential,
        arrCopy[9][9], numbersTriedCount;
    bool gridPotential[9][9], repeat, usedNumbers[9], taken, run;

    void squareEliminator(bool [][9], int, int);
    void display(int [][9]);


    for (int m = 0; m < 9; m++)
        for (int n = 0; n < 9; n++)
        {
            numberOfPotentials[m][n] = 0;
        }




    /// Takes each number and checks for all the rows, columns and 3x3 grids
    /// that are already occupied by it on the board. It then adds that
    /// number to the next available slot of all other squares in a 3
    /// dimensional array and keeps track of how many potential numbers are on
    /// each square.
    for (int i = 1; i <= 9; i++)
    {

        /// Initialize all the squares to false if they are occupied
        /// and true if they are empty
        for (int m = 0; m < 9; m++)
            for (int n = 0; n < 9; n++)
                if (arr[m][n] > 0)
                {
                    gridPotential[m][n] = false;
                }
                else
                {
                    gridPotential[m][n] = true;
                }




        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++)
            {

                if (arr[j][k] == i)
                {
                    /// Eliminates the column, row and 3x3 grid the
                    /// number occupies
                    squareEliminator(gridPotential, j, k);



                    /// This row has already been eliminated, start at the
                    /// beginning of the next row. k is negative one
                    /// because it will be incremented to 0 at the start of
                    /// it's for loop. If j equals 8, incrementing j would
                    /// cause the next set of test parameters to be
                    /// 9 and 0 which is outside the range of the array
                    if (j != 8)
                    {
                        k = -1;
                        j++;
                    }
                }// end if (arr[j][k] == i)
            }




        /// Adds the number to the next available slot on each square that was
        /// not part of a row, column or 3x3 grid that was already occupied
        /// by the number and increments the count for each square the number
        /// is added to
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++)
                if (gridPotential[j][k] == true)
                {
                    potentialNumbers[j][k][numberOfPotentials[j][k]] = i;

                    numberOfPotentials[j][k]++;
                }



    }// end for (int i = 1; i <= 9; i++)

    cout << endl;
    cout << "Brute force being applied, please wait..." << endl << endl;



    /// Seed random number generator with current time
    srand(time(0));



    /// At each empty square, takes a randomly selected potential number of
    /// that square and tests to see if it was already used in the row,
    /// column or 3x3 grid. If it was not used, adds it to the square and goes
    /// to the next empty square. If it was used, picks another randomly
    /// selected potential number of that square and tests it. If all the
    /// potential numbers of an empty square are already taken, a new brute
    /// force attempt is started. Brute force is applied until the puzzle
    /// is solved.
    for (int j = 0; j < 9; j++)
    {
        for (int k = 0; k < 9; k++)
        {

            /// Initializes arrCopy to arr's state at the start of each brute
            /// force attempt
            if (j == 0 && k == 0)
            {
                for (int m = 0; m < 9; m++)
                    for (int n = 0; n < 9; n++)
                    {
                        arrCopy[m][n] = arr[m][n];
                    }

            }// end if (j == 0 && k == 0)



            if (arrCopy[j][k] == 0)
            {
                for (int m = 0; m < 9; m++)
                {
                    usedNumbers[m] = false;
                }

                repeat = false;

                numbersTriedCount = 0;

                do
                {

                    do
                    {
                        randomPotential =
                            potentialNumbers
                            [j][k][rand() % numberOfPotentials[j][k]];

                        if (usedNumbers[randomPotential - 1] == false)
                        {
                            repeat = false;

                            run = true;

                            usedNumbers[randomPotential - 1] = true;

                            numbersTriedCount++;


                        }// end if (usedNumbers[randomPotential - 1] == false)
                        else
                        {
                            repeat = true;

                            /// Ends this brute force attempt
                            if (numbersTriedCount == numberOfPotentials[j][k])
                            {
                                j = 0;

                                k = -1;

                                /// Breaks out of the do while loops

                                repeat = false;

                                run = false;

                                taken = false;
                            }
                        }

                    }
                    while(repeat);


                    if (run)
                    {


                        taken = false;

                        /// Checks the row of the square to see if the number
                        /// was already used
                        for (int i = 0; i < 9; i++)
                        {
                            if (arrCopy[j][i] == randomPotential)
                            {
                                taken = true;
                            }// end if (arrCopy[j][i] == randomPotential)

                        }// end for (int i = 0; i < 9; i++)


                        if (!taken)
                        {

                            /// Checks the column of the square to see if the
                            /// number was already used
                            for (int i = 0; i < 9; i++)
                            {
                                if (arrCopy[i][k] == randomPotential)
                                {
                                    taken = true;

                                }// end if (arrCopy[i][k] == randomPotential)

                            }// end for (int i = 0; i < 9; i++)



                            if (!taken)
                            {

                                /// This section checks within the 3 x 3 grid
                                /// of the square being tested to see if the
                                /// number was already used

                                /// Top left 3x3 grid
                                if (j <= 2 && k <=2)
                                {
                                    for (int m = 0; m < 3; m++)
                                        for (int n = 0; n < 3; n++)
                                            if
                                            (arrCopy[m][n] == randomPotential)
                                            {
                                                taken = true;

                                            }
                                }
                                /// Top center 3x3 grid
                                else if (j <= 2 && (k >= 3 && k <= 5))
                                {
                                    for (int m = 0; m < 3; m++)
                                        for (int n = 3; n < 6; n++)
                                            if
                                            (arrCopy[m][n] == randomPotential)
                                            {
                                                taken = true;

                                            }
                                }
                                /// Top right 3x3 grid
                                else if (j <= 2 && (k >= 6 && k <= 8))
                                {
                                    for (int m = 0; m < 3; m++)
                                        for (int n = 6; n < 9; n++)
                                            if
                                            (arrCopy[m][n] == randomPotential)
                                            {
                                                taken = true;

                                            }
                                }
                                /// Middle left 3x3 grid
                                else if ((j >= 3 && j <= 5) && k <=2)
                                {
                                    for (int m = 3; m < 6; m++)
                                        for (int n = 0; n < 3; n++)
                                            if
                                            (arrCopy[m][n] == randomPotential)
                                            {
                                                taken = true;

                                            }
                                }
                                /// Middle center 3x3 grid
                                else if ((j >= 3 && j <= 5) &&
                                         (k >= 3 && k <= 5))
                                {
                                    for (int m = 3; m < 6; m++)
                                        for (int n = 3; n < 6; n++)
                                            if
                                            (arrCopy[m][n] == randomPotential)
                                            {
                                                taken = true;

                                            }
                                }
                                /// Middle right 3x3 grid
                                else if ((j >= 3 && j <= 5) &&
                                         (k >= 6 && k <= 8))
                                {
                                    for (int m = 3; m < 6; m++)
                                        for (int n = 6; n < 9; n++)
                                            if
                                            (arrCopy[m][n] == randomPotential)
                                            {
                                                taken = true;

                                            }
                                }
                                /// Bottom left 3x3 grid
                                else if ((j >= 6 && j <= 8) && k <=2)
                                {
                                    for (int m = 6; m < 9; m++)
                                        for (int n = 0; n < 3; n++)
                                            if
                                            (arrCopy[m][n] == randomPotential)
                                            {
                                                taken = true;

                                            }
                                }
                                /// Bottom center 3x3 grid
                                else if ((j >= 6 && j <= 8) &&
                                         (k >= 3 && k <= 5))
                                {
                                    for (int m = 6; m < 9; m++)
                                        for (int n = 3; n < 6; n++)
                                            if
                                            (arrCopy[m][n] == randomPotential)
                                            {
                                                taken = true;

                                            }
                                }
                                /// Bottom right 3x3 grid
                                else
                                {
                                    for (int m = 6; m < 9; m++)
                                        for (int n = 6; n < 9; n++)
                                            if
                                            (arrCopy[m][n] == randomPotential)
                                            {
                                                taken = true;

                                            }
                                }



                                /// If the number is not taken in the row,
                                /// column, or 3x3 grid, it is placed on
                                /// the square
                                if (!taken)
                                {
                                    arrCopy[j][k] = randomPotential;
                                }


                            }// end if (!taken)

                        }// end if (!taken)

                    }// end if (run)

                }
                while (taken);

            }// end if (arrCopy[j][k] == 0)

        }// end for (int k = 0; k < 9; k++)
    }// end for (int j = 0; j < 9; j++)

    display(arrCopy);

    return true;

}// end bool bruteForce(int arr[][9])

/******************************************************************************/

int main()
{
    int board[9][9], numToSolve = 0, startingFilled;

    bool fillBoard(int [][9], int &);
    bool solve(int [][9], int &);
    bool bruteForce(int [][9]);


    if(!fillBoard(board, numToSolve))
    {
        cout << "what?" << endl;
    }
    else
    {
        startingFilled = 81 - numToSolve;

        if(!solve(board, numToSolve))
        {
            cout << "Oh, no!" << endl;

            if (bruteForce(board))
                cout << "Brute force for the win!" << endl << endl;
        }

        cout << "Number filled at start: " << startingFilled << endl;
    }


    return 0;
}
