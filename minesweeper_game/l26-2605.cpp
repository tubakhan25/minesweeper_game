#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
int openCell(char vBoard[][9], const int board[][9], int row, int col);
void emptyCellCheck(char vBoard[][9], const int board[][9], int row, int col);
void visibleBoard(char vBoard[][9]) {
	for (int i = 0;i < 9;i++) {
		for (int j = 0;j < 9;j++) {
			vBoard[i][j] = '#';
		}
	}

}
void makeBoard(int board[][9]) {
	for (int i = 0; i < 9;i++) {
		for (int j = 0;j < 9;j++) {
			board[i][j] = 0;
		}
	}
}
void printBoard(int board[][9]) {
	for (int i = 0;i < 9;i++) {

		for (int j = 0; j < 9;j++) {
			if (board[i][j] >= 0) {
				cout << " ";
			}
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

// *********** PLACE MINES
void placeMines(int board[][9]) {
	srand(time(0));
	for (int i = 0;i < 11;i++)
	{

		int random_x = rand() % 9;
		int random_y = rand() % 9;
		//cout << random_x << " " << random_y;
		if (random_x != 4 && random_y != 4)
			board[random_x][random_y] = -1;
		else
			continue;

	}

}
void minesCount(int board[][9]) {

	for (int r = 0;r < 9;r++) {
		for (int c = 0;c < 9;c++) {
			if (board[r][c] != -1) {
				int count = 0;
				for (int i = r - 1;i <= r + 1;i++) {
					for (int j = c - 1;j <= c + 1;j++) {
						if (i >= 0 && i < 9 && j >= 0 && j < 9 && board[i][j] == -1) {
							count++;

						}
					}
				}
				board[r][c] = count;

			}
		}
	}
	//if close
}

int flag(int board[][9], char vBoard[][9], int r, int c) {


	if (board[r][c] == -1) {
		vBoard[r][c] = 'F';
	}
	else {
		vBoard[r][c] = 'f';
	}
	return 1;
}

void unFlag(char vBoard[][9], int r, int c) {
	if (vBoard[r][c] == 'F' || vBoard[r][c] == 'f') {
		vBoard[r][c] = '#';
	}
}

int play(int board[][9], char vBoard[][9], int& rightFlag) {
	int attempts = 0;
	bool fl = 0;
	int row;
	int col;
	while (true) {
		cout << "enter row and column: ";
		cin >> row >> col;
		if (row < 0 || row >= 9 || col < 0 || col >= 9)
			return 0;
		attempts++;
		cout << "Flag(1) or Open(0)";
		cin >> fl;
		if (fl == 0 && board[row][col] == -1 && (vBoard[row][col] != 'f' && vBoard[row][col] != 'F')) {
			cout << "GAME OVER" << endl;
			printBoard(board);
			return 0;
		}
		if (fl == 1 && rightFlag < 10) {
			flag(board, vBoard, row, col);
			rightFlag++;
		}
		else if (fl == 0) {
			openCell(vBoard, board, row, col);
			for (int i = 0;i < 9;i++) {
				for (int j = 0;j < 9;j++) {
					cout << vBoard[i][j] << " ";
				}
				cout << endl;
			}
		}
	}
	return attempts;
}

int openCell(char vBoard[][9], const int board[][9], int row, int col) {
	int value;

	if (vBoard[row][col] == '#') {
		value = board[row][col];
		vBoard[row][col] = char(value) + 48;
		// In openCell, after revealing the selected cell:
		if (board[row][col] == 0) {
			for (int i = row - 1; i <= row + 1; ++i) {
				for (int j = col - 1; j <= col + 1; ++j) {
					if (i != row || j != col)
						emptyCellCheck(vBoard, board, i, j);
				}
			}
		}

	}

	else if (vBoard[row][col] == 'F' || vBoard[row][col] == 'f') {
		cout << "Flagged" << endl;
		return 0;
	}
	else {
		cout << "already opened" << endl;
	}
	return 1;
}
void emptyCellCheck(char vBoard[][9], const int board[][9], int row, int col) {
	if (row < 0 || row >= 9 || col < 0 || col >= 9)
		return;

	if (vBoard[row][col] != '#') // already open or flagged
		return;

	if (board[row][col] != 0)   // return without revealing numbered  cell
		return;

	vBoard[row][col] = '0';

	for (int i = row - 1; i <= row + 1; i++) {
		for (int j = col - 1; j <= col + 1; j++) {
			if (i != row || j != col)
				emptyCellCheck(vBoard, board, i, j);
		}
	}
}

//for (int i = row - 1;i <= row + 1;i++) {
//	for (int j = col - 1;j <= col + 1;j++) {
//		if (board[i][j] == -1) {
//			return 0;
//		}
//		else {
//			vBoard[i][j] = '0';
//			
//		}
//	}
//}
//return 1;

int main() {
	int rightFlag = 0;
	int board[9][9];
	char vBoard[9][9];
	makeBoard(board);

	placeMines(board);
	printBoard(board);
	minesCount(board);
	printBoard(board);
	visibleBoard(vBoard);

	for (int i = 0;i < 9;i++) {
		for (int j = 0;j < 9;j++) {
			cout << vBoard[i][j] << " ";
		}
		cout << endl;
	}




	int attempts = play(board, vBoard, rightFlag);
	cout << attempts;



}