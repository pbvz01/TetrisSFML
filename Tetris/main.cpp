#include <SFML/Graphics.hpp>
#include <stdlib.h> 
#include <time.h>


const int countBlocks = 4;
const int widthField = 10;
const int heightField = 20;

int playField[heightField][widthField]{0};

struct Point
{
	int x;
	int y;
};

const int dataFigures[7][4] =
{
	{1,3,5,7},
	{2,4,5,7},
	{3,5,4,6},
	{3,5,4,7},
	{2,3,5,7},
	{3,5,7,6},
	{2,3,4,5}
};

int GetRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
};

void translateOnFigure(const int numFigure, Point* arrFigures)
{

	for (int i = 0; i < countBlocks; i++)
	{
		arrFigures[i].x = dataFigures[numFigure][i] % 2;
		arrFigures[i].y = dataFigures[numFigure][i] / 2;
	}
};

bool check(const Point* arr)
{
	for (int i = 0; i < countBlocks; i++)
		if (arr[i].x < 0 || arr[i].x >= widthField || arr[i].y >= heightField) return 0;
		else if (playField[arr[i].y][arr[i].x]) return 0;

	return 1;
};

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	rand();

	Point dataBlock[4]{ 0 };
	Point copyBlock[4]{ 0 };
	int numFigures = GetRandomNumber(0,6);
	int colorFigures = GetRandomNumber(1,6);

	sf::RenderWindow mainWindow(sf::VideoMode(400,600), "Tetris");
	sf::Texture textureBlock;
	textureBlock.loadFromFile("image/BlocksTexture.png");

	sf::Sprite spriteBlock(textureBlock, sf::IntRect(30 * colorFigures,0,30,30));

	int moveX = 0;
	bool rotate = false;

	float timer = 0, delay = 0.3f;
	sf::Clock clock;

	bool beginGame = true;

	while (mainWindow.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mainWindow.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Left) moveX = -1;
				else if (event.key.code == sf::Keyboard::Right) moveX = 1;
				else if (event.key.code == sf::Keyboard::Up) rotate = true;
				else if (event.key.code == sf::Keyboard::Down) delay = 0.05;
			}
		}

		if (rotate)
		{
			Point p = dataBlock[1]; 
			for (int i = 0; i < countBlocks; i++)
			{
				copyBlock[i] = dataBlock[i];

				int x = dataBlock[i].y - p.y; 
				int y = dataBlock[i].x - p.x; 
				dataBlock[i].x = p.x - x;
				dataBlock[i].y = p.y + y;
			}
			if (!check(dataBlock))
			{
				for (int i = 0; i < countBlocks; i++)
				{
					dataBlock[i] = copyBlock[i];
				}
			}
		}

		if (moveX)
		{
			for (int i = 0; i < countBlocks; i++)
			{
				copyBlock[i] = dataBlock[i];
				dataBlock[i].x += moveX;
			}
			if (!check(dataBlock))
			{
				for (int i = 0; i < countBlocks; i++)
				{
					dataBlock[i] = copyBlock[i];
				}
			}
		}


		if (timer > delay)
		{
			for (int i = 0; i < countBlocks; i++)
			{
				copyBlock[i] = dataBlock[i];
				dataBlock[i].y += 1;
			}
			if (!check(dataBlock))
			{
				for (int i = 0; i < countBlocks; i++)
				{
					playField[copyBlock[i].y][copyBlock[i].x] = colorFigures;
				}

				numFigures = GetRandomNumber(0,6);
				colorFigures = GetRandomNumber(1,6);

				translateOnFigure(numFigures, dataBlock);
			}

			timer = 0;
		}

		int k = heightField - 1;
		for (int i = heightField - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < widthField; j++)
			{
				if (playField[i][j]) count++;
				playField[k][j] = playField[i][j];
			}
			if (count < widthField) k--;
		}


		if (beginGame)
		{
			translateOnFigure(numFigures, dataBlock);
			beginGame = false;
		}
			
		moveX = 0;
		rotate = false;
		delay = 0.3f;

		mainWindow.clear(sf::Color::White);

		for (int i = 0; i < heightField; i++)
			for (int j = 0; j < widthField; j++)
			{
				if (playField[i][j] == 0) continue;
				spriteBlock.setTextureRect(sf::IntRect(playField[i][j]*30,0,30,30));
				spriteBlock.setPosition(j * 30, i * 30);
				mainWindow.draw(spriteBlock);
			}

		for (int i = 0; i < countBlocks; i++)
		{
			spriteBlock.setTextureRect(sf::IntRect(colorFigures*30,0,30,30));

			spriteBlock.setPosition(dataBlock[i].x * 30, dataBlock[i].y * 30);

			mainWindow.draw(spriteBlock);
		}

		mainWindow.display();
	}

	return 0;
}


