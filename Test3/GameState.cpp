#include"GameState.h"

GameState::GameState( Paddle &paddle, Ball &ball, sf::Sound ballSound)
{
	m_paddle = paddle;
	m_ball = ball;
	m_ballSound = ballSound;
}

void GameState::Reset(Paddle &paddle, Ball &ball, int gameWidth, int gameHeight, int level, int lives)
{
	Paddle::Position lPaddlePos;
	lPaddlePos.x = gameWidth / 2;
	lPaddlePos.y = gameHeight - 20.0f;
	paddle.SetPosition(lPaddlePos);

	Ball::Position nPos;
	nPos.x = paddle.GetPostion().x + paddle.GetParameter().pWidth / 2 + 5.0f;
	nPos.y = paddle.GetPostion().y - ball.GetParameter().ballRadius - 15.0f;
	ball.SetPosition(nPos);
	Ball::DynamicParam dp = ball.GetDynamicParameter();
	dp.ballAngle = -165;
	ball.SetDynamicParameter(dp);
}

void GameState::BrickDrawI(std::vector<Brick>& brick, int gameWidth, int gameHeight)
{
	Brick::Position bpos;
	bpos.y = (gameHeight / 2) - 100.0f;
	bpos.x = 100.0f;
	for (int i = 0; i < 16; i++)
	{
		if (i < 8)
		{
			bpos.x = 100.0f + i * 55;
		}
		else if(i>=8)
		{
			bpos.y = (gameHeight / 2) - 75.0f;
			bpos.x = 100.0f + ((i - 8) * 55);
		}
		brick[i].SetPosition(bpos);
	}
}

void GameState::BrickDrawII(std::vector<Brick>& bricks, int gameWidth, int gameHeight, sf::Texture &brickT1, sf::Texture &brickT)
{
	Brick::Position bpos;
	bpos.y = (gameHeight / 2) - 100.0f;
	bpos.x = 100.0f;
	for (int i = 0; i < 24; i++)
	{
		bricks[i].setTexture(&brickT, false);
		if (i < 8)
		{
			bpos.x = 100.0f + i * 55;
		}
		else if (i >= 8 && i<=16)
		{
			bpos.y = (gameHeight / 2) - 60.0f;
			bpos.x = 100.0f +((i - 8) * 55);
		}
		else if (i >= 16)
		{
			bricks[i].setTexture(&brickT1, false);
			Brick::Parameter brickprop = bricks[i].GetParameter();
			brickprop.hitpt = brickprop.hitpt+1;
			brickprop.fColor = sf::Color::Yellow;
			bricks[i].SetParameter(brickprop);
			bpos.y = (gameHeight / 2) - 20.0f;
			bpos.x = 100.0f + ((i - 16) * 55);
		}
		bricks[i].SetPosition(bpos);
	}
}

void GameState::BrickDrawIII(std::vector<Brick>& bricks, int gameWidth, int gameHeight, sf::Texture &brickT1, sf::Texture &brickT, sf::Texture &brickT3)
{
	Brick::Position bpos;
	bpos.y = (gameHeight / 2);
	bpos.x = 250.0f;
	for (int i = 0; i < 32; i++)
	{
		if (i < 8)
		{
			bricks[i].setTexture(&brickT, false);
			bpos.y = 25.0f + i * 25;
		}
		else if (i >= 8 && i < 16)
		{
			bricks[i].setTexture(&brickT1, false);
			Brick::Parameter brickprop = bricks[i].GetParameter();
			brickprop.hitpt = brickprop.hitpt + 1;
			bricks[i].SetParameter(brickprop);
			bpos.y = 25.0f+ ((i - 8) * 25);
			bpos.x = 400 ;
		}
		else if (i >= 16 && i < 24)
		{
			bricks[i].setTexture(&brickT1, false);
			Brick::Parameter brickprop = bricks[i].GetParameter();
			brickprop.hitpt = brickprop.hitpt + 1;
			bricks[i].SetParameter(brickprop);
			bpos.y = 25.0f+((i-16)*25);
			bpos.x = gameWidth-300.0f;
		}
		else if (i >= 24)
		{
			bricks[i].setTexture(&brickT3, false);
			bpos.y = 25.0f + ((i - 24) * 25);
			bpos.x = gameWidth - 100.0f;
		}
		bricks[i].SetPosition(bpos);
	}
}
int GameState::BrickCollide(std::vector<Brick>&brick, Ball &ball, sf::Sound &m_brickSound, int score, Paddle &paddle, sf::Sound &damage)
{
	std::vector<int> hit;
	std::vector<int> nhit;
	for (int i = 0; i < brick.size(); i++)
	{
		bool brickhit = false;
		if (ball.GetPostion().x + ball.GetParameter().ballRadius >= brick[i].GetPostion().x - brick[i].GetParameter().bWidth / 2 &&
			ball.GetPostion().x - ball.GetParameter().ballRadius <= brick[i].GetPostion().x + brick[i].GetParameter().bWidth / 2
			&& ball.GetPostion().y + ball.GetParameter().ballRadius > brick[i].GetPostion().y-brick[i].GetParameter().bHeight/2
				&& ball.GetPostion().y - ball.GetParameter().ballRadius < brick[i].GetPostion().y+ brick[i].GetParameter().bHeight/2)
		{
			brickhit = true;
			if (brickhit)
			{
				brickhit = false;
				Brick::Parameter bp = brick[i].GetParameter();
				bp.hitpt = bp.hitpt - 1;
				Ball::DynamicParam dp = ball.GetDynamicParameter();
				dp.ballAngle = -dp.ballAngle;
				dp.ballAngle = 3.14f - dp.ballAngle + (std::rand() % 20) * 3.14f / 180;
				ball.ChangeAngle(true);
				ball.SetDynamicParameter(dp);
				if (bp.hitpt == 0)
				{
					bp.destroyed = true;
				}
				if (bp.destroyed)
				{
					m_brickSound.play();
					hit.push_back(i);
					bp.fColor = sf::Color::Black;
					score = score + 100;
					if (i > 24)
					{
						Ball::DynamicParam bchange = ball.GetDynamicParameter();
						bchange.ballSpeed = bchange.ballSpeed + 25.0f;
						ball.SetDynamicParameter(bchange);
						Paddle::Parameter pchange = paddle.GetParameter();
						pchange.pWidth = pchange.pWidth - 7.0f;
						paddle.SetParameter(pchange);
					}
				}
				else
				{
					damage.play();
				}
				brick[i].SetParameter(bp);
			}
			
		}
		if(brick[i].GetParameter().hitpt != 0)
		{
				nhit.push_back(i);
		}
	}
	std::vector<Brick>br;
	for (int j = 0; j < nhit.size(); j++)
	{
		br.push_back(brick[nhit[j]]);
	}
	if (br.size() > 0 || ((score==1600 ||score==8800) || (score==4000 || score==11200) || (score==7200||score==144000))) // or if it is the last brick find a better way to tell if it's the last brick
	{
		brick.clear();
		brick = br;
	}

	return score;
}


void GameState::MoveBall(Ball &ball, Paddle &paddle, bool fireb, float dt)
{
	if (fireb)
	{
		float factor = ball.GetDynamicParameter().ballSpeed * dt;
		float oX = std::cos(ball.GetDynamicParameter().ballAngle) * factor;
		float oY = std::sin(ball.GetDynamicParameter().ballAngle) * factor;
		ball.Move(oX, oY);
	}
	else if (!fireb)
	{
		Ball::Position nPos;
		nPos.x = paddle.GetPostion().x + paddle.GetParameter().pWidth / 2 + 5.0f;
		nPos.y = paddle.GetPostion().y - ball.GetParameter().ballRadius - 15.0f;
		ball.SetPosition(nPos);
		Ball::DynamicParam dp = ball.GetDynamicParameter();
		dp.ballAngle = -165;
		ball.SetDynamicParameter(dp);
	}
}

void GameState::MovePaddle(Paddle &paddle, float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		(paddle.GetPostion().x - paddle.GetParameter().pWidth / 2 > 5.f))
	{
		paddle.Move((-paddle.GetDynamicParameter().paddleSpeed * dt), 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
		(paddle.GetPostion().x + paddle.GetParameter().pWidth / 2 < gameWidth - 5.f))
	{
		paddle.Move((paddle.GetDynamicParameter().paddleSpeed * dt), 0.f);
	}
}

void GameState::WallBounce(Ball &ball, int gameWidth, int gameHeight, sf::Sound &m_ballSound)
{
	//Top Bounce
	if (ball.GetPostion().y - ball.GetParameter().ballRadius < 0.f)
	{
		m_ballSound.play();
		Ball::DynamicParam dp = ball.GetDynamicParameter();
		dp.ballAngle = -dp.ballAngle;
		ball.SetDynamicParameter(dp);

		Ball::Position newPos;
		newPos.y = ball.GetParameter().ballRadius + 0.1f;
		newPos.x = ball.GetPostion().x;
		ball.SetPosition(newPos);
	}
	//Right Bounce
	if (ball.GetPostion().x + ball.GetParameter().ballRadius >= gameWidth)
	{
		m_ballSound.play();
		Ball::Position newPos;
		newPos.y = ball.GetPostion().y;
		newPos.x = gameWidth - ball.GetParameter().ballRadius - 0.1f;
		ball.SetPosition(newPos);
		Ball::DynamicParam dp = ball.GetDynamicParameter();
		dp.ballAngle = 3.14f - dp.ballAngle + (std::rand() % 20) * 3.14f / 180;
		ball.ChangeAngle(true);
		ball.SetDynamicParameter(dp);
	}
	//Left Bounce
	if (ball.GetPostion().x + ball.GetParameter().ballRadius < 0.0f)
	{
		m_ballSound.play();
		Ball::Position newPos;
		newPos.y = ball.GetPostion().y;
		newPos.x = (gameWidth - gameWidth) + ball.GetParameter().ballRadius + 0.1f;
		ball.SetPosition(newPos);
		Ball::DynamicParam dp = ball.GetDynamicParameter();
		dp.ballAngle = 3.14f - dp.ballAngle + (std::rand() % 20) * 3.14f / 180;
		ball.ChangeAngle(true);
		ball.SetDynamicParameter(dp);
	}
}

void GameState::PaddleCollide(Ball &ball, Paddle &paddle, sf::Sound &m_paddleSound, bool fireb)
{
	if (ball.GetPostion().x + ball.GetParameter().ballRadius >= paddle.GetPostion().x - paddle.GetParameter().pWidth / 2 &&
		ball.GetPostion().x - ball.GetParameter().ballRadius <= paddle.GetPostion().x + paddle.GetParameter().pWidth / 2
		&& ball.GetPostion().y + ball.GetParameter().ballRadius > paddle.GetPostion().y - paddle.GetParameter().pHeight / 2
		&& ball.GetPostion().y - ball.GetParameter().ballRadius < paddle.GetPostion().y + paddle.GetParameter().pHeight / 2 && fireb)
	{
		m_paddleSound.play();
		Ball::DynamicParam dp = ball.GetDynamicParameter();
		if (ball.GetPostion().x <= paddle.GetPostion().x)
		{
			dp.ballAngle = -dp.ballAngle+75;
		}
		else if (ball.GetPostion().x > paddle.GetPostion().x)
		{
			dp.ballAngle = -dp.ballAngle -75;
		}
		ball.SetDynamicParameter(dp);

		Ball::Position newPos;
		newPos.y = ball.GetPostion().y - ball.GetParameter().ballRadius - 0.1f;
		newPos.x = ball.GetPostion().x;
		ball.SetPosition(newPos);

	}
}