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

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 // INCLUDES.
#include "Player.h"
#include "Level.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTORS & DESTRUCTOR.
Player::Player()
{
	initAudio();

	attackDelay = 0;
	deathAnimDelay = 0;
	movingLeft = false;
	movingRight = true;						// Even thought the player hasnt moved any direction when they first spawn this needs to be true for the move logic to work.
	isJumping = false;
	isFalling = true;
	onGround = false;
	isAttacking = false;
	isDead = false;
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.

void Player::update(float dt)
{
	// If were falling OR jumping then apply gravity.
	if (isFalling || isJumping)
	{
		gravityFall(dt);
	}

	//checkGround();
	updateCollisionBox();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::handleInput(float dt)
{
	attackDelay += dt;
	setAllAnimsFalse();

	// If were WALKING RIGHT.
	if (input->isKeyDown(sf::Keyboard::D) && !input->isKeyDown(sf::Keyboard::A))
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
	if (input->isKeyDown(sf::Keyboard::A) && !input->isKeyDown(sf::Keyboard::D))
	{
		checkMovingLeft(dt);
	}

	// If were JUMPING
	if (input->isKeyDown(sf::Keyboard::Space) && onGround)
	{
		checkJumping(dt);
	}

	if (!onGround && isJumping)
	{
		if (movingLeft)
		{
			jump.setFlipped(true);
		}
		else
		{
			jump.setFlipped(false);
		}

		jump.setPlaying(true);
		jump.animate(dt);
		setTextureRect(jump.getCurrentFrame());
	}

	// If were RUNNING.
	/*
	 * First 'if' checks if we're holding down the right mouse AND either of the left or right keys.
	 * The second 'if' checks that were only holding down either A or D, but not both.
	 */
	if (input->isMouseRDown() && (input->isKeyDown(sf::Keyboard::A) || input->isKeyDown(sf::Keyboard::D)))
	{
		if ((input->isKeyDown(sf::Keyboard::A) && !input->isKeyDown(sf::Keyboard::D)) || (!input->isKeyDown(sf::Keyboard::A) && input->isKeyDown(sf::Keyboard::D)))
		{
			checkRunning(dt);
		}
	}

	// If were ATTACKING.
	if (input->isMouseLDown())
	{
		checkAttacking(dt);
	}
	else
	{
		isAttacking = false;
	}

	if (input->isKeyDown(sf::Keyboard::K))
	{
		deathAnimDelay += dt;

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

		if (deathAnimDelay > 3)
		{
			//Level::setMusicStopped(true);
			isDead = true;
			//currentGameState->setCurrentState(State::YOU_DIED);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Apply gravity to the player.
void Player::gravityFall(float dt)
{
	// s = ut + 1/2 at^2.
	//				  s		  =			u		  t   +  1/2			a				 t^2
	sf::Vector2f displacement = (stepVelocity * dt) + (0.5f * gravitationalAccel * dt * dt);

	// v = u + at.
	stepVelocity += gravitationalAccel * dt;

	setPosition(getPosition() + displacement);

	isFalling = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ensure are anims are reset back to false for each handleInput call.
void Player::setAllAnimsFalse()
{
	attack.setPlaying(false);
	jump.setPlaying(false);
	run.setPlaying(false);
	walk.setPlaying(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

		movingRight = true;
		movingLeft = false;
		walk.setPlaying(true);
		walk.setFlipped(false);
		walk.animate(dt);
		setTextureRect(walk.getCurrentFrame());

		setPosition(sf::Vector2f(getPosition().x + (getVelocity().x * dt), getPosition().y));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

		movingRight = false;
		movingLeft = true;
		walk.setPlaying(true);
		walk.setFlipped(true);
		walk.animate(dt);
		setTextureRect(walk.getCurrentFrame());

		setPosition(sf::Vector2f(getPosition().x + (getVelocity().x * dt), getPosition().y));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkRunning(float dt)
{
	if (!isAttacking)
	{
		if (input->isKeyDown(sf::Keyboard::D) && !input->isKeyDown(sf::Keyboard::A))
		{
			movingRight = true;
			movingLeft = false;
			run.setFlipped(false);
			run.setPlaying(true);
			run.animate(dt);
			setTextureRect(run.getCurrentFrame());
		}

		if (input->isKeyDown(sf::Keyboard::A) && !input->isKeyDown(sf::Keyboard::D))
		{
			movingRight = false;
			movingLeft = true;
			run.setPlaying(true);
			run.setFlipped(true);
			run.animate(dt);
			setTextureRect(run.getCurrentFrame());
		}

		if (!onGround && isJumping)
		{
			if (movingLeft)
			{
				jump.setFlipped(true);
			}
			else
			{
				jump.setFlipped(false);
			}

			jump.setPlaying(true);
			jump.animate(dt);
			setTextureRect(jump.getCurrentFrame());
		}

		setPosition(sf::Vector2f(getPosition().x + (getVelocity().x * 1.3 * dt), getPosition().y));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkJumping(float dt)
{
	input->setKeyUp(sf::Keyboard::Space);

	if (movingLeft)
	{
		jump.setFlipped(true);
	}
	else
	{
		jump.setFlipped(false);
	}

	if (!sfxMuted)
	{
		audioMan.playSoundbyName("jump");
	}

	isJumping = true;
	onGround = false;
	stepVelocity = sf::Vector2f(0, -400);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	if (attackDelay > 1.0f)
	{
		if (!sfxMuted)
		{
			audioMan.playSoundbyName("up");
		}

		attackDelay = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Reponse function, what the sprite does based on collision
// Colliding object is passed in for information
// e.g. compare sprite positions to determine new velocity direction.
// e.g. checking sprite type (world, enemy, bullet etc) so response is based on that.
void Player::collisionResponse(GameObject* col)
{
	checkTileCollisions(col);

	// Check object collisions
	// Check enemy collisions
	// Etc etc.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkTileCollisions(GameObject* col)
{
	std::cout << "Player collided with tile!\n";

	sf::Vector2f tileCentre = sf::Vector2f((col->getPosition().x + (col->getSize().x / 2.0f)), (col->getPosition().y + (col->getSize().y / 2.0f)));
	float xColBoxCentre = getCollisionBox().left + getCollisionBox().width / 2;
	float yColBoxCentre = getCollisionBox().top + getCollisionBox().height / 2;

	float xDiff = tileCentre.x - xColBoxCentre;
	float yDiff = tileCentre.y - yColBoxCentre;			// Top will give me the y value.

	float leftXDiff = getCollisionBox().left - getPosition().x;
	//float rightXDiff = (getPosition().x + getSize().x) - (getCollisionBox().left + getCollisionBox().width);
	float topYDiff = getCollisionBox().top - getPosition().y;

	// X-axis collision.
	if (std::abs(xDiff) > std::abs(yDiff))
	{
		std::cout << "X-axis collision!\n";

		// Right hand side of tile collission.
		if (xDiff < 0)
		{
			// THIS IS GOOD!
			std::cout << "Right\n";
			stepVelocity.x = 0;
			//onGround = true;
			setPosition(sf::Vector2f((col->getPosition().x + col->getSize().x) - leftXDiff, getPosition().y));
		}
		else			// Left hand side of tile collision.
		{
			// THIS IS PERFECT DO NOT CHANGE!
			std::cout << "Left\n";
			stepVelocity.x = 0;
			//onGround = true;
			setPosition(sf::Vector2f(col->getPosition().x - (leftXDiff + getCollisionBox().width), getPosition().y));
		}
	}
	else				// Y-axis collision.
	{
		std::cout << "Y-axis collision!\n";

		// Bottom of tile collision.
		if (yDiff < 0)
		{
			// THIS IS GOOD DO NOT CHANGE!
			std::cout << "Bottom\n";
			stepVelocity.y = 0;
			setPosition(sf::Vector2f(getPosition().x, (col->getPosition().y + col->getSize().y)));
		}
		else			// Top of tile collision.
		{
			// THIS IS GOOD DO NOT CHANGE!
			std::cout << "Top\n";
			stepVelocity.y = 0;
			setPosition(sf::Vector2f(getPosition().x, col->getPosition().y - getSize().y));
			onGround = true;
			isJumping = false;
			isFalling = false;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::updateCollisionBox()
{
	if (movingLeft)
	{
		setCollisionBox(getSize().x / 3, 18, 30, 50);
	}
	else
	{
		setCollisionBox(getSize().x / 5, 18, 30, 50);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::initAudio()
{
	audioMan.addSound("sfx/smb_jump-super.wav", "jump");
	audioMan.addSound("sfx/smb_1-up.ogg", "up");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::getMovingRight()
{
	return movingRight;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::getMovingLeft()
{
	return movingLeft;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setIsFalling(bool l_isFalling)
{
	isFalling = l_isFalling;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setIsOnGround(bool l_isOnGround)
{
	onGround = l_isOnGround;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setSFXMuteAudio(bool l_muted)
{
	sfxMuted = l_muted;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The state here has been passed by pointer all the way from Screen, through Level and finally to Player.
void Player::passAndSetCurrentSateFromScreen(GameState* currGameState)
{
	// Sets state so player now knows about what state the game is in.
	currentState = currGameState->getCurrentState();
	currentGameState = currGameState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

State Player::getCurrentGameState()
{
	return currentState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::getIsDead()
{
	return isDead;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setIsDead(bool l_isDead)
{
	isDead = l_isDead;
}
