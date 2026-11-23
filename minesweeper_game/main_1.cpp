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


//void emptyCellOpen(char Rboard[][9],int board[][9], int row, int col) {
//
//	if (row < 0 || row >= 9 || col < 0 || col >= 9)                   //outside boundary
//				return;
//		
//			if (Rboard[row][col] != '# ')                                      //if already opened or flaged
//				return;
//		
//
//	if (board[row][col] == 0)
//			{
//				for (int i = -1; i <= 1; ++i)
//				{
//					for (int j = -1; j <= 1; ++j)
//					{
//						if (i != 0 || j != 0)                                // leave the cell own(0,0) position
//						{
//							int r = row + i;
//							int c = col + j;
//							emptyCellOpen(Rboard,board, i, j);
//						}
//		
//					}
//				}
//			}
//}

// &&&&&&&&&& empty cells open &&&&&&&&&&&&
void openEmptyCells(int board[][9], int row, int col, sf::RectangleShape grid[][9]) {


	if (row < 9 && col < 9)
	{
		for (int i = row - 1;i <= row + 1;i++) {
			for (int j = col - 1;j <= col + 1;j++) {
				if (i > 8 || j > 8 || i < 0 || j < 0) {
					continue;
				}
				if (grid[i][j].getFillColor() == sf::Color::Red) {
					continue;
				}
				if (grid[i][j].getFillColor() == sf::Color::Transparent) {
					continue;// stops empty cells from calling function again and again
				}
				grid[i][j].setFillColor(sf::Color::Transparent);

				if (board[i][j] == 0) {
					int r = i;
					int c = j;
					openEmptyCells(board, r, c, grid);
				}

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

	mines = 10;
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
	int win = 0;
	int over = 0;
	int flag = 0;
	float wind_x = 900;
	float wind_y = 900;
	int csize = 50;
	int gap = 10;
	sf::Font font("DenitahStellar.ttf");


	sf::RectangleShape grid[9][9];
	for (int i = 0;i < 9;i++) {
		for (int j = 0;j < 9;j++) {
			grid[i][j].setSize({ 50.f,50.f });
			grid[i][j].setOrigin({ 25.f,25.f });
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
	text.setFillColor(sf::Color::Magenta);
	sf::FloatRect Tbounds = text.getLocalBounds();
	text.setOrigin(Tbounds.position + Tbounds.size / 2.f);

	sf::Text won(font, "GAME WON!", 200);
	won.setPosition({ 250, 250 });
	won.setFillColor(sf::Color::Green);
	sf::Text flagCount(font, char(flag + '0'), 70);
	flagCount.setPosition({ 200,50 });
	flagCount.setFillColor(sf::Color::Black);

	sf::Text heading1(font, "flags: ", 70);
	heading1.setPosition({ 50,50 });
	heading1.setFillColor(sf::Color::Black);
	sf::Text gameOver(font, "GAME OVER", 100);
	gameOver.setPosition({ 350.f, 350.f });
	gameOver.Bold;
	gameOver.setFillColor(sf::Color::Red);
	gameOver.setCharacterSize(100);



	// %%%%%%%%%%%%%%%%%% sprites &&&&&&&&&&&&&&&&&&&&&
	sf::Texture texture2;
	if (!texture2.loadFromFile("flag_game.jpg")) {
		cout << "error 2";
	}
	sf::Texture texture;
	if (!texture.loadFromFile("mine_bomb.jpg")) {
		cout << "error";
	}

	sf::Sprite flag_img(texture2);
	flag_img.setScale({ 0.04f,0.04f });

	sf::Sprite bomb(texture);
	//sf::FloatRect bounds = sprite.getLocalBounds();
	//sprite.setOrigin(bounds.position + bounds.size / 2.f);
	bomb.setScale({ 0.06f,0.06f });


	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		window.clear(sf::Color::White);

		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();

			//mouse click
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
			{

				int mouseX = sf::Mouse::getPosition(window).x;
				int mouseY = sf::Mouse::getPosition(window).y;
				int relativeX = mouseX - 155;
				int relativeY = mouseY - 155;

				int clicks = 0;
				if (mousePressed->button == sf::Mouse::Button::Left)
				{
					for (int i = 0;i < 9;i++) {
						for (int j = 0;j < 9;j++) {
							if (relativeX >= j * gap + 50 * j && relativeX <= j * gap + (50 * j) + 50 && relativeY >= i * gap + 50 * i && relativeY <= i * gap + (50 * i) + 50)
							{ //total len of gap till start + end(+50)


								if (grid[i][j].getFillColor() != sf::Color::Red)
								{
									grid[i][j].setFillColor(sf::Color::Transparent);
									if (board[i][j] == 0) {
										openEmptyCells(board, i, j, grid);
									}
									if (Rboard[i][j] == 'M') {
										for (int i = 0;i < 9;i++) {
											for (int j = 0;j < 9;j++) {
												if (Rboard[i][j] == 'M')
												{
													grid[i][j].setFillColor(sf::Color::Transparent);
												}
												over = 1;

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
								{
									grid[i][j].setFillColor(sf::Color::Red);
									flag++;
									flagCount.setString(char(flag + '0'));
									//sprite


								}
								else {
									grid[i][j].setFillColor(sf::Color(128, 128, 128));
									flag--;
									flagCount.setString(char(flag + '0'));


								}

							}
						}
					}
				}
			}

		}


		window.draw(flagCount);
		window.draw(heading1);
		//drawing
		float tx = 155;  //
		float ty = 155;
		//text drawing


		for (int i = 0;i < 9;i++) {
			for (int j = 0;j < 9;j++) {




				ch = Rboard[i][j];
				sf::Text text(font, ch, 36);
				text.setFillColor(sf::Color::Black);
				text.setPosition({ tx,ty });
				bomb.setPosition({ tx,ty });
				if (ch == 'M') {
					window.draw(bomb);
				}
				else
				{
					text.setPosition({ tx + 10,ty + 10 });
					if (text.getString() == '1') {
						text.setFillColor(sf::Color::Blue);
					}
					else if (text.getString() == '2') {
						text.setFillColor(sf::Color::Green);
					}
					else if (text.getString() == '3') {
						text.setFillColor(sf::Color::Red);
					}
					window.draw(text);
				}

				tx = tx + 60;

			}
			tx = 155;
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

		//window.draw(flag_img);
		if (over == 1) {
			window.draw(gameOver);
		}


		for (int i = 0;i < 9;i++) {
			for (int j = 0;j < 9;j++)
			{
				if (grid[i][j].getFillColor() == sf::Color::Red)
				{
					flag_img.setPosition({ float(j * gap + 50 * j) + 155 ,float(i * gap + 50 * i) + 155 });
					window.draw(flag_img);
				}


			}
		}





		//window.draw(text);
		window.display();
	}

}