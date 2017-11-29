#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "Ball.h"
#include "Paddle.h"
#include<string>
#include<sstream>
#include<stdio.h>

int p1score = 0;
int p2score = 0;
bool pHit = false;
int hitCount = 0;
sf::Sprite pUp;
bool bigSize = false;
bool fireball = false;

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	// Define some constants
	const float pi = 3.14159f;
	const int gameWidth = 800;
	const int gameHeight = 600;
	sf::Vector2f paddleSize(25, 100);
	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "SFML Pong",
		sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	/* Create the left paddle
	sf::RectangleShape leftPaddle;
	leftPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
	leftPaddle.setOutlineThickness(3);
	leftPaddle.setOutlineColor(sf::Color::Black);
	leftPaddle.setFillColor(sf::Color::Red);
	leftPaddle.setOrigin(paddleSize / 2.f);
*/
	Paddle::Parameter lpaddle_params;
	lpaddle_params.fColor = sf::Color::Red;
	Paddle leftPaddle(lpaddle_params);

	Paddle::Parameter rpaddle_params;
	rpaddle_params.fColor = sf::Color::Blue;
	Paddle rightPaddle(rpaddle_params);
	
	Paddle::Parameter opaddle_params;
	opaddle_params.fColor = sf::Color::Green;
	opaddle_params.pHeight = 50;
	opaddle_params.pWidth = 50;
	Paddle oPaddle(opaddle_params);

	Ball::Parameter ball_params;
	Ball ball(ball_params);	
	
	//Load the sound
	// Load the sounds used in the game
	sf::SoundBuffer ballSoundBuffer;
	if (!ballSoundBuffer.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/ball.wav"))
		return EXIT_FAILURE;
	sf::Sound ballSound(ballSoundBuffer);

	// Load the text font
	sf::Font font;
	if (!font.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/sansation.ttf"))
		return EXIT_FAILURE;

	// Initialize the pause message
	sf::Texture mainscreen;
	if (!mainscreen.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/main.jpg"))
	{
		return EXIT_FAILURE;
	}
	sf::Sprite mainsprite;
	mainsprite.setTexture(mainscreen);
	sf::Text pauseMessage;
	pauseMessage.setFont(font);
	pauseMessage.setCharacterSize(60);
	pauseMessage.setPosition(150.f, 100.f);
	pauseMessage.setFillColor(sf::Color::Red);
	pauseMessage.setString("Welcome to Pong!\nPress SPACE to Play");

	//Scoring
	sf::Text score1;
	score1.setFont(font);
	score1.setString("P1=0");
	score1.setCharacterSize(40);
	score1.setFillColor(sf::Color::Red);
	score1.setPosition(0, 0);

	sf::Text score2;
	score2.setFont(font);
	score2.setString("P2=0");
	score2.setCharacterSize(40);
	score2.setFillColor(sf::Color::Blue);
	score2.setPosition(650, 0);

	//Load Powerup
	sf::Texture powerup;
	if (!powerup.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/plus.png"))
	{
		return EXIT_FAILURE;
	}

	sf::Texture fireup;
	if (!fireup.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/fireball.png"))
	{
		return EXIT_FAILURE;
	}

	sf::Texture def;
	if (!def.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/def.png"))
	{
		return EXIT_FAILURE;
	}
//	pUp.setTexture(powerup);
//	pUp.setPosition(sf::Vector2f(gameWidth/2, gameHeight/2));
	//Load Background Image
	sf::Texture texture;
	if (!texture.loadFromFile("C:/Users/Akshay/source/repos/Test3/Test3/resources/thegrid.png"))
	{
		return EXIT_FAILURE;
	}

	//Create Background Sprite
	sf::Sprite sprite;
	sprite.setTexture(texture);


	// Define the paddles properties
	sf::Clock AITimer;
	const sf::Time AITime = sf::seconds(0.05f);
//	const float paddleSpeed = 400.f;
//	float rightPaddleSpeed = 400.f;


	sf::Clock clock;
	bool isPlaying = false;
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
					Paddle::Position lPaddlePos;
					lPaddlePos.x = 10 + leftPaddle.GetParameter().pWidth/2;
					lPaddlePos.y= gameHeight / 2;
					leftPaddle.SetPosition(lPaddlePos);

					Paddle::Position rPaddlePos;
					rPaddlePos.x = gameWidth - 10 - rightPaddle.GetParameter().pWidth/ 2;
					rPaddlePos.y = gameHeight / 2;
					rightPaddle.SetPosition(rPaddlePos);

					Paddle::Position oPaddlePos;
					oPaddlePos.x = gameWidth/2 +oPaddle.GetParameter().pWidth/2;
					oPaddlePos.y = gameHeight /2;
					oPaddle.SetPosition(oPaddlePos);

					Ball::Position originPos;
					originPos.x = gameWidth / 2;
					originPos.y = gameHeight / 2;
					ball.SetPosition(originPos);

					// Reset the ball angle
					do
					{
						// Make sure the ball initial angle is not too much vertical
						Ball::DynamicParam dp = ball.GetDynamicParameter();
						dp.ballAngle = (std::rand() % 360) * 2 * pi / 360;
						dp.ballSpeed = 400.0f;
						ball.SetDynamicParameter(dp);
					} while (std::abs(std::cos(ball.GetDynamicParameter().ballAngle)) < 0.7f);
				}
			}
		}

		if (isPlaying)
		{
			float deltaTime = clock.restart().asSeconds();

			// Move the player's paddle
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
				(leftPaddle.GetPostion().y - leftPaddle.GetParameter().pHeight/ 2 > 5.f))
			{
				leftPaddle.Move(0.f, (-leftPaddle.GetDynamicParameter().paddleSpeed * deltaTime));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
				(leftPaddle.GetPostion().y + leftPaddle.GetParameter().pHeight / 2 < gameHeight - 5.f))
			{
				leftPaddle.Move(0.f, (leftPaddle.GetDynamicParameter().paddleSpeed * deltaTime));
			}

			// Move the computer's paddle (if b/w 5 pixels from top and bottom of screen)
			if (((rightPaddle.GetDynamicParameter().paddleSpeed  < 0.f) && (rightPaddle.GetPostion().y - rightPaddle.GetParameter().pHeight / 2 > 5.f)) ||
				((rightPaddle.GetDynamicParameter().paddleSpeed   > 0.f) && (rightPaddle.GetPostion().y + rightPaddle.GetParameter().pHeight / 2 < gameHeight - 5.f)))
			{
				float pfactor = rightPaddle.GetDynamicParameter().paddleSpeed * deltaTime;
				rightPaddle.CMove(0.f, (rightPaddle.GetDynamicParameter().paddleSpeed  * deltaTime));
			}

			//Move Obstacle Paddle 
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
				(oPaddle.GetPostion().y + oPaddle.GetParameter().pHeight / 2 < gameHeight + 5.f))
			{
				oPaddle.Move(0.f, (oPaddle.GetDynamicParameter().paddleSpeed * deltaTime));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
				(oPaddle.GetPostion().y - oPaddle.GetParameter().pHeight / 2 > 0))
			{
				oPaddle.Move(0.f, (-oPaddle.GetDynamicParameter().paddleSpeed * deltaTime));
			}
			

			// Update the computer's paddle direction according to the ball position
			if (AITimer.getElapsedTime() > AITime)
			{

				AITimer.restart();
				if (ball.GetPostion().y + ball.GetParameter().ballRadius > rightPaddle.GetPostion().y + rightPaddle.GetParameter().pHeight / 2)
				{
					Paddle::DynamicParam nState = rightPaddle.GetDynamicParameter();
					nState.paddleSpeed = 300; // change paddle speed here
					rightPaddle.SetDynamicParameter(nState);
				}
				else if (ball.GetPostion().y - ball.GetParameter().ballRadius < rightPaddle.GetPostion().y - rightPaddle.GetParameter().pHeight / 2)
				{
					Paddle::DynamicParam nState = rightPaddle.GetDynamicParameter();
					nState.paddleSpeed = -300; // change paddle speed here
					rightPaddle.SetDynamicParameter(nState);
				}
				else
				{
					Paddle::DynamicParam nState = rightPaddle.GetDynamicParameter();
					nState.paddleSpeed = 0.0f; // change paddle speed here [Most frequent cond. set it to 400 is working]
					rightPaddle.SetDynamicParameter(nState);
				}

				/*Obstacle Move
				if (ball.GetPostion().y - ball.GetParameter().ballRadius < oPaddle.GetPostion().y &&
					(oPaddle.GetPostion().y + oPaddle.GetParameter().pHeight / 2 < gameHeight + 5.f))
				{
					oPaddle.Move(0.f, (oPaddle.GetDynamicParameter().paddleSpeed * deltaTime));
				}
				if (ball.GetPostion().y + ball.GetParameter().ballRadius > oPaddle.GetPostion().y &&
					(oPaddle.GetPostion().y - oPaddle.GetParameter().pHeight / 2 > 0))
				{
					oPaddle.Move(0.f, (-oPaddle.GetDynamicParameter().paddleSpeed * deltaTime));
				}*/
			}

			// Move the ball
			float factor = ball.GetDynamicParameter().ballSpeed * deltaTime;
			float oX = std::cos(ball.GetDynamicParameter().ballAngle) * factor;
			float oY = std::sin(ball.GetDynamicParameter().ballAngle) * factor;
			ball.Move(oX, oY);

			//Win Cond
			if (p2score == 5 || p1score == 5)
			{
				if (p1score == 5)
				{
					isPlaying = false;
					pauseMessage.setString("You Won!\nPress space to restart or\nescape to exit");
				}
				else if (p2score == 5)
				{
					isPlaying = false;
					pauseMessage.setString("You Lost!\nPress space to restart or\nescape to exit");
				}
			}
			
			
			// Check collisions between the ball and the screen
			//Player Loses
			if (ball.GetPostion().x - ball.GetParameter().ballRadius < 0.f)
			{
				p2score = p2score + 1;
				std::string nScore = std::to_string(p2score);
				score2.setString(nScore);
				bool pHit = false;
				int hitCount = 0;
				Paddle::Position lPaddlePos;
				lPaddlePos.x = 10 + leftPaddle.GetParameter().pWidth / 2;
				lPaddlePos.y = gameHeight / 2;
				leftPaddle.SetPosition(lPaddlePos);
				Paddle::Parameter lpaddleRest;
				lpaddleRest.pHeight = 100;
				lpaddleRest.fColor = sf::Color::Red;
				leftPaddle.SetParameter(lpaddleRest);

				Paddle::Position rPaddlePos;
				rPaddlePos.x = gameWidth - 10 - rightPaddle.GetParameter().pWidth / 2;
				rPaddlePos.y = gameHeight / 2;
				rightPaddle.SetPosition(rPaddlePos);
				Paddle::Parameter rpaddleRest;
				rpaddleRest.pHeight = 100;
				rpaddleRest.fColor = sf::Color::Blue;
				rightPaddle.SetParameter(rpaddleRest);

				Paddle::Position oPaddlePos;
				oPaddlePos.x = gameWidth / 2 + oPaddle.GetParameter().pWidth / 2;
				oPaddlePos.y = gameHeight / 2 ;
				oPaddle.SetPosition(oPaddlePos);

				Ball::Position originPos;
				originPos.x = gameWidth / 2;
				originPos.y = gameHeight / 2;
				ball.SetPosition(originPos);

				do
				{
					// Make sure the ball initial angle is not too much vertical
					Ball::DynamicParam dp = ball.GetDynamicParameter();
					dp.ballAngle = (std::rand() % 360) * 2 * pi / 360;
					dp.ballSpeed = 400.0f;
					ball.SetDynamicParameter(dp);
				} while (std::abs(std::cos(ball.GetDynamicParameter().ballAngle)) < 0.7f);
			}
			if (ball.GetPostion().x + ball.GetParameter().ballRadius > gameWidth)
			{
				p1score = p1score + 1;
				std::string nScore = std::to_string(p1score);
				score1.setString(nScore);
				bool pHit = false;
				int hitCount = 0;
				Paddle::Position lPaddlePos;
				lPaddlePos.x = 10 + leftPaddle.GetParameter().pWidth / 2;
				lPaddlePos.y = gameHeight / 2;
				leftPaddle.SetPosition(lPaddlePos);
				Paddle::Parameter lpaddleRest;
				lpaddleRest.pHeight = 100;
				lpaddleRest.fColor = sf::Color::Red;
				leftPaddle.SetParameter(lpaddleRest);

				Paddle::Position rPaddlePos;
				rPaddlePos.x = gameWidth - 10 - rightPaddle.GetParameter().pWidth / 2;
				rPaddlePos.y = gameHeight / 2;
				rightPaddle.SetPosition(rPaddlePos);
				Paddle::Parameter rpaddleRest;
				rpaddleRest.pHeight = 100;
				rpaddleRest.fColor = sf::Color::Blue;
				rightPaddle.SetParameter(rpaddleRest);

				Paddle::Position oPaddlePos;
				oPaddlePos.x = gameWidth / 2 + oPaddle.GetParameter().pWidth / 2;
				oPaddlePos.y = gameHeight / 2;
				oPaddle.SetPosition(oPaddlePos);

				Ball::Position originPos;
				originPos.x = gameWidth / 2;
				originPos.y = gameHeight / 2;
				ball.SetPosition(originPos);

				do
				{
					// Make sure the ball initial angle is not too much vertical
					Ball::DynamicParam dp = ball.GetDynamicParameter();
					dp.ballAngle = (std::rand() % 360) * 2 * pi / 360;
					dp.ballSpeed = 400.0f;
					ball.SetDynamicParameter(dp);
				} while (std::abs(std::cos(ball.GetDynamicParameter().ballAngle)) < 0.7f);
			}
			
			
			//Top Bounce
			if (ball.GetPostion().y - ball.GetParameter().ballRadius < 0.f)
			{
				ballSound.play();
				Ball::DynamicParam dp = ball.GetDynamicParameter();
				dp.ballAngle = -dp.ballAngle;
				ball.SetDynamicParameter(dp);

				Ball::Position newPos;
				newPos.y = ball.GetParameter().ballRadius + 0.1f;
				newPos.x = ball.GetPostion().x;
				ball.SetPosition(newPos);
			}
			//Bottom Bounce
			if (ball.GetPostion().y + ball.GetParameter().ballRadius > gameHeight)
			{
				ballSound.play();
				Ball::DynamicParam dp = ball.GetDynamicParameter();
				dp.ballAngle = -dp.ballAngle;
				ball.SetDynamicParameter(dp);

				Ball::Position newPos;
				newPos.y = gameHeight - ball.GetParameter().ballRadius - 0.1f;
				newPos.x = ball.GetPostion().x;
				ball.SetPosition(newPos);
			}

			// Check the collisions between the ball and the paddles
			// Left Paddle
			if (ball.GetPostion().x - ball.GetParameter().ballRadius < leftPaddle.GetPostion().x + leftPaddle.GetParameter().pWidth/ 2 &&
				ball.GetPostion().x - ball.GetParameter().ballRadius > leftPaddle.GetPostion().x &&
				ball.GetPostion().y + ball.GetParameter().ballRadius >= leftPaddle.GetPostion().y - leftPaddle.GetParameter().pHeight / 2 &&
				ball.GetPostion().y - ball.GetParameter().ballRadius <= leftPaddle.GetPostion().y + leftPaddle.GetParameter().pHeight / 2)
			{
				if (ball.GetPostion().y > leftPaddle.GetPostion().y)
				{
					Ball::DynamicParam dp=ball.GetDynamicParameter();
					dp.ballAngle = pi - dp.ballAngle + (std::rand() % 20) * pi / 180;
					ball.SetDynamicParameter(dp);
				}
				else
				{
					Ball::DynamicParam dp = ball.GetDynamicParameter();
					dp.ballAngle = pi - dp.ballAngle - (std::rand() % 20) * pi / 180;
					ball.SetDynamicParameter(dp);
				}

				ballSound.play();
				pHit = true;
				hitCount = hitCount + 1;
				Ball::Position newPos;
				newPos.y = ball.GetPostion().y;
				newPos.x = leftPaddle.GetPostion().x + ball.GetParameter().ballRadius + leftPaddle.GetParameter().pWidth / 2 + 0.1f;
				ball.SetPosition(newPos);
				Ball::DynamicParam dp = ball.GetDynamicParameter();
				dp.ballSpeed = dp.ballSpeed + 5;
				ball.ChangeAngle(true);				//Makes sure it doesn't travel in a straight line to the left
				ball.SetDynamicParameter(dp);
			}

			// Right Paddle
			if (ball.GetPostion().x + ball.GetParameter().ballRadius > rightPaddle.GetPostion().x - rightPaddle.GetParameter().pWidth/ 2 &&
				ball.GetPostion().x + ball.GetParameter().ballRadius < rightPaddle.GetPostion().x &&
				ball.GetPostion().y + ball.GetParameter().ballRadius >= rightPaddle.GetPostion().y - rightPaddle.GetParameter().pHeight / 2 &&
				ball.GetPostion().y - ball.GetParameter().ballRadius <= rightPaddle.GetPostion().y + rightPaddle.GetParameter().pHeight / 2)
			{
				if (ball.GetPostion().y > rightPaddle.GetPostion().y)
				{
					Ball::DynamicParam dp = ball.GetDynamicParameter();
					dp.ballAngle= pi - dp.ballAngle + (std::rand() % 15) * pi / 180; //Learn the Math
					ball.SetDynamicParameter(dp);
				}
				else
				{
					Ball::DynamicParam dp = ball.GetDynamicParameter();
					dp.ballAngle = pi - dp.ballAngle - (std::rand() % 15) * pi / 180;// Learn the Math
					ball.SetDynamicParameter(dp);
				}

				ballSound.play();
				pHit = false;
				hitCount = hitCount + 1;
				Ball::Position newPos;
				newPos.y = ball.GetPostion().y;
				newPos.x = rightPaddle.GetPostion().x - ball.GetParameter().ballRadius- rightPaddle.GetParameter().pWidth / 2 - 0.1f;
				ball.SetPosition(newPos);
				Ball::DynamicParam dp = ball.GetDynamicParameter();
				dp.ballSpeed = dp.ballSpeed + 5;
				ball.ChangeAngle(true);
				ball.SetDynamicParameter(dp);
			}

			// Obstacle Paddle Collisions
			// Left Side Hit for obstacle
			if (ball.GetPostion().x - ball.GetParameter().ballRadius < oPaddle.GetPostion().x + oPaddle.GetParameter().pWidth / 2 &&
				ball.GetPostion().x - ball.GetParameter().ballRadius > oPaddle.GetPostion().x &&
				ball.GetPostion().y + ball.GetParameter().ballRadius >= oPaddle.GetPostion().y - oPaddle.GetParameter().pHeight / 2 &&
				ball.GetPostion().y - ball.GetParameter().ballRadius <= oPaddle.GetPostion().y + oPaddle.GetParameter().pHeight / 2)
			{
				if (ball.GetPostion().y > oPaddle.GetPostion().y)
				{
					Ball::DynamicParam dp = ball.GetDynamicParameter();
					dp.ballAngle = pi - dp.ballAngle + (std::rand() % 20) * pi / 180;
					ball.SetDynamicParameter(dp);
				}
				else
				{
					Ball::DynamicParam dp = ball.GetDynamicParameter();
					dp.ballAngle = pi - dp.ballAngle - (std::rand() % 20) * pi / 180;
					ball.SetDynamicParameter(dp);
				}
				if (bigSize)
				{
					if (pHit)
					{
						//double player paddle size
						Paddle::Parameter bigger = leftPaddle.GetParameter();
						bigger.pHeight = 500;
						leftPaddle.SetParameter(bigger);
						
					}
					else
					{
						//double ai paddle size
						Paddle::Parameter bigger = rightPaddle.GetParameter();
						bigger.pHeight = 500;
						rightPaddle.SetParameter(bigger);
					}
					bigSize = false;
					oPaddle.setTexture(&def, true);
				}
				ballSound.play();
				Ball::Position newPos;
				newPos.y = ball.GetPostion().y;
				newPos.x = oPaddle.GetPostion().x + ball.GetParameter().ballRadius + oPaddle.GetParameter().pWidth / 2 + 0.1f;
				ball.SetPosition(newPos);
				Ball::DynamicParam dp = ball.GetDynamicParameter();
				dp.ballSpeed = dp.ballSpeed + 5;
				ball.ChangeAngle(true);				//Makes sure it doesn't travel in a straight line to the left
				ball.SetDynamicParameter(dp);
			}
			// Right Side Hit for obstacle
			if (ball.GetPostion().x + ball.GetParameter().ballRadius > oPaddle.GetPostion().x - oPaddle.GetParameter().pWidth / 2 &&
				ball.GetPostion().x + ball.GetParameter().ballRadius < oPaddle.GetPostion().x &&
				ball.GetPostion().y + ball.GetParameter().ballRadius >= oPaddle.GetPostion().y - oPaddle.GetParameter().pHeight / 2 &&
				ball.GetPostion().y - ball.GetParameter().ballRadius <= oPaddle.GetPostion().y + oPaddle.GetParameter().pHeight / 2)
			{
				if (ball.GetPostion().y > oPaddle.GetPostion().y)
				{
					Ball::DynamicParam dp = ball.GetDynamicParameter();
					dp.ballAngle = pi - dp.ballAngle + (std::rand() % 15) * pi / 180; //Learn the Math
					ball.SetDynamicParameter(dp);
				}
				else
				{
					Ball::DynamicParam dp = ball.GetDynamicParameter();
					dp.ballAngle = pi - dp.ballAngle - (std::rand() % 15) * pi / 180;// Learn the Math
					ball.SetDynamicParameter(dp);
				}
				if (bigSize)
				{
					if (pHit)
					{
						//double player paddle size
						Paddle::Parameter bigger = leftPaddle.GetParameter();
						bigger.pHeight = 200;
						leftPaddle.SetParameter(bigger);

					}
					else
					{
						//double ai paddle size
						Paddle::Parameter bigger = rightPaddle.GetParameter();
						bigger.pHeight = 200;
						rightPaddle.SetParameter(bigger);
					}
					bigSize = false;
					oPaddle.setTexture(&def,true);
				}
				ballSound.play();
				Ball::Position newPos;
				newPos.y = ball.GetPostion().y;
				newPos.x = oPaddle.GetPostion().x - ball.GetParameter().ballRadius - oPaddle.GetParameter().pWidth / 2 - 0.1f;
				ball.SetPosition(newPos);
				Ball::DynamicParam dp = ball.GetDynamicParameter();
				dp.ballSpeed = dp.ballSpeed + 5;
				ball.ChangeAngle(true);
				ball.SetDynamicParameter(dp);
			}
			//Top and Bottom Collision for Obstacle
			if (ball.GetPostion().y - ball.GetParameter().ballRadius ==oPaddle.GetPostion().y+oPaddle.GetParameter().pHeight/2)
			{
				ballSound.play();
				Ball::DynamicParam dp = ball.GetDynamicParameter();
				dp.ballAngle = -dp.ballAngle;
				ball.SetDynamicParameter(dp);

				Ball::Position newPos;
				newPos.y = ball.GetParameter().ballRadius + 0.1f;
				newPos.x = ball.GetPostion().x;
				ball.SetPosition(newPos);
			}
			if (ball.GetPostion().y + ball.GetParameter().ballRadius == oPaddle.GetPostion().y + oPaddle.GetParameter().pHeight / 2)
			{
				ballSound.play();
				Ball::DynamicParam dp = ball.GetDynamicParameter();
				dp.ballAngle = -dp.ballAngle;
				ball.SetDynamicParameter(dp);

				Ball::Position newPos;
				newPos.y = gameHeight - ball.GetParameter().ballRadius - 0.1f;
				newPos.x = ball.GetPostion().x;
				ball.SetPosition(newPos);
			}
			
		}


			// Clear the window
			window.clear(sf::Color(50, 200, 50));

			if (isPlaying)
			{
				// Draw the paddles and the ball
				window.draw(sprite);
				window.draw(score1);
				window.draw(score2);
				if (hitCount == 5)
				{
					bigSize = true;
				}
				if (bigSize)
				{
					oPaddle.setTexture(&powerup, true);

				}
				else if(!bigSize)
				{
					oPaddle.setTexture(&def, true);
				}

				leftPaddle.Draw(&window);
				oPaddle.Draw(&window);
				rightPaddle.Draw(&window);
				ball.Draw(&window);
			}
			else
			{
				// Draw the pause message
				window.draw(mainsprite);
				window.draw(pauseMessage);
			}

			// Display things on screen
			window.display();
	}
	
	return EXIT_SUCCESS;
}


