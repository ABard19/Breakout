#include"Ball.h"

Ball::Ball()
{
	Parameter p;
	SetParameter(p);
}

Ball::Ball(const Parameter &p)
{
	SetParameter(p);
}

void Ball::SetParameter(const Parameter &p)
{
	ball.setRadius(p.ballRadius - 3);
	ball.setOutlineThickness(p.oThickness);
	ball.setOutlineColor(p.oColor);
	ball.setFillColor(p.fColor);
	ball.setOrigin(p.OriginX / 2, p.OriginY / 2);
}
sf::CircleShape Ball::GetBall() const
{

	return ball;
}

void Ball::SetDynamicParameter(const DynamicParam &p)
{
	dp = p;
}

Ball::DynamicParam Ball::GetDynamicParameter() const
{
	return dp;
}
void Ball::Move(float oX, float  oY)
{
	ball.move(oX,oY);
}

Ball::Position Ball::GetPostion() const
{
	Position p;
	p.x = ball.getPosition().x;
	p.y = ball.getPosition().y;
	return p;
}

void Ball::SetPosition(const Position &position)
{
	ball.setPosition(sf::Vector2f(position.x, position.y));
}

Ball::Parameter Ball::GetParameter()const
{
	return p;
}


void Ball::ChangeAngle(const bool hit)
{
	if (hit)
	{
		dp.ballAngle= 3.14 - dp.ballAngle + (std::rand() % 20) * 3.14 / 180;
	}
}

void Ball::Reset()
{
	dp.ballAngle = (std::rand() % 360) * 2 * 3.14 / 360;
	dp.ballSpeed = 250.0f;
}

void Ball::Draw(sf::RenderWindow* win)
{
	win->draw(ball);
}
