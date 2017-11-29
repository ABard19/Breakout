#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
public:
	struct Parameter
	{
		float pWidth=100;
		float pHeight = 25;
		float oThickness = 3.0f;
		sf::Color oColor = sf::Color::Black;
		sf::Color fColor = sf::Color::White;
		float originX = pWidth / 2;
		float originY = pHeight / 2;
	};

	struct DynamicParam
	{
		float paddleSpeed = 400.0f;
	};

	struct Position
	{
		float x = -1;
		float y = -1;
	};

	sf::RectangleShape GetPaddle() const;
	Position GetPostion() const;
	void SetPosition(const Position &position);
	Parameter GetParameter() const;
	DynamicParam GetDynamicParameter() const;
	void SetParameter(const Parameter &p);
	void SetDynamicParameter(const DynamicParam &p);
	Paddle(const Parameter &p);
	Paddle();
	void Reset();
	void Move(float oX, float oY);
	void CMove(float oX, float oY);
	void Draw(sf::RenderWindow* win);
	void setTexture(const sf::Texture* texture, bool resetRect = false);

private:
	sf::RectangleShape paddle;
	DynamicParam m_dp;
	Parameter m_p;
};