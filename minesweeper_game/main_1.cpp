#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
using namespace std;


// %%%%%%%%%%%%%%%%% ALL ZEROES %%%%%%%%%%%%%%%%%%%

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

int main()
{
	//cpp
	srand(time(0));                      //to place random mines(according to current time)
	int board[9][9];

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
	placeMines(board, mines, 4, 4);
	minesCount(board);
	for (int i = 0;i < 9;i++) {
		for (int j = 0;j < 9;j++) {
			if (board[i][j] >= 0) {
				cout << " ";
			}
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	//play(board, vBoard, totalcells, mines);






//

	sf::RenderWindow window(sf::VideoMode({ 900, 900 }), "Minesweeper");
	bool flag = 0;
	float wind_x = 900;
	float wind_y = 900;
	int csize = 50;
	int gap = 10;
	sf::Font font("DenitahStellar.ttf");


	sf::RectangleShape grid[9][9];
	for (int i = 0;i < 9;i++) {
		for (int j = 0;j < 9;j++) {
			grid[i][j].setSize({ 50.f,50.f });
			//grid[i][j].setOrigin({ 25.f,25.f });
			grid[i][j].setFillColor(sf::Color(128, 128, 128));
			grid[i][j].setOutlineThickness(-2.f);
			grid[i][j].setOutlineColor(sf::Color::Black);

		}
	}

	// %%%%%%%%%%%%% character array %%%%%%%%%
	char Rboard[9][9];
	for (int i = 0;i < 9;i++) {
		for (int j = 0;j < 9;j++) {
			if (board[i][j] == -1) {
				Rboard[i][j] = 'M';
			}
			else if (board[i][j] == 0) {
				Rboard[i][j] = ' ';
			}
			else
				Rboard[i][j] = board[i][j] + 48;
		}
	}
	//text 

	char ch = '"s"';
	sf::Text text(font, ch, 36);



	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		window.clear(sf::Color::Black);
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();

			//mouse click
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
			{

				int mouseX = sf::Mouse::getPosition(window).x;
				int mouseY = sf::Mouse::getPosition(window).y;
				int relativeX = mouseX - 180;
				int relativeY = mouseY - 180;

				int clicks = 0;
				if (mousePressed->button == sf::Mouse::Button::Left)
				{
					for (int i = 0;i < 9;i++) {
						for (int j = 0;j < 9;j++) {
							if (relativeX >= j * gap + 50 * j && relativeX <= j * gap + (50 * j) + 50 && relativeY >= i * gap + 50 * i && relativeY <= i * gap + (50 * i) + 50) { //total len of gap till start + end(+50)


								if (grid[i][j].getFillColor() != sf::Color::Red)
								{
									grid[i][j].setFillColor(sf::Color::Transparent);
									if (Rboard[i][j] == 'M') {
										for (int i = 0;i < 9;i++) {
											for (int j = 0;j < 9;j++) {
												grid[i][j].setFillColor(sf::Color::Transparent);
											}
										}
									}
								}

							}
						}
					}
				}

				if (mousePressed->button == sf::Mouse::Button::Right)
				{
					for (int i = 0;i < 9;i++) {
						for (int j = 0;j < 9;j++) {

							if (relativeX >= j * gap + 50 * j && relativeX <= j * gap + (50 * j) + 50 && relativeY >= i * gap + 50 * i && relativeY <= i * gap + (50 * i) + 50) { //total len of gap till start + end(+50)
								if (grid[i][j].getFillColor() != sf::Color::Red)
									grid[i][j].setFillColor(sf::Color::Red);
								else {
									grid[i][j].setFillColor(sf::Color(128, 128, 128));
								}

							}
						}
					}
				}
			}

		}



		//drawing
		float tx = 205;
		float ty = 180;
		//text drawing
		for (int i = 0;i < 9;i++) {
			for (int j = 0;j < 9;j++) {
				ch = Rboard[i][j];
				sf::Text text(font, ch, 36);
				text.setPosition({ tx,ty });
				window.draw(text);
				tx = tx + 60;

			}
			tx = 205;
			ty = ty + 60;
		}

		//grid drawing
		int numCells = 9;
		int position_formula = (((wind_x / numCells) - csize - gap) / 2) * numCells;
		float posx = position_formula;
		float posy = 180;
		grid[0][0].setPosition({ posx,posy });
		for (int i = 0;i < 9;i++) {
			posx = 180;
			for (int j = 0;j < 9;j++) {
				grid[i][j].setPosition({ posx,posy });
				window.draw(grid[i][j]);
				posx += csize + gap;

			}
			posy += csize + gap;
		}

		//window.draw(text);
		window.display();
	}

}