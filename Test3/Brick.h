#pragma once
#include <SFML/Graphics.hpp>

class Brick
{
public:
	struct Parameter
	{
		float bWidth = 50;
		float bHeight = 20;
		float oThickness = 0.1f;
		sf::Color oColor = sf::Color::Black;
		sf::Color fColor = sf::Color::White;
		float originX = bWidth / 2;
		float originY = bHeight / 2;
//		float angle = 90.0f;
		int hitpt = 1;
		bool destroyed = false;
	};

	struct DynamicParam
	{
		float brickSpeed = 200.0f;
	};

	struct Position
	{
		float x = -1;
		float y = -1;
	};

	sf::RectangleShape GetBrick() const;
	Position GetPostion() const;
	void SetPosition(const Position &position);
	Parameter GetParameter() const;
	DynamicParam GetDynamicParameter() const;
	void SetParameter(const Parameter &b);
	void SetDynamicParameter(const DynamicParam &b);
	Brick();
	Brick(const Parameter &b);
	void Move(float oX, float oY);
	void Draw(sf::RenderWindow* win);
	void setTexture(const sf::Texture* texture, bool resetRect = false);

private:
	sf::RectangleShape brick;
	DynamicParam m_db;
	Parameter m_b;
};