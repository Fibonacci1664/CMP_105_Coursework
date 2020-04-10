/*
 * This class is a rather generic base class from which most things with the game will inherit from.
 *
 * Original @author Dr Paul Robertson.
 * Updated by @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////

 // INCLUDES.
#include "GameObject.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTORS & DESTRUCTOR.
GameObject::GameObject()
{
	input = nullptr;
	indexPos = 0;
}

GameObject::~GameObject()
{
	//std::cout << "GameObject object destroyed!\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUCNTIONS.

// Override this function to provide input handling for the derived class
void GameObject::handleInput(float dt)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// override this function to provide functionality
void GameObject::update(float dt)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Reponse function, what the sprite does based on collision
// Colliding object is passed in for information
// e.g. compare sprite positions to determine new velocity direction.
// e.g. checking sprite type (world, enemy, bullet etc) so response is based on that.
void GameObject::collisionResponse(GameObject* collider)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GETTERS & SETTERS.

// Sets the velocity of the sprite
void GameObject::setVelocity(sf::Vector2f vel)
{
	velocity = vel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameObject::setVelocity(float vx, float vy)
{
	velocity.x = vx;
	velocity.y = vy;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// get sprite velocity
sf::Vector2f GameObject::getVelocity()
{
	return velocity;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Returns collision box + position, giving the collision box in the correct position
sf::FloatRect GameObject::getCollisionBox() {
	
	return sf::FloatRect(collisionBox.left + getPosition().x, collisionBox.top + getPosition().y, collisionBox.width, collisionBox.height); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GameObject::getIndex()
{
	//std::cout << "World tile index = " << indexPos << '\n';
	return indexPos;
}

void GameObject::setIndex(int l_indexPos)
{
	indexPos = l_indexPos;
}
