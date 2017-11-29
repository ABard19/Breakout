#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<string>
#include<iostream>

class Ball
{
public:
	struct Parameter
	{
		float ballRadius = 12.f;
		float oThickness = 0.33f;
		sf::Color oColor=sf::Color::Black;
		sf::Color fColor = sf::Color::White;
		float OriginX = ballRadius / 2;
		float OriginY = ballRadius / 2;
	};

	struct DynamicParam
	{
		float ballSpeed = 400.0f; //Incremented with every hit
		float ballAngle = 0.f; // to be changed later
	};

	struct Position
	{
		float x=-1;
		float y=-1;
	};

	sf::CircleShape GetBall() const;
	Position GetPostion() const;
	void SetPosition(const Position &position);
	Parameter GetParameter() const;
	DynamicParam GetDynamicParameter() const;
	void SetParameter(const Parameter &p);
	void SetDynamicParameter(const DynamicParam &p);
	Ball(const Parameter &p);
	Ball();
	void Reset();
	void ChangeAngle(const bool hit);
	void Move(float oX, float oY);
	void Draw(sf::RenderWindow* win);
private:
	sf::CircleShape ball;

	DynamicParam dp;
	Parameter p;
};




