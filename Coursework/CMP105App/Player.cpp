/*
 * This class controls all aspects of the player including:
 *		- Animations
 *		- Item Collection
 *		- Health
 *
 * Original @author D. Green.
 *
 * © D. Green. 2020.
 */

////////////////// THIS COMMENT IS FOR A PULL TEST AFTER MY FIRST COMMIT AND PUSH //////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#include "Player.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTORS & DESTRUCTOR.
Player::Player()
{
	movingLeft = false;
	movingRight = false;
	isJumping = false;
	onGround = false;
	isAttacking = false;
	setVelocity(sf::Vector2f(100, -350));
	addFrames();
	setTextureRect(idle.getCurrentFrame());
	idle.setPlaying(true);
	idle.setLooping(true);

	gravityScalar = 100;
	gravitationalAccel = sf::Vector2f(0, 9.8f) * gravityScalar;
}

Player::~Player()
{
	std::cout << "Player object destroyed!\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.

void Player::update(float dt)
{
	gravityFall(dt);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::handleInput(float dt)
{
	setAllAnimsFalse();

	// If were WALKING RIGHT.
	if (input->isKeyDown(sf::Keyboard::D))
	{
		checkMovingRight(dt);
	}
	else
	{
		/*
		 * We only need one of the ifs to have an else which contains the idle code
		 * as this will be the only code that executes in this entire function if no
		 * keys are pressed.
		 */
		idle.setPlaying(true);
		idle.animate(dt);

		if (movingLeft)
		{
			idle.setFlipped(true);
		}
		else
		{
			idle.setFlipped(false);
		}

		setTextureRect(idle.getCurrentFrame());
	}

	// If were WALKING LEFT.
	if (input->isKeyDown(sf::Keyboard::A))
	{
		checkMovingLeft(dt);
	}

	// If were RUNNING.
	if (input->isMouseRDown())
	{
		checkRunning(dt);
	}

	// If were JUMPING
	if (input->isKeyDown(sf::Keyboard::Space))
	{
		checkJumping(dt);
	}

	// If were ATTACKING.
	if (input->isMouseLDown())
	{
		checkAttacking(dt);		
	}

	// If we STOP ATTACKING.
	if (!input->isMouseLDown())
	{
		isAttacking = false;
	}

	if (input->isKeyDown(sf::Keyboard::K))
	{
		if (movingLeft)
		{
			death.setFlipped(true);
		}
		else
		{
			death.setFlipped(false);
		}

		death.setPlaying(true);
		death.animate(dt);
		setTextureRect(death.getCurrentFrame());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Apply gravity to the player.
void Player::gravityFall(float dt)
{
	// s = ut + 1/2 at^2.
	//				  s		  =			u		  t   +  1/2			a				 t^2
	sf::Vector2f displacement = (stepVelocity * dt) + (0.5f * gravitationalAccel * dt * dt);

	// v = u + at.
	stepVelocity += gravitationalAccel * dt;

	setPosition(getPosition() + displacement);

	if ((getPosition().y + getSize().y) > window->getSize().y)
	{
		setPosition(getPosition().x, window->getSize().y - getSize().y);
		stepVelocity = sf::Vector2f(0, 0);
		onGround = true;
		isJumping = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Add all frames from the single sprite sheet.
void Player::addFrames()
{
	// Add attack frames.
	for (int i = 0; i < (attackFrames * FRAME_WIDTH); i += FRAME_WIDTH)
	{
		attack.addFrame(sf::IntRect(i, -25, FRAME_WIDTH, FRAME_HEIGHT));
	}

	// Add death frames.
	for (int i = 0; i < (deathFrames * FRAME_WIDTH); i += FRAME_WIDTH)
	{
		death.addFrame(sf::IntRect(i, FRAME_HEIGHT + 15, FRAME_WIDTH, FRAME_HEIGHT));
	}

	// All idle frames.
	for (int i = 0; i < (idleFrames * FRAME_WIDTH); i += FRAME_WIDTH)
	{
		idle.addFrame(sf::IntRect(i, (FRAME_HEIGHT * 2.063f), FRAME_WIDTH, FRAME_HEIGHT));
	}

	// Add jump frames.
	for (int i = 0; i < (jumpFrames * FRAME_WIDTH); i += FRAME_WIDTH)
	{
		jump.addFrame(sf::IntRect(i, (FRAME_HEIGHT * 3.1), FRAME_WIDTH, FRAME_HEIGHT));
	}

	// Add run frames.
	for (int i = 0; i < (runFrames * FRAME_WIDTH); i += FRAME_WIDTH)
	{
		run.addFrame(sf::IntRect(i, (FRAME_HEIGHT * 4.15), FRAME_WIDTH, FRAME_HEIGHT));
	}

	// Add walk frames.
	for (int i = 0; i < (walkFrames * FRAME_WIDTH); i += FRAME_WIDTH)
	{
		walk.addFrame(sf::IntRect(i, (FRAME_HEIGHT * 5.2), FRAME_WIDTH, FRAME_HEIGHT));
	}	

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	attack.setFrameSpeed(1.0f / 20.0f);
	death.setFrameSpeed(1.0f / 10.0f);
	idle.setFrameSpeed(1.0f / 15.0f);
	jump.setFrameSpeed(1.0f / 15.0f);
	run.setFrameSpeed(1.0f / 20.0f);
	walk.setFrameSpeed(1.0f / 15.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ensure are anims are reset back to false for each handleInput call.
void Player::setAllAnimsFalse()
{
	attack.setPlaying(false);
	jump.setPlaying(false);
	run.setPlaying(false);
	walk.setPlaying(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check if the player is moving right.
void Player::checkMovingRight(float dt)
{
	if (!isAttacking)
	{
		// If our last direction was moving left then flip the x velocity so its positive again.
		if (movingLeft)
		{
			velocity.x = -velocity.x;
		}

		// Move right walking.
		if (input->isKeyDown(sf::Keyboard::D))
		{
			movingRight = true;
			movingLeft = false;
			walk.setPlaying(true);
			walk.setFlipped(false);
			walk.animate(dt);
			setTextureRect(walk.getCurrentFrame());

			setPosition(sf::Vector2f(getPosition().x + (getVelocity().x * dt), getPosition().y));
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check if the player is moving left.
void Player::checkMovingLeft(float dt)
{
	if (!isAttacking)
	{
		// If our last direction was moving right then flip the x velocity so its negative again.
		if (movingRight)
		{
			velocity.x = -velocity.x;
		}

		// Move left walking.
		if (input->isKeyDown(sf::Keyboard::A))
		{
			movingRight = false;
			movingLeft = true;
			walk.setPlaying(true);
			walk.setFlipped(true);
			walk.animate(dt);
			setTextureRect(walk.getCurrentFrame());

			setPosition(sf::Vector2f(getPosition().x + (getVelocity().x * dt), getPosition().y));
		}
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkRunning(float dt)
{
	if (!isAttacking)
	{
		if (input->isKeyDown(sf::Keyboard::D))
		{
			movingRight = true;
			movingLeft = false;
			run.setFlipped(false);
			run.setPlaying(true);
			run.animate(dt);
			setTextureRect(run.getCurrentFrame());

			setPosition(sf::Vector2f(getPosition().x + (getVelocity().x * 1.5 * dt), getPosition().y));
		}
		else if (input->isKeyDown(sf::Keyboard::A))
		{
			movingRight = false;
			movingLeft = true;
			run.setPlaying(true);
			run.setFlipped(true);
			run.animate(dt);
			setTextureRect(run.getCurrentFrame());

			setPosition(sf::Vector2f(getPosition().x + (getVelocity().x * 1.5 * dt), getPosition().y));
		}
	}
}

void Player::checkJumping(float dt)
{
	if (movingLeft)
	{
		jump.setFlipped(true);
	}
	else
	{
		jump.setFlipped(false);
	}

	isJumping = true;
	onGround = false;
	jump.setPlaying(true);
	jump.animate(dt);
	setTextureRect(jump.getCurrentFrame());

	setPosition(getPosition().x + (getVelocity().x * dt), getPosition().y + (getVelocity().y * dt));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkAttacking(float dt)
{
	if (movingLeft)
	{
		attack.setFlipped(true);
	}
	else
	{
		attack.setFlipped(false);
	}

	isAttacking = true;
	attack.setPlaying(true);
	attack.animate(dt);
	setTextureRect(attack.getCurrentFrame());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
