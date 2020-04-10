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
	flashRedDelay = 0;
	attackDelay = 0;
	deathAnimDelay = 0;
	hitPointReductionDelay = 0;
	movingLeft = false;
	movingRight = false;
	idling = true;
	isJumping = false;
	isFalling = true;
	onGround = false;
	isAttacking = false;
	isDead = false;
	respawned = false;
	flashRed = false;
	lives = 3;
	hitPoints = 3;
	setVelocity(sf::Vector2f(100, 0));				// This vel is used for movement.
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
void Player::handleInput(float dt)
{
	hitPointReductionDelay += dt;
	deathAnimDelay += dt;
	attackDelay += dt;
	setAllAnimsFalse();

	// If were WALKING RIGHT.
	if (input->isKeyDown(sf::Keyboard::D) && !input->isKeyDown(sf::Keyboard::A))
	{
		movingRight = true;
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
		idling = true;
	}

	// If were WALKING LEFT.
	if (input->isKeyDown(sf::Keyboard::A) && !input->isKeyDown(sf::Keyboard::D))
	{
		movingLeft = true;
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

	if (isDead)
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

		if (deathAnimDelay > 3)
		{
			deathAnimDelay = 0;
			--lives;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::update(float dt)
{

	// If were falling OR jumping then apply gravity.
	if (isFalling || isJumping)
	{
		gravityFall(dt);
	}	

	if (flashRed)
	{
		setFillColor(sf::Color::Red);
		flashRedDelay += dt;
	}

	if (flashRed && flashRedDelay > 0.1f)
	{
		flashRed = false;
		setFillColor(sf::Color::White);
		flashRedDelay = 0;
	}

	// If we've respawned we need to make sure we respawn facing the correct direction, then reset all values to there starting values.
	if (respawned)
	{
		resetPlayerValues();
	}

	//checkGround();
	updateCollisionBox();
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

void Player::resetPlayerValues()
{
	if (movingRight)
	{
		movingLeft = false;
		isJumping = false;
		isFalling = true;
		onGround = false;
		isAttacking = false;
		isDead = false;
		respawned = false;
	}
	else
	{
		movingLeft = false;
		movingRight = false;
		isJumping = false;
		isFalling = true;
		onGround = false;
		isAttacking = false;
		isDead = false;
		respawned = false;
	}
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
		idling = false;
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
		idling = false;
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
			idling = false;
			run.setFlipped(false);
			run.setPlaying(true);
			run.animate(dt);
			setTextureRect(run.getCurrentFrame());
		}

		if (input->isKeyDown(sf::Keyboard::A) && !input->isKeyDown(sf::Keyboard::D))
		{
			movingRight = false;
			movingLeft = true;
			idling = false;
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
			audioMan.playSoundbyName("attack");
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


	// Couldnt do this, what is below, as for every collision repsonse we'd recall every function, NO USE!
	// Check object collisions
	// Check enemy collisions
	// Etc etc.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::checkExitDoorCollisions(GameObject* col)
{
	if (Collision::checkBoundingBox(col, this))
	{
		//std::cout << "this works!\n";

		return true;		
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkTileCollisions(GameObject* col)
{
	//std::cout << "Player collided with tile!\n";

	sf::Vector2f tileCentre = sf::Vector2f((col->getPosition().x + (col->getSize().x / 2.0f)), (col->getPosition().y + (col->getSize().y / 2.0f)));
	float xColBoxCentre = getCollisionBox().left + getCollisionBox().width / 2;
	float yColBoxCentre = getCollisionBox().top + getCollisionBox().height / 2;
	float xDiff = tileCentre.x - xColBoxCentre;
	float yDiff = tileCentre.y - yColBoxCentre;			// Top will give me the y value.

	// This is the difference between the left side of the collding obejct if and the right side of player collider.
	float rightXDiff = (getPosition().x + getSize().x) - (getCollisionBox().left + getCollisionBox().width);

	// This is the difference between the right side of the collding obejct if and the left side of player collider.
	float leftXDiff = getCollisionBox().left - getPosition().x;
	float topYDiff = getCollisionBox().top - getPosition().y;

	// X-axis collision.
	if (std::abs(xDiff) > std::abs(yDiff))
	{
		//std::cout << "X-axis collision!\n";

		// Right hand side of tile collission.
		if (xDiff < 0)
		{
			// THIS IS PERFECT NEVER CHANGE IT!
			//std::cout << "Right\n";
			stepVelocity.x = 0;
			setPosition(sf::Vector2f((col->getCollisionBox().left + col->getCollisionBox().width + 1) - leftXDiff, getPosition().y));		// new version
		}
		else			// Left hand side of tile collision.
		{
			// THIS IS PERFECT DO NOT CHANGE!
			//std::cout << "Left\n";
			stepVelocity.x = 0;
			setPosition(sf::Vector2f((col->getCollisionBox().left - getSize().x) + (rightXDiff - 1), getPosition().y));		// new version.
		}
	}
	else				// Y-axis collision.
	{
		//std::cout << "Y-axis collision!\n";

		// Bottom of tile collision.
		if (yDiff < 0)
		{
			// THIS IS GOOD DO NOT CHANGE!
			//std::cout << "Bottom\n";
			stepVelocity.y = 0;
			setPosition(sf::Vector2f(getPosition().x, (col->getPosition().y + col->getSize().y)));
		}
		else			// Top of tile collision.
		{
			// Check if we've collided with a spikes tile. Player can only be injured every second.
			if (col->getIndex() == 25 && hitPointReductionDelay > 1.0f)
			{
				spikeCollision();
			}
			else	// /If it's not a spike tile then it must be a normal ground tile.
			{
				// THIS IS GOOD DO NOT CHANGE!
				//std::cout << "Top\n";
				stepVelocity.y = 0;
				setPosition(sf::Vector2f(getPosition().x, col->getCollisionBox().top - getSize().y));
				onGround = true;
				isJumping = false;
				isFalling = false;
			}		
		}
	}
}

void Player::spikeCollision()
{
	std::cout << "Collided with spikes!\n";

	--hitPoints;
	audioMan.playSoundbyName("umph");
	hitPointReductionDelay = 0;

	if (movingRight)
	{
		/*
		 * Check if the last spike collision caused our hit points to reach zero, this prevents the stepvelocity
		 * being applied on the collision that causes death and prevent the player respawning with the applied step velocity.
		 */
		if (hitPoints == 0)
		{
			isDead = true;
		}
		else
		{
			flashRed = true;
			stepVelocity = sf::Vector2f(-100, -300);
		}
	}
	else if (movingLeft)
	{
		if (hitPoints == 0)
		{
			isDead = true;
		}
		else
		{
			flashRed = true;
			stepVelocity = sf::Vector2f(100, -300);
		}
	}
}

void Player::checkLiftCollisions(GameObject* col)
{
	if (Collision::checkBoundingBox(this, col))
	{
		checkTileCollisions(col);
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
	audioMan.addSound("sfx/player/jump.ogg", "jump");
	audioMan.addSound("sfx/player/sword_attack.ogg", "attack");
	audioMan.addSound("sfx/player/umph.ogg", "umph");
}

void Player::respawn()
{
	respawned = true;
	isDead = false;
	hitPoints = 3;
	setPosition(sf::Vector2f(100, 100));
	stepVelocity = sf::Vector2f(0, 0);
	setFillColor(sf::Color::White);
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

bool Player::getIdling()
{
	return idling;
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

bool Player::getIsOnGround()
{
	return onGround;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setSFXMuteAudio(bool l_muted)
{
	sfxMuted = l_muted;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//// The state here has been passed by pointer all the way from Screen, through Level and finally to Player.
//void Player::passAndSetCurrentSateFromScreen(GameState* currGameState)
//{
//	// Sets state so player now knows about what state the game is in.
//	currentState = currGameState->getCurrentState();
//	currentGameState = currGameState;
//}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//State Player::getCurrentGameState()
//{
//	return currentState;
//}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Player::getIsDead()
{
	return isDead;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setIsDead(bool l_isDead)
{
	isDead = l_isDead;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setRespawned(bool l_respawned)
{
	respawned = l_respawned;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Player::getLives()
{
	return lives;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setLives(int l_lives)
{
	lives = l_lives;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setHitPoints(int l_hitPoints)
{
	hitPoints = l_hitPoints;
}