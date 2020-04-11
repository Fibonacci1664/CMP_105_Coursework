// INCLUDES.
#include "Pause.h"
#include "Level.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Pause::Pause(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());
	quitClicked = false;
	mainMenuClicked = false;
	continueClicked = false;
	pressedLetterP = false;
	unpaused = false;
	originalViewPos = Level::getView();
	xTranslation = 0;

	initAudio();
	initPauseText();
	initScroll();
	initMainMenuButton();
	initContinueButton();
	initQuitButton();
	initTransLayer();
}

Pause::~Pause()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Pause::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::P))
	{
		input->setKeyUp(sf::Keyboard::P);
		audio->playSoundbyName("scroll");
		unpaused = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::update(float dt)
{
	sf::View currentViewPos = Level::getView();
	xTranslation = currentViewPos.getCenter().x - originalViewPos.getCenter().x;

	std::cout << "view has moved in the x axis by: " << xTranslation << '\n';

	mousePos = sf::Vector2f(input->getMouseX() + xTranslation, input->getMouseY());
	checkMainMenuButtonCollisions();
	checkContinueCollisions();
	checkQuitButtonCollisions();
	updatePauseMenuPosition();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::render()
{
	beginDraw();
	window->draw(transLayer);
	window->draw(pausedTextBox);
	window->draw(scrollBox);
	window->draw(mainMenuButton);
	window->draw(continueButton);
	window->draw(quitButton);
	endDraw();

	if (unpaused)
	{
		setGameState(State::LEVEL);
		unpaused = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::updatePauseMenuPosition()
{
	transLayer.setPosition((Level::getView().getCenter().x - Level::getView().getSize().x / 2.0f), (Level::getView().getCenter().y - Level::getView().getSize().y / 2.0f));	
	pausedTextBox.setPosition(sf::Vector2f(Level::getView().getCenter().x, Level::getView().getCenter().y - 150));
	scrollBox.setPosition(sf::Vector2f(Level::getView().getCenter().x, Level::getView().getCenter().y + 50));
	mainMenuButton.setPosition(sf::Vector2f(Level::getView().getCenter().x, Level::getView().getCenter().y));
	continueButton.setPosition(sf::Vector2f(Level::getView().getCenter().x, Level::getView().getCenter().y + 50));
	quitButton.setPosition(sf::Vector2f(Level::getView().getCenter().x, Level::getView().getCenter().y + 100));
}

void Pause::beginDraw()
{
	//window->clear(sf::Color(127, 127, 127, 255));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::initAudio()
{
	//audio->addSound("sfx/pause/unroll_scroll_2.ogg", "scroll");
}

void Pause::initPauseText()
{
	if (!pausedTexture.loadFromFile("gfx/text/paused_2.png"))
	{
		std::cerr << "Sorry could not load pause text image!\n";
	}

	pausedTextBox.setSize(sf::Vector2f(284.4f, 74.4f));
	pausedTextBox.setOrigin(sf::Vector2f(pausedTextBox.getSize().x / 2.0f, pausedTextBox.getSize().y / 2.0f));
	pausedTextBox.setPosition(sf::Vector2f(Level::getView().getCenter().x, Level::getView().getCenter().y - 200)); // window->getSize().y / 6.0f));
	pausedTextBox.setTexture(&pausedTexture);
}

void Pause::initScroll()
{
	if (!scrollTexture.loadFromFile("gfx/text/pause_scroll.png"))
	{
		std::cerr << "Sorry could not load scroll image!\n";
	}

	scrollBox.setSize(sf::Vector2f(230.4f, 376.8f));
	scrollBox.setOrigin(sf::Vector2f(scrollBox.getSize().x / 2.0f, scrollBox.getSize().y / 2.0f));
	scrollBox.setPosition(sf::Vector2f(window->getSize().x / 2.0f, window->getSize().y - 195));
	scrollBox.setTexture(&scrollTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::initMainMenuButton()
{
	if (!mainMenuButtonTexture.loadFromFile("gfx/buttons/normal/main_menu_button.png"))
	{
		std::cerr << "Sorry could not load main menu button image!\n";
	}

	if (!mainMenuButtonHoverTexture.loadFromFile("gfx/buttons/hover/mm_hover.png"))
	{
		std::cerr << "Sorry could not load main menu button hover image!\n";
	}

	if (!mainMenuButtonClickedTexture.loadFromFile("gfx/buttons/clicked/mm_clicked.png"))
	{
		std::cerr << "Sorry could not load main menu button clicked image!\n";
	}

	mainMenuButton.setSize(sf::Vector2f(141.0f, 31.8f));
	mainMenuButton.setOrigin(sf::Vector2f(mainMenuButton.getSize().x / 2.0f, mainMenuButton.getSize().y / 2.0f));
	mainMenuButton.setPosition(sf::Vector2f(window->getSize().x / 2.0f, 275));
	mainMenuButton.setCollisionBox(0, 0, 141.0f, 31.8f);
	mainMenuButton.setTexture(&mainMenuButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::initContinueButton()
{
	if (!continueButtonTexture.loadFromFile("gfx/buttons/normal/continue_button.png"))
	{
		std::cerr << "Sorry could not load continue button image!\n";
	}

	if (!continueButtonHoverTexture.loadFromFile("gfx/buttons/hover/c_hover.png"))
	{
		std::cerr << "Sorry could not load continue button hover image!\n";
	}

	if (!continueButtonClickedTexture.loadFromFile("gfx/buttons/clicked/c_clicked.png"))
	{
		std::cerr << "Sorry could not load continue button clicked image!\n";
	}

	continueButton.setSize(sf::Vector2f(144, 31.8f));
	continueButton.setOrigin(sf::Vector2f(continueButton.getSize().x / 2.0f, continueButton.getSize().y / 2.0f));
	continueButton.setPosition(sf::Vector2f(window->getSize().x / 2.0f, 312));
	continueButton.setCollisionBox(0, 0, 144, 31.8f);
	continueButton.setTexture(&continueButtonTexture);
}

void Pause::initQuitButton()
{
	if (!quitButtonTexture.loadFromFile("gfx/buttons/normal/quit_button.png"))
	{
		std::cerr << "Sorry could not load quit button image!\n";
	}

	if (!quitButtonHoverTexture.loadFromFile("gfx/buttons/hover/q_hover.png"))
	{
		std::cerr << "Sorry could not load quit button hover image!\n";
	}

	if (!quitButtonClickedTexture.loadFromFile("gfx/buttons/clicked/q_clicked.png"))
	{
		std::cerr << "Sorry could not load quit button clicked image!\n";
	}

	quitButton.setSize(sf::Vector2f(78.6f, 31.8f));
	quitButton.setOrigin(sf::Vector2f(quitButton.getSize().x / 2.0f, quitButton.getSize().y / 2.0f));
	quitButton.setPosition(sf::Vector2f(window->getSize().x / 2.0f, 350));
	quitButton.setCollisionBox(0, 0, 78.6f, 31.8f);
	quitButton.setTexture(&quitButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::initTransLayer()
{
	transLayer.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	transLayer.setFillColor(sf::Color(150, 150, 150, 2.5f));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::checkMainMenuButtonCollisions()
{
	if (checkMouseCollisions(&mainMenuButton, mousePos))
	{
		std::cout << "Collision detected with the main menu button!\n";

		mainMenuButton.setTexture(&mainMenuButtonHoverTexture);

		if (checkMouseCollisions(&mainMenuButton, mousePos) && input->isMouseLDown())
		{
			std::cout << "Clicked on the main menu button!\n";
			mainMenuButton.setTexture(&mainMenuButtonClickedTexture);
			mainMenuClicked = true;
		}
	}
	else
	{
		mainMenuButton.setTexture(&mainMenuButtonTexture);
	}

	if (mainMenuClicked && !input->isMouseLDown())
	{
		audio->playSoundbyName("scroll");
		mainMenuClicked = false;
		setGameState(State::MENU);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::checkContinueCollisions()
{
	if (checkMouseCollisions(&continueButton, mousePos))
	{
		std::cout << "Collision detected with the continue button!\n";

		continueButton.setTexture(&continueButtonHoverTexture);

		if (checkMouseCollisions(&continueButton, mousePos) && input->isMouseLDown())
		{
			std::cout << "Clicked on the continue button!\n";
			continueButton.setTexture(&continueButtonClickedTexture);
			continueClicked = true;
		}
	}
	else
	{
		continueButton.setTexture(&continueButtonTexture);
	}

	// If we've clicked the continue button AND released the mouse left button.
	if (continueClicked && !input->isMouseLDown())
	{
		audio->playSoundbyName("scroll");
		continueClicked = false;
		unpaused = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pause::checkQuitButtonCollisions()
{
	if (checkMouseCollisions(&quitButton, mousePos))
	{
		std::cout << "Collision detected with the quit button!\n";

		quitButton.setTexture(&quitButtonHoverTexture);

		if (checkMouseCollisions(&quitButton, mousePos) && input->isMouseLDown())
		{
			std::cout << "Clicked on the quit button!\n";
			quitButton.setTexture(&quitButtonClickedTexture);
			quitClicked = true;
		}
	}
	else
	{
		quitButton.setTexture(&quitButtonTexture);
	}

	if (quitClicked && !input->isMouseLDown())
	{
		audio->playSoundbyName("scroll");
		quitClicked = false;
		setGameState(State::CREDITS);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////