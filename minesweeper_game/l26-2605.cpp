// created by : Tuba (l26-2605), Arfa (l26-2601) and Shumaem(l26-2602)

#include <iostream>
#include <ctime>                 //for time()
#include <cstdlib>               //for rand() ,srand()
using namespace std;

//%%%%%%%%%%%%%%% PLAYER BOARD:ALL CELLS CLOSED %%%%%%%%%%%%%%%%%%%%%%%

void visibleBoard(char vBoard[][9])
{
	for (int i = 0;i < 9;i++)
	{

		for (int j = 0;j < 9;j++)
		{
			vBoard[i][j] = '#';
		}
	}

}

// %%%%%%%%%%%%%%%%%% TO DISPLAY THE BOARD WITH ROW COLUM NUMBERS %%%%%%%%%%%%%

void displayboard(char vBoard[][9])
{
	cout << "\n";
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (vBoard[i][j] >= 0)
			{
				cout << " ";
			}
			cout << vBoard[i][j] << " ";
		}
		cout << endl;
	}
}

// %%%%%%%%%%%%%%%%% HIDDEN BOARD; ALL ZEROES %%%%%%%%%%%%%%%%%%%

void makeBoard(int board[][9])
{
	for (int i = 0; i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			board[i][j] = 0;
		}
	}
}

// %%%%%%%%%%%%%%%%% PLACE MINES %%%%%%%%%%%%%%%%

void placeMines(int board[][9], int mines, int saferow, int safecol)
{
	int n = 0;
	while (n < mines)
	{
		int x = rand() % 9;                 //for random cell
		int y = rand() % 9;
		if (x == saferow && y == safecol)
			continue;                       // saferow and safecol(the first click) cannot be a mine
		if (board[x][y] != -1)              // if there is altready mine , skip it
		{
			board[x][y] = -1;
			n++;
		}
	}
}

// %%%%%%%%%%%%%%%%%%% TO SHOW ALL MINES WHEN GAME IS OVER %%%%%%%%%%%%%%%%

void showallmines(int board[9][9], char vBoard[9][9])
{
	for (int i = 0;i < 9;i++)
	{
		for (int j = 0;j < 9;j++)
		{
			if (board[i][j] == -1)
			{
				vBoard[i][j] = '*';
			}
		}
	}
}

// %%%%%%%%%%%% TO COUNT MINES THAT EACH CELLS HAVE IN ITS SURROUNDINGS %%%%%%%%%%%%

void minesCount(int board[][9])
{
	for (int r = 0;r < 9;r++)
	{
		for (int c = 0;c < 9;c++)
		{
			if (board[r][c] != -1)                                   //if cell is not a mine
			{
				int count = 0;
				for (int i = r - 1;i <= r + 1;i++)                   // checking mines in neighbours
				{
					for (int j = c - 1;j <= c + 1;j++)
					{
						if (i >= 0 && i < 9 && j >= 0 && j < 9 && board[i][j] == -1)
						{                                              //if it is -1(mine) and underboundary then count++
							count++;

						}
					}
				}
				board[r][c] = count;                                  //put the mine number(cell has in its surroundings)in that cell
			}
		}
	}
}

// %%%%%%%%%%%%%%%% TO OPEN THE CELL %%%%%%%%%%%%%%%%%%%%%%%%

void openCell(char vBoard[][9], const int board[][9], int row, int col, int& safeleft)
{
	if (row < 0 || row >= 9 || col < 0 || col >= 9)                   //outside boundary
		return;

	if (vBoard[row][col] != '#')                                      //if already opened or flaged
		return;

	vBoard[row][col] = '0' + board[row][col];
	safeleft--;

	// If open cell is zero (no mine in neighbour), open its neighbour cells too
	if (board[row][col] == 0)
	{
		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; ++j)
			{
				if (i != 0 || j != 0)                                // leave the cell own(0,0) position
				{
					int r = row + i;
					int c = col + j;
					openCell(vBoard, board, r, c, safeleft);
				}

			}
		}
	}

}

void play(int board[][9], char vBoard[][9], int totalcells, int mines)
{
	int row;
	int col;
	int safeleft = totalcells - mines;
	bool firstmove = true;

	while (true)
	{
		displayboard(vBoard);
		cout << "enter row and column(0-8): ";                     // cell user want to click
		cin >> row >> col;

		if (row < 0 || row >= 9 || col < 0 || col >= 9)
		{
			cout << "OUTSIDE THE BOARD!" << endl;
			continue;
		}

		char choice;
		cout << "o=OPEN ; f=FLAG:";
		cin >> choice;

		//selected cell can be close,already flagged or open

		// if player selects flag option
		if (choice == 'f')
		{
			if (vBoard[row][col] == '#')         //flag the close cell
			{
				vBoard[row][col] = 'f';
			}
			else if (vBoard[row][col] == 'f')      //remove the flag
			{
				vBoard[row][col] = '#';
			}
			else
			{
				cout << " CANNOT FLAG AN OPEN CELL!" << endl;
			}
		}
		else if (choice == 'o')                   //if user want to open cell
		{

			if (vBoard[row][col] == 'f')
			{
				cout << "REMOVE THE FLAG FIRST!" << endl;
			}
			else if (vBoard[row][col] != '#')
			{
				cout << "ALREADY OPENED!" << endl;
			}
			else
			{
				if (firstmove)                                    //first move always safe
				{
					placeMines(board, mines, row, col);             //after first move,place mines in board
					minesCount(board);                           //count mines around cells
					firstmove = false;
				}

				if (choice == 'o' && board[row][col] == -1 && vBoard[row][col] != 'f')
				{
					cout << "GAME OVER" << endl;
					showallmines(board, vBoard);                 //if game over ,display all mines
					displayboard(vBoard);
					return;
				}

				openCell(vBoard, board, row, col, safeleft);

				if (safeleft == 0)                                //total 71 safe cells---with each safe open ,safeleft cell decreases
				{
					cout << "YOU WIN!" << endl;
					displayboard(vBoard);
					return;
				}
			}
		}
		else
		{
			cout << "INVALID CHOICE.USE O OR F:" << endl;
		}
	}
}

int main()
{
	srand(time(0));                      //to place random mines(according to current time)
	int board[9][9];
	char vBoard[9][9];
	int rows = 9, cols = 9, mines;
	int totalcells = rows * cols;

	cout << "HOW MANY MINES U WANT TO PLACE(1-80)=";
	cin >> mines;
	while (mines < 1 || mines > 80)
	{
		cin.clear();                        //resets cin's error state
		cin.ignore();                       //discards the leftover bad characters in input buffer
		cout << "MINES MUST BE BETWEEN (1-80)--ENTER AGAIN";
		cin >> mines;
	}
	makeBoard(board);
	visibleBoard(vBoard);
	play(board, vBoard, totalcells, mines);

	return 0;
}
