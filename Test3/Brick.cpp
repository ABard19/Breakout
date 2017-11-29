#include "Brick.h"

sf::RectangleShape Brick::GetBrick() const
{
	return brick;
}

Brick::Brick()
{
	Parameter b;
	SetParameter(b);
}

Brick::Brick(const Parameter &b)
{
	SetParameter(b);
}
Brick::Parameter Brick::GetParameter()const
{
	return m_b;
}
void Brick::SetParameter(const Parameter &b)
{
	m_b = b;
	brick.setSize(sf::Vector2f(b.bWidth, b.bHeight));
	brick.setOrigin(b.bWidth / 2, b.bHeight);
	brick.setOutlineThickness(b.oThickness);
	brick.setOutlineColor(b.oColor);
	brick.setFillColor(b.fColor);
}

void Brick::SetDynamicParameter(const DynamicParam &b)
{
	m_db = b;
}

Brick::DynamicParam Brick::GetDynamicParameter() const
{
	return m_db;
}


void Brick::Move(float oX, float  oY)
{
	brick.move(oX, oY);
}


void Brick::SetPosition(const Position &position)
{
	brick.setPosition(sf::Vector2f(position.x, position.y));
}


Brick::Position Brick::GetPostion() const
{
	Position bp;
	bp.x = brick.getPosition().x;
	bp.y = brick.getPosition().y;
	return bp;
}
void Brick::Draw(sf::RenderWindow* win)
{
	win->draw(brick);
}

void Brick::setTexture(const sf::Texture * texture, bool resetRect)
{
	brick.setTexture(texture, true);
}
