#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

// %%%%%%%%%%%%%%%%% ALL ZEROES %%%%%%%%%%%%%%%%%%%

void makeBoard(int board[][100], int rows, int cols)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			board[i][j] = 0;
}


// %%%%%%%%%%%%%%%%% PLACE MINES %%%%%%%%%%%%%%%%

void placeMines(int board[][100], int rows, int cols, int mines, int saferow, int safecol)
{
	int n = 0;
	while (n < mines)
	{
		int x = rand() % rows;              //for random cell
		int y = rand() % cols;
		if (x == saferow && y == safecol)
			continue;                       // first  click safe
		if (board[x][y] != -1)              
		{
			board[x][y] = -1;
			n++;
		}
	}
}


// %%%%%%%%%%%% COUNT MINES AROUND EACH CELL %%%%%%%

void minesCount(int board[][100], int rows, int cols)
{
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (board[r][c] != -1)
			{
				int count = 0;
				for (int i = r - 1; i <= r + 1; i++)
					for (int j = c - 1; j <= c + 1; j++)
						if (i >= 0 && i < rows && j >= 0 && j < cols && board[i][j] == -1)
							count++;
				board[r][c] = count;
			}
		}
	}
}


// &&&&&&&&&& empty cells open &&&&&&
void openEmptyCells(int board[][100], int row, int col, sf::RectangleShape grid[][100], int Rows, int Cols) {

	for (int i = row - 1; i <= row + 1; i++) {
		for (int j = col - 1; j <= col + 1; j++) {
			if (i >= Rows || j >= Cols || i < 0 || j < 0)
				continue;                                   // outside the board
			if (grid[i][j].getFillColor() == sf::Color::Red)
				continue;                                   // flagged cells stay closed
			if (grid[i][j].getFillColor() == sf::Color::Transparent)
				continue;                                   // already open, stops endless recursion

			grid[i][j].setFillColor(sf::Color::Transparent);

			if (board[i][j] == 0)
				openEmptyCells(board, i, j, grid, Rows, Cols);
		}
	}
}


int play(int rows, int cols, int mines) {          

	srand((unsigned)time(0));
	int board[100][100];

	
	int totalcells = rows * cols;

	// %%%%%%%%%%%%% FIXED CELL SIZE AND GAP %%%%%%%%%%
	float csize = 38;
	float gap = 8;
	float step = csize + gap;
	float headerH = 50;


	float wind_x = 1500;
	float wind_y = 1000;

	makeBoard(board, rows, cols);                   

	sf::RenderWindow window(sf::VideoMode({ 1500,1000 }), "Minesweeper");
	int win = 0;
	int over = 0;
	int flag = 0;
	sf::Font font("DenitahStellar.ttf");

	// %%%%%%%%%%%%% centre== start =free space / 2 %%%%%%%%%%%%%
	float boardW = cols * csize + (cols - 1) * gap;
	float boardH = rows * csize + (rows - 1) * gap;
	float startX = (wind_x - boardW) / 2;                              // left right center
	float startY = headerH + (wind_y - headerH - boardH) / 2;         

	static sf::RectangleShape grid[100][100];    
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			grid[i][j].setSize({ csize, csize });
			grid[i][j].setPosition({ startX + j * step, startY + i * step });
			grid[i][j].setFillColor(sf::Color(128, 128, 128));
			grid[i][j].setOutlineThickness(-2.f);
			grid[i][j].setOutlineColor(sf::Color::Black);
		}
	}

	// %%%%%%%%%%%%% character array %%%%%%%%%
	char Rboard[100][100];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			Rboard[i][j] = ' ';
	bool firstclick = true;

	//text
	char ch = 's';

	sf::Text heading1(font, "flags: ", 50);
	heading1.setPosition({ 30, 10 });
	heading1.setFillColor(sf::Color::Black);

	sf::Text flagCount(font, to_string(flag), 50);
	flagCount.setPosition({ 220, 10 });
	flagCount.setFillColor(sf::Color::Black);

	sf::Text gameOver(font, "GAME OVER", 80);
	gameOver.setStyle(sf::Text::Bold);
	gameOver.setFillColor(sf::Color::Red);
	gameOver.setOutlineThickness(3.f);
	gameOver.setOutlineColor(sf::Color::White);
	sf::FloatRect goBounds = gameOver.getLocalBounds();
	gameOver.setOrigin(goBounds.position + goBounds.size / 2.f);   // origin = center of the text
	gameOver.setPosition({ wind_x / 2, wind_y / 2 });              // text center = window center


	sf::Text won(font, "GAME WON!", 80);
	won.setStyle(sf::Text::Bold);
	won.setFillColor(sf::Color(0, 150, 0));
	won.setOutlineThickness(3.f);
	won.setOutlineColor(sf::Color::White);
	sf::FloatRect wonBounds = won.getLocalBounds();
	won.setOrigin(wonBounds.position + wonBounds.size / 2.f);
	won.setPosition({ wind_x / 2, wind_y / 2 });


	// %%%%%%%%%%%%%%%%%% sprites &&&&&&&&&&&&&&&&&&&&&
	sf::Texture texture2;
	if (!texture2.loadFromFile("flag_game.jpg")) {
		cout << "error 2";
	}
	sf::Texture texture;
	if (!texture.loadFromFile("mine_bomb.jpg")) {
		cout << "error";
	}

	// scale = wanted size (50) / picture width so the picture fits one cell
	sf::Sprite flag_img(texture2);
	flag_img.setScale({ csize / texture2.getSize().x, csize / texture2.getSize().x });

	sf::Sprite bomb(texture);
	bomb.setScale({ csize / texture.getSize().x, csize / texture.getSize().x });


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
				if (over == 1 || win == 1)
					continue;                                  // game finished now ignore clicks

				float relativeX = mousePressed->position.x - startX;   // distance from board top-left corner
				float relativeY = mousePressed->position.y - startY;

				if (mousePressed->button == sf::Mouse::Button::Left)
				{
					for (int i = 0; i < rows; i++) {
						for (int j = 0; j < cols; j++) {
							if (relativeX >= j * step && relativeX <= j * step + csize && relativeY >= i * step && relativeY <= i * step + csize)
							{
								if (grid[i][j].getFillColor() == sf::Color(128, 128, 128))   // closed, unflagged cell
								{
									if (firstclick)
									{
										placeMines(board, rows, cols, mines, i, j);   
										minesCount(board, rows, cols);

										for (int a = 0; a < rows; a++)
										{
											for (int b = 0; b < cols; b++)
											{
												if (board[a][b] == -1) Rboard[a][b] = 'M';
												else if (board[a][b] == 0) Rboard[a][b] = ' ';
												else Rboard[a][b] = board[a][b] + 48;
											}
										}
										firstclick = false;
									}

									grid[i][j].setFillColor(sf::Color::Transparent);
									if (board[i][j] == 0)
										openEmptyCells(board, i, j, grid, rows, cols);

									if (Rboard[i][j] == 'M') {
										over = 1;
										for (int a = 0; a < rows; a++)
											for (int b = 0; b < cols; b++)
												if (Rboard[a][b] == 'M')
													grid[a][b].setFillColor(sf::Color::Transparent);
									}
									else {
										// win check
										int opened = 0;
										for (int a = 0; a < rows; a++)
											for (int b = 0; b < cols; b++)
												if (grid[a][b].getFillColor() == sf::Color::Transparent)
													opened++;
										if (opened == totalcells - mines)
											win = 1;
									}
								}
							}
						}
					}
				}

				if (mousePressed->button == sf::Mouse::Button::Right)
				{
					for (int i = 0; i < rows; i++) {
						for (int j = 0; j < cols; j++) {
							if (relativeX >= j * step && relativeX <= j * step + csize && relativeY >= i * step && relativeY <= i * step + csize)
							{
								if (grid[i][j].getFillColor() == sf::Color(128, 128, 128))
								{
									grid[i][j].setFillColor(sf::Color::Red);
									flag++;
									flagCount.setString(to_string(flag));
								}
								else if (grid[i][j].getFillColor() == sf::Color::Red) {
									grid[i][j].setFillColor(sf::Color(128, 128, 128));
									flag--;
									flagCount.setString(to_string(flag));
								}
								// an opened cell cannot be flagged
							}
						}
					}
				}
			}
		}


		window.draw(flagCount);
		window.draw(heading1);

		//numbers and bombs (hidden under the gray squares)
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {

				ch = Rboard[i][j];
				float cellX = startX + j * step;
				float cellY = startY + i * step;

				if (ch == 'M') {
					bomb.setPosition({ cellX, cellY });
					window.draw(bomb);
				}
				else if (ch != ' ')
				{
					sf::Text num(font, string(1, ch), 36);
					switch (ch) {
					case '1': num.setFillColor(sf::Color::Blue); break;
					case '2': num.setFillColor(sf::Color(0, 128, 0)); break;
					case '3': num.setFillColor(sf::Color::Red); break;

					default:  num.setFillColor(sf::Color(100, 100, 100)); break;
					}
					sf::FloatRect nb = num.getLocalBounds();
					num.setOrigin(nb.position + nb.size / 2.f);                       // center of the digit
					num.setPosition({ cellX + csize / 2, cellY + csize / 2 });        // center of the cell
					window.draw(num);
				}
			}
		}

		//grid drawing (on top of the numbers)
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				window.draw(grid[i][j]);

		//flag pictures on red cells
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (grid[i][j].getFillColor() == sf::Color::Red)
				{
					flag_img.setPosition({ startX + j * step, startY + i * step });
					window.draw(flag_img);
				}
			}
		}

		if (over == 1)
			window.draw(gameOver);
		if (win == 1)
			window.draw(won);

		window.display();
	}
	return 0;

}



//// &&&&&&&&&&&&&&&&&&&&&& main &&&&&&&&&&&&&&&&&&&&&&&&&&
int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1500, 1000 }), "MINESWEEPER");


	sf::Font font("DenitahStellar.ttf");

	sf::Text levels(font, "Choose Level", 100);
	levels.setFillColor(sf::Color::Cyan);
	levels.setPosition({ 500,250 });
	levels.setOutlineThickness(-2);
	levels.setOutlineColor(sf::Color::Black);

	sf::RectangleShape basic({ 250,130 });
	basic.setPosition({ 300,400 });
	basic.setOutlineColor(sf::Color::Green);
	basic.setFillColor(sf::Color::Green);

	sf::Text basicH(font, "BASIC", 100);
	basicH.setOutlineThickness(-2);
	basicH.setOutlineColor(sf::Color::Black);
	basicH.setPosition({ 300,400 });


	sf::RectangleShape inter({ 250,130 });
	inter.setPosition({ 600,400 });
	inter.setOutlineColor(sf::Color::Green);
	inter.setFillColor(sf::Color::Yellow);

	sf::Text interH(font, "INTER", 100);
	interH.setOutlineThickness(-2);
	interH.setOutlineColor(sf::Color::Black);
	interH.setPosition({ 600,400 });



	sf::RectangleShape hard({ 250,130 });
	hard.setPosition({ 900,400 });
	hard.setOutlineColor(sf::Color::Green);
	hard.setFillColor(sf::Color::Red);

	sf::Text hardH(font, "HARD", 100);
	hardH.setOutlineThickness(-2);
	hardH.setOutlineColor(sf::Color::Black);
	hardH.setPosition({ 900,400 });



	sf::FloatRect boundsB = basic.getGlobalBounds();
	sf::FloatRect boundsI = inter.getGlobalBounds();
	sf::FloatRect boundsH = hard.getGlobalBounds();

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}



			//detect click 
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
			{

				int mouseX = mousePressed->position.x;
				int mouseY = mousePressed->position.y;
				if (mousePressed->button == sf::Mouse::Button::Left) {

					//easy
					if (mouseX >= boundsB.position.x &&
						mouseX <= boundsB.position.x + boundsB.size.x &&
						mouseY >= boundsB.position.y &&
						mouseY <= boundsB.position.y + boundsB.size.y) {

						play(9, 9, 12);      
					}

					//inter
					if (mouseX >= boundsI.position.x &&
						mouseX <= boundsI.position.x + boundsI.size.x &&
						mouseY >= boundsI.position.y &&
						mouseY <= boundsI.position.y + boundsI.size.y) {

						play(16, 16, 40);        
					}
					///hard
					if (mouseX >= boundsH.position.x &&
						mouseX <= boundsH.position.x + boundsH.size.x &&
						mouseY >= boundsH.position.y &&
						mouseY <= boundsH.position.y + boundsH.size.y) {

						play(20, 20, 60);        
					}
				}
			}



		}

		window.clear(sf::Color::White);
		window.draw(levels);
		window.draw(basic);
		window.draw(basicH);
		window.draw(inter);
		window.draw(interH);
		window.draw(hard);
		window.draw(hardH);



		window.display();
	}

	return 0;
}
