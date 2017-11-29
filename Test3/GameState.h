#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<string>
#include<iostream>
#include"Paddle.h"
#include"Ball.h"
#include<string.h>
#include"Brick.h"
#include<vector>




class GameState
{
public:
	GameState(Paddle &paddle, Ball &ball, sf::Sound ballSound);
	void Reset(Paddle &paddle, Ball &ball, int gameWidth, int gameHeight, int level, int lives);
	void MoveBall(Ball &ball, Paddle &paddle, bool fireb, float dt);
	void MovePaddle(Paddle &paddle, float dt);
	void WallBounce(Ball &ball, int gameWidth, int gameHeight, sf::Sound &m_ballSound);
	void PaddleCollide(Ball &ball, Paddle &paddle, sf::Sound &m_paddleSound, bool fireb);
	void BrickDrawI(std::vector<Brick>& brick, int gameWidth, int gameHeight);
	void BrickDrawII(std::vector<Brick>& bricks, int gameWidth, int gameHeight, sf::Texture &brickT1, sf::Texture &brickT);
	void BrickDrawIII(std::vector<Brick>& bricks, int gameWidth, int gameHeight, sf::Texture &brickT1, sf::Texture &brickT, sf::Texture &brickT3);
	int BrickCollide(std::vector<Brick>&brick, Ball &ball, sf::Sound &m_brickSound, int score, Paddle &paddle, sf::Sound &damage);
	int ScoreUp();
private:
	Paddle m_paddle;
	Ball m_ball;
	int gameWidth = 800.0f;
	int gameHeight = 600.0f;
	int score;
	bool fireb;
	float dt;
	int level;
	sf::Sound m_ballSound;
	sf::Sound m_brickSound;
	sf::Sound m_paddleSound;

};