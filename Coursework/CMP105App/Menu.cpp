// INCLUDES.
#include "Menu.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR/S & DESTRUCTOR.
Menu::Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) : Screen(hwnd, in, gs, aud)
{
	fadedIn = false;
	initMenuBackground();
	initHowToPlayButton();
	initNewGameButton();
	initQuitButton();
	initTransFadeRect();
}

Menu::~Menu()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Menu::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::N))
	{
		input->setKeyUp(sf::Keyboard::N);

		audio->stopAllMusic();

		setGameState(State::LEVEL);
	}

	if (input->isKeyDown(sf::Keyboard::H))
	{
		input->setKeyUp(sf::Keyboard::H);

		setGameState(State::HOW_TO_PLAY);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::update(float dt)
{
	std::cout << "Menu update being called!\n";

	sf::Vector2f mousePos = sf::Vector2f(input->getMouseX(), input->getMouseY());

	if (checkMouseCollisions(&howToPlayButton, mousePos))
	{
		std::cout << "Hover collision detected with the how to play button!\n";	
		howToPlayButton.setTexture(&howToPlayButtonHoverTexture);

		if (checkMouseCollisions(&howToPlayButton, mousePos) && input->isMouseLDown())
		{
			std::cout << "Clicked on the how to play button!\n";
			howToPlayButton.setTexture(&howToPlayButtonClickedTexture);
		}
	}
	else
	{
		howToPlayButton.setTexture(&howToPlayButtonTexture);
	}

	if (checkMouseCollisions(&newGameButton, mousePos))
	{
		std::cout << "Collision detected with the new game button!\n";

		newGameButton.setTexture(&newGameButtonHoverTexture);

		if (checkMouseCollisions(&newGameButton, mousePos) && input->isMouseLDown())
		{
			std::cout << "Clicked on the new game button!\n";
			newGameButton.setTexture(&newGameButtonClickedTexture);
		}
	}
	else
	{
		newGameButton.setTexture(&newGameButtonTexture);
	}

	if (checkMouseCollisions(&quitButton, mousePos))
	{
		std::cout << "Collision detected with the quit button!\n";

		quitButton.setTexture(&quitButtonHoverTexture);

		if (checkMouseCollisions(&quitButton, mousePos) && input->isMouseLDown())
		{
			std::cout << "Clicked on the quit button!\n";
			quitButton.setTexture(&quitButtonClickedTexture);
		}
	}
	else
	{
		quitButton.setTexture(&quitButtonTexture);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::render()
{
	beginDraw();
	/*window->draw(menuBg);
	window->draw(howToPlayButton);
	window->draw(newGameButton);
	window->draw(quitButton);*/

	if (!fadedIn)
	{
		float decr = 255;

		while (decr > 0)
		{
			decr -= 0.05;

			transFade.setFillColor(sf::Color(0, 0, 0, decr));
			window->draw(menuBg);
			window->draw(howToPlayButton);
			window->draw(newGameButton);
			window->draw(quitButton);
			window->draw(transFade);
			endDraw();

			if (decr < 1)
			{
				fadedIn = true;
			}
		}
	}

	// Draw all things one last time to ensure they are on top and NOT the trans layer.
	window->draw(menuBg);
	window->draw(howToPlayButton);
	window->draw(newGameButton);
	window->draw(quitButton);

	endDraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::beginDraw()
{
	window->clear(sf::Color(0, 0, 0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::initMenuBackground()
{
	if (!menuBgTexture.loadFromFile("gfx/screens/menu_final.png"))
	{
		std::cerr << "Sorry could not load menu background image!\n";
	}

	menuBg.setSize(sf::Vector2f(960, 540));
	menuBg.setTexture(&menuBgTexture);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Menu::initHowToPlayButton()
{
	if (!howToPlayButtonTexture.loadFromFile("gfx/buttons/normal/how_to_play_button.png"))
	{
		std::cerr << "Sorry could not load how to play button image!\n";
	}

	if (!howToPlayButtonHoverTexture.loadFromFile("gfx/buttons/hover/htp_hover.png"))
	{
		std::cerr << "Sorry could not load how to play button hover image!\n";
	}

	if (!howToPlayButtonClickedTexture.loadFromFile("gfx/buttons/clicked/htp_clicked.png"))
	{
		std::cerr << "Sorry could not load how to play button clicked image!\n";
	}

	howToPlayButton.setSize(sf::Vector2f(175.2, 31.8f));
	howToPlayButton.setPosition(sf::Vector2f(75, 170));
	howToPlayButton.setTexture(&howToPlayButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::initNewGameButton()
{
	if (!newGameButtonTexture.loadFromFile("gfx/buttons/normal/new_game_button.png"))
	{
		std::cerr << "Sorry could not load new game button image!\n";
	}

	if (!newGameButtonHoverTexture.loadFromFile("gfx/buttons/hover/ng_hover.png"))
	{
		std::cerr << "Sorry could not load new game button hover image!\n";
	}

	if (!newGameButtonClickedTexture.loadFromFile("gfx/buttons/clicked/ng_clicked.png"))
	{
		std::cerr << "Sorry could not load new game button clicked image!\n";
	}

	newGameButton.setSize(sf::Vector2f(144, 31.8f));
	newGameButton.setPosition(sf::Vector2f(95, 210));
	newGameButton.setTexture(&newGameButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::initQuitButton()
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
	quitButton.setPosition(sf::Vector2f(130, 250));
	quitButton.setTexture(&quitButtonTexture);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check for bounding box collision with a point/vector2 rather than two boxes.
bool Menu::checkMouseCollisions(sf::RectangleShape* s1, sf::Vector2f s2)
{
	if ((s1->getGlobalBounds().left + s1->getGlobalBounds().width) < s2.x)
	{
		return false;
	}
		
	if (s1->getGlobalBounds().left > s2.x)
	{
		return false;
	}
		
	if (s1->getGlobalBounds().top + s1->getGlobalBounds().height < s2.y)
	{
		return false;
	}
		
	if (s1->getGlobalBounds().top > s2.y)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::initTransFadeRect()
{
	transFade.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
}