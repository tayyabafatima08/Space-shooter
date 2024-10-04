#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;
int main()
{
	// Create a window
	RenderWindow window(VideoMode(800, 600), "Space Shooter");
	// for start background sprite
	Texture startbackgroundTexture;
	if (!startbackgroundTexture.loadFromFile("Background1.JPG"))
	{
		// Handle loading error
		return EXIT_FAILURE;
	}
	Sprite startbackground(startbackgroundTexture);
	// Create a button
	//For button 1
	RectangleShape button1;
	button1.setSize(Vector2f(150, 50));
	button1.setPosition(200, 430);
	Color idleColor1 = Color::Transparent;
	Color hoverColor1 = Color::Blue;
	button1.setFillColor(idleColor1);
	// for text in button 1
	Font button1font;
	if (!button1font.loadFromFile("arial_bold.ttf"))
	{
		return EXIT_FAILURE;
	}
	Text button1text("Play", button1font, 24);
	button1text.setPosition(button1.getPosition().x + (button1.getSize().x - button1text.getGlobalBounds().width) / 2, button1.getPosition().y + (button1.getSize().y - button1text.getGlobalBounds().height) / 2);
	button1text.setFillColor(Color::White);
	// for button 2
	RectangleShape button2;
	button2.setSize(Vector2f(150, 50));
	button2.setPosition(200, 480);
	Color idleColor2 = Color::Transparent;
	Color hoverColor2 = Color::Red;
	button2.setFillColor(idleColor2);
	// for text in button
	Font button2font;
	if (!button2font.loadFromFile("arial_bold.ttf"))
	{
		return EXIT_FAILURE;
	}
	Text button2text("Quit", button2font, 24);
	button2text.setPosition(button2.getPosition().x + (button2.getSize().x - button2text.getGlobalBounds().width) / 2, button2.getPosition().y + (button2.getSize().y - button2text.getGlobalBounds().height) / 2);
	button2text.setFillColor(Color::White);
	// for score board
	int score = 0;
	Font font;
	if (!font.loadFromFile("arial_bold.ttf"))
	{
		return EXIT_FAILURE;
	}
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10, 10);
	// for background sprite
	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("space.JPG"))
	{
		// Handle loading error
		return EXIT_FAILURE;
	}
	Sprite background(backgroundTexture);
	// for player sprite
	Texture jet;
	if (!jet.loadFromFile("jet1.png"))
	{
		// Handle loading error
		return EXIT_FAILURE;
	}
	Sprite jetsprite;
	jetsprite.setTexture(jet);
	jetsprite.setPosition(375, 510);
	// for enemy sprite
	const int num = 25;
	Texture enemyTexture;
	if (!enemyTexture.loadFromFile("enemy1.png"))
	{
		// Handle loading error
		return EXIT_FAILURE;
	}
	Sprite enemysprite[num];
	for (int i = 0; i < num; ++i)
	{
		enemysprite[i].setTexture(enemyTexture);
	}
	// for enemies
	int speedX[num];
	bool inwindow[num] = { false };
	srand(static_cast<unsigned>(time(NULL)));
	// for enemies position
	for (int i = 0; i < num; ++i)
	{
		enemysprite[i].setPosition(rand() % window.getSize().x, -50 - i * 50);
		speedX[i] = (rand() % 200) + 5.0;
	}
	// for player fire
	Clock coolDown;
	double fireDelay = 0.1;
	const int fireCount = 2000;
	RectangleShape fire[fireCount];
	for (int i = 0; i < fireCount; ++i)
	{
		fire[i].setPosition(-100, -100);
	}
	Clock clock;
	// pspeed--> player speed
	double pSpeed = 4.5;
	//  espeed--> enemy speed
	double eSpeed = 1.5;
	// fspeed--> fire speed
	double fSpeed = 200.0;
	// for player life
	int life = 3;
	Font lifefont;
	if (!lifefont.loadFromFile("arial_bold.ttf"))
	{
		return EXIT_FAILURE;
	}
	Text lifeText;
	lifeText.setFont(lifefont);
	lifeText.setCharacterSize(24);
	lifeText.setFillColor(Color::White);
	lifeText.setPosition(10, 30);

	bool gameover = false;
	// for Game Over
	Font gameoverFont;
	if (!gameoverFont.loadFromFile("arial_bold.ttf"))
	{
		return EXIT_FAILURE;
	}
	Text gameoverText;
	gameoverText.setFont(gameoverFont);
	gameoverText.setCharacterSize(24);
	gameoverText.setFillColor(Color::White);
	gameoverText.setPosition(100, 100);
	// game start
	bool gamestart = false;
	bool gamequit = false;
	// for sounds
	// for fire sound
	SoundBuffer fireBuffer;
	if (!fireBuffer.loadFromFile("shortlaser.WAV"))
	{
		return EXIT_FAILURE;
	}
	Sound fireSound;
	fireSound.setBuffer(fireBuffer);
	// for starting background
	SoundBuffer backgroundBuffer;
	if (!backgroundBuffer.loadFromFile("backgroundTune.WAV"))
	{
		return EXIT_FAILURE;
	}
	Sound backgroundSound;
	backgroundSound.setBuffer(backgroundBuffer);
	backgroundSound.play();

	const int maxLevels = 7;
	int currentLevel = 1;
	int enemiesremaining = num;
	// main loop
	while (window.isOpen())
	{   // event processing 
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				// Close the window if the close event is recieved
				window.close();
			}
			if (event.type == Event::MouseButtonPressed)
			{
				if (button1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) || button2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					backgroundSound.stop();
				}
				if (button1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					gamestart = true;
					gamequit = false;
				}
				else if (button2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					gamequit = true;
					gamestart = false;
				}
			}
		}
		// Check if the mouse is over the button 1
		Vector2f mousePos1 = Vector2f(Mouse::getPosition(window));
		if (button1.getGlobalBounds().contains(mousePos1)) {
			button1.setFillColor(hoverColor1);
			// Check for button click
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				// Handle button click action
			}
		}
		else {
			button1.setFillColor(idleColor1);
		}
		// Check if the mouse is over the button 2
		Vector2f mousePos2 = Vector2f(Mouse::getPosition(window));
		if (button2.getGlobalBounds().contains(mousePos2)) {
			button2.setFillColor(hoverColor2);
			// Check for button click
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				// Handle button click action
			}
		}
		else {
			button2.setFillColor(idleColor2);
		}
		// Check if the mouse is over the button 3
		Vector2f mousePos3 = Vector2f(Mouse::getPosition(window));
		// clearing windown
		window.clear();
		if (gamequit)
		{
			window.close();
		}
		else if (gamestart)
		{
			if (score >= currentLevel * 250)
			{
				currentLevel++;
				if (currentLevel <= maxLevels)
				{
					switch (currentLevel)
					{
					case 2:
						eSpeed = 2.0;
						break;
					case 3:
						eSpeed = 2.5;
						break;
					case 4:
						eSpeed = 3.0;
						break;
					case 5:
						eSpeed = 3.5;
						break;
					case 6:
						eSpeed = 4.0;
						break;
					case 7:
						eSpeed = 4.5;
						break;
					default:
						break;
					}
					srand(static_cast<unsigned>(time(NULL)));
					for (int i = 0; i < num; ++i) {
						enemysprite[i].setPosition(rand() % window.getSize().x, -50 - i * 50);
						speedX[i] = (rand() % 200) + 5.0;
					}
					jetsprite.setPosition(375, 510);
					enemiesremaining = num;
				}
				else
				{
					// player have completed this game
					gamestart = false;
					gameover = true;
					window.close();
				}
			}
			// for enemies movement
			double time = clock.restart().asSeconds();
			for (int i = 0; i < num; ++i)
			{
				if (!inwindow[i])
				{
					double y = enemysprite[i].getPosition().y + eSpeed * time * 50;
					enemysprite[i].setPosition(enemysprite[i].getPosition().x, y);
					if (enemysprite[i].getPosition().y > window.getSize().y)
					{
						enemysprite[i].setPosition(rand() % window.getSize().x, -50 - i * 50);
					}
				}
				else
				{
					double y = enemysprite[i].getPosition().y + eSpeed * time * 50;
					enemysprite[i].setPosition(enemysprite[i].getPosition().x, y);
				}
			}
			// for killing of enemy
			bool hitenemy = false;
			for (int i = 0; i < fireCount; ++i)
			{
				bool enemyKilled = false;
				for (int j = 0; j < num; ++j)
				{
					if (fire[i].getGlobalBounds().intersects(enemysprite[j].getGlobalBounds()))
					{
						if (enemysprite[j].getPosition().y > 0)
						{
							enemysprite[j].setPosition(-100, -100);
							fire[i].setPosition(-100, -100);
							score += 5;
							enemiesremaining--;
						}
					}
				}
			}
			scoreText.setString("Score :" + to_string(score));
			// for player
			if (Keyboard::isKeyPressed(Keyboard::D) && jetsprite.getPosition().x + jetsprite.getGlobalBounds().width < window.getSize().x) // for left
			{
				jetsprite.move(pSpeed, 0);
			}
			if (Keyboard::isKeyPressed(Keyboard::A) && jetsprite.getPosition().x > 0)  // for right
			{
				jetsprite.move(-pSpeed, 0);
			}
			if (Keyboard::isKeyPressed(Keyboard::W) && jetsprite.getPosition().y > 0)  // for up
			{
				jetsprite.move(0, -pSpeed);
			}
			if (Keyboard::isKeyPressed(Keyboard::S) && jetsprite.getPosition().y + jetsprite.getGlobalBounds().height < window.getSize().y)  // for down
			{
				jetsprite.move(0, pSpeed);
			}
			// for killing of player
				// for collision
			for (int i = 0; i < num; ++i)
			{
				if (jetsprite.getGlobalBounds().intersects(enemysprite[i].getGlobalBounds()))
				{
					jetsprite.setPosition(375, 510);
					life--;
					lifeText.setString("life :" + to_string(life));
					break;
				}
			}
			// if they get down the window
			for (int i = 0; i < num; ++i)
			{
				if (enemysprite[i].getPosition().y > window.getSize().y)
				{
					jetsprite.setPosition(375, 510);
					life--;
					lifeText.setString("life :" + to_string(life));
					break;
				}
			}
			// for Game Over
			if (life <= 0)
			{
				gameover = true;
				window.clear();
				gameoverText.setString("Game Over! Press any key to exit.");
				gameoverText.setPosition(195, 300);
				window.draw(gameoverText);
				window.display();
				Event gameOverEvent;
				while (window.waitEvent(gameOverEvent))
				{
					if (gameOverEvent.type == Event::KeyPressed)
					{
						window.close();
					}
				}
			}
			// setting player fire.
			bool fired = false;
			if (Keyboard::isKeyPressed(Keyboard::T) && !fired && coolDown.getElapsedTime().asSeconds() > fireDelay)
			{
				for (int i = 0; i < fireCount && !fired; ++i)
				{
					if (fire[i].getPosition().y < 0)
					{
						fire[i].setSize(Vector2f(5, 5));
						fire[i].setFillColor(Color::Yellow);
						fire[i].setPosition(jetsprite.getPosition().x + (jetsprite.getGlobalBounds().width / 2), jetsprite.getPosition().y - 20);
						fired = true;
						coolDown.restart();
						fireSound.play();
					}
				}
			}
			// player fire movement
			for (int i = 0; i < fireCount; ++i)
			{
				if (fire[i].getPosition().y > 0)
				{
					fire[i].move(0, -fSpeed * time);
					if (fire[i].getPosition().y + fire[i].getPosition().y < 0)
					{
						fire[i].setPosition(-100, -100);
					}
				}
			}
			// for another firing logic
			bool anotherFired = false;
			if (Keyboard::isKeyPressed(Keyboard::G) && !fired && coolDown.getElapsedTime().asSeconds() > fireDelay)
			{
				for (int i = 0; i < fireCount && !anotherFired; ++i)
				{
					if (fire[i].getPosition().y < 0)
					{
						fire[i].setSize(Vector2f(5, 5));
						fire[i].setFillColor(Color::Green);

						fire[i].setPosition(Vector2f(jetsprite.getPosition().x - 10, jetsprite.getPosition().y));
						fire[++i].setSize(Vector2f(5, 5));
						fire[i].setFillColor(Color::Green);
						fire[i].setPosition(jetsprite.getPosition().x + jetsprite.getGlobalBounds().width + 5, jetsprite.getPosition().y);
						anotherFired = true;
						coolDown.restart();
						fireSound.play();
					}
				}
			}
			// player another fire movement
			for (int i = 0; i < fireCount; ++i)
			{
				if (fire[i].getPosition().y > 0)
				{
					fire[i].move(0, -fSpeed * time);
				}
				else
				{
					fire[i].setPosition(-100, -100);
					anotherFired = false;
				}
			}
			// other game logic/update
			// window is clear
			window.clear();
			// draw background 
			window.draw(background);
			// draw player sprite
			window.draw(jetsprite);
			// draw enemies sprite
			for (int i = 0; i < num; ++i)
			{
				window.draw(enemysprite[i]);
			}
			// draw fires
			for (int i = 0; i < fireCount; ++i)
			{
				window.draw(fire[i]);
			}
			// draw player score
			window.draw(scoreText);
			// draw player life
			window.draw(lifeText);
		}
		else
		{
			// for background
			window.draw(startbackground);
			// draw buttons
			window.draw(button1);
			window.draw(button2);
			// draw button texts
			window.draw(button1text);
			window.draw(button2text);
		}
		// displaying window
		window.display();
	}
}