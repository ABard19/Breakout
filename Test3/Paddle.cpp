#include "Paddle.h"

sf::RectangleShape Paddle::GetPaddle() const
{
	return paddle;
}

Paddle::Paddle()
{
	Parameter p;
	SetParameter(p);
}

Paddle::Paddle(const Parameter &p)
{
	SetParameter(p);
}
Paddle::Parameter Paddle::GetParameter()const
{
	return m_p;
}
void Paddle::SetParameter(const Parameter &p)
{
	m_p = p;
	paddle.setSize(sf::Vector2f(p.pWidth,p.pHeight));
	paddle.setOrigin(p.pWidth/2, p.pHeight);
	paddle.setOutlineThickness(p.oThickness);
	paddle.setOutlineColor(p.oColor);
	paddle.setFillColor(p.fColor);
}

void Paddle::SetDynamicParameter(const DynamicParam &p)
{
	m_dp = p;
}

Paddle::DynamicParam Paddle::GetDynamicParameter() const
{
	return m_dp;
}


void Paddle::Move(float oX, float  oY)
{
	paddle.move(oX, oY);
}

void Paddle::CMove(float oX, float  oY)
{
	paddle.move(oX, oY);
}

void Paddle::SetPosition(const Position &position)
{
	paddle.setPosition(sf::Vector2f(position.x, position.y));
}


Paddle::Position Paddle::GetPostion() const
{
	Position p;
	p.x = paddle.getPosition().x;
	p.y = paddle.getPosition().y;
	return p;
}
void Paddle::Draw(sf::RenderWindow* win)
{
	win->draw(paddle);
}

void Paddle::setTexture(const sf::Texture * texture, bool resetRect)
{
	paddle.setTexture(texture, true);
}
