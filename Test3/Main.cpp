#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "Ball.h"
#include "Paddle.h"
#include "GameState.h"
#include<string>
#include<sstream>
#include<stdio.h>
#include<vector>

#pragma region Global Variables

// Define some constants
const float pi = 3.14159f;
const int gameWidth = 800;
const int gameHeight = 600;
int levelCount = 1;
int changeLev = 1;
int lives = 7;
int brickCount = 16;
bool isPlaying = false;
bool isWon = false;
int p1score = 0;
bool pHit = false;
int hitCount = 0;
sf::Sprite pUp;
bool fireball = false;
bool lost = false;
sf::Sprite mainsprite;
sf::Text pauseMessage;
sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "SFML Breakout",sf::Style::Titlebar | sf::Style::Close);
#pragma endregion


void StartScreen(sf::Texture mainscreen)
{
	//StartScreen
	mainsprite.setTexture(mainscreen);
	window.draw(mainsprite);
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	sf::Vector2f paddleSize(100, 25);
	// Create the window of the application
	window.setVerticalSyncEnabled(true);

#pragma region Load Assets

	// Load the sounds used in the game
	sf::SoundBuffer ballSoundBuffer;
	if (!ballSoundBuffer.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/ball.wav"))
		return EXIT_FAILURE;
	sf::Sound ballSound(ballSoundBuffer);

	sf::SoundBuffer bHitS;
	if (!bHitS.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/brick.wav"))
		return EXIT_FAILURE;
	sf::Sound brickHit(bHitS);

	sf::SoundBuffer pHitS;
	if (!pHitS.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/paddle.wav"))
		return EXIT_FAILURE;
	sf::Sound paddleHit(pHitS);

	sf::SoundBuffer levup;
	if (!levup.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/levelup.wav"))
		return EXIT_FAILURE;
	sf::Sound WinLev(levup);

	sf::SoundBuffer death;
	if (!death.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/death.wav"))
		return EXIT_FAILURE;
	sf::Sound die(death);

	sf::SoundBuffer GO;
	if (!GO.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/failure.wav"))
		return EXIT_FAILURE;
	sf::Sound over(GO);

	// Load the text font
	sf::Font font;
	if (!font.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/sansation.ttf"))
		return EXIT_FAILURE;

	// Initialize the pause message
	sf::Texture mainscreen;
	if (!mainscreen.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/mainmenu.png"))
	{
		return EXIT_FAILURE;
	}

	sf::Texture brickT;
	if (!brickT.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/brick.png"))
	{
		return EXIT_FAILURE;
	}

	sf::Texture brickT2;
	if (!brickT2.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/brick2.png"))
	{
		return EXIT_FAILURE;
	}
	sf::Texture brickT3;
	if (!brickT3.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/brick3.png"))
	{
		return EXIT_FAILURE;
	}

#pragma endregion

	//Create Player Paddle
	Paddle::Parameter lpaddle_params;
	lpaddle_params.fColor = sf::Color::Red;
	Paddle leftPaddle(lpaddle_params);

	//Create Ball
	Ball::Parameter ball_params;
	ball_params.fColor = sf::Color::Blue;
	ball_params.oColor = sf::Color::Yellow;
	Ball ball(ball_params);

	//Create Brick
	Brick::Parameter brick_params;
	brick_params.fColor = sf::Color::White;
	std::vector<Brick> brick(16, brick_params);
	for (int i = 0; i < brick.size(); i++)
	{
		brick[i].setTexture(&brickT, false);
	}

#pragma region ScreenText

	//Scoring
	sf::Text score1;
	score1.setFont(font);
	score1.setString("Score=0");
	score1.setCharacterSize(40);
	score1.setFillColor(sf::Color::Red);
	score1.setPosition(0, 0);

	//Lives
	sf::Text Life;
	Life.setFont(font);
	Life.setString("Lives=3");
	Life.setCharacterSize(40);
	Life.setFillColor(sf::Color::Green);
	Life.setPosition(gameWidth-50, 0);

	//Lives
	sf::Text Message;
	Message.setFont(font);
	Message.setString("You finished with the score of");
	Message.setCharacterSize(40);
	Message.setFillColor(sf::Color::Yellow);
	Message.setPosition(100, gameHeight/2);

#pragma endregion

//Create Background Sprite
	sf::Texture texture;
	if (!texture.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/ruins.jpg"))
	{
		return EXIT_FAILURE;
	}
	sf::Sprite sprite;
	sprite.setTexture(texture);

	sf::Clock clock;

//Create Game Manager
	GameState manager(leftPaddle, ball, ballSound);

	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Window closed or escape key pressed: exit
			if ((event.type == sf::Event::Closed) ||
				((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				window.close();
				break;
			}

			// Space key pressed: play
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				if (!isPlaying)
				{
					isPlaying = true;
					clock.restart();

					bool pHit = false;
					int hitCount = 0;
					// Reset the position of the paddles and ball
					manager.Reset(leftPaddle, ball, gameWidth, gameHeight, levelCount, lives);
					manager.BrickDrawI(brick, gameWidth, gameHeight);
				}
			}
		}

		if (isPlaying)
		{
			float deltaTime = clock.restart().asSeconds();
			// Fire the ball
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
			{
				fireball = true;
			}
			//Move Ball
			manager.MoveBall(ball, leftPaddle, fireball, deltaTime);
			//Move Paddle
			manager.MovePaddle(leftPaddle, deltaTime);
			//Win Cond
			if (brick.size() == 0)
			{
					isPlaying = false;
					levelCount = levelCount + 1;
			}					
			//Player Loses reset positions
			if (ball.GetPostion().y + ball.GetParameter().ballRadius > gameHeight+15.0f)
			{
				fireball = false;
				lives = lives - 1;
				if (lives != 0)
				{
					die.play();
					manager.Reset(leftPaddle, ball, gameWidth, gameHeight, levelCount, lives);
				}
				else
				{
					isPlaying = false;
					lost = true;
					over.play();
				}
			}
			//Check for collision against bricks
			p1score= manager.BrickCollide(brick,ball,brickHit, p1score, leftPaddle);
			//Check for bounce against walls
			manager.WallBounce(ball, gameWidth, gameHeight, ballSound);
			// Check the collisions between the ball and the paddle
			manager.PaddleCollide(ball, leftPaddle, paddleHit, fireball);
		}

#pragma region DrawGame

			// Clear the window
			window.clear(sf::Color(50, 200, 50));
			if (isPlaying)
			{
				// Draw the paddles and the ball
				window.draw(sprite);
				std::string nscore = std::to_string(p1score);
				score1.setString(nscore);
				window.draw(score1);
				std::string nLife = std::to_string(lives);
				Life.setString(nLife);
				window.draw(Life);
				leftPaddle.Draw(&window);
				ball.Draw(&window);
				for (int i = 0; i < brick.size(); i++)
				{
					brick[i].Draw(&window);
				}
				if (brick.size() == 0)
				{
					WinLev.play();
					levelCount = levelCount + 1;
				}
				if (levelCount != changeLev)
				{
					//Load New Level
					brick.resize(16 + levelCount * 8);
					fireball = false;
					manager.Reset(leftPaddle, ball, gameWidth, gameHeight, levelCount, lives);
					if (levelCount%2 == 0)
					{
						manager.BrickDrawII(brick, gameWidth, gameHeight, brickT2, brickT);
					}
					else if (levelCount%3 ==0)
					{
						manager.BrickDrawIII(brick, gameWidth, gameHeight, brickT2, brickT, brickT3);
					}
					else if (levelCount % 3 == 1)
					{
						manager.BrickDrawI(brick, gameWidth, gameHeight);
						Ball::DynamicParam bdp = ball.GetDynamicParameter();
						bdp.ballSpeed = bdp.ballSpeed + 100.0f;
						ball.SetDynamicParameter(bdp);
					}
					changeLev = levelCount;
				}
			}
			else if(!isPlaying && !lost)
			{
				// Draw the pause message
				StartScreen(mainscreen);
			}
			else if (!isPlaying && lost)
			{
				window.clear(sf::Color::Black);
				window.draw(Message);
				score1.setPosition(gameWidth / 2, gameHeight / 2 + 50);
				window.draw(score1);
			}
			if (isWon)
			{
				//WinScreen
			}
			window.display();
	}
#pragma endregion

	return EXIT_SUCCESS;
}


