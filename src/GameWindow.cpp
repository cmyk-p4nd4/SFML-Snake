#include "GameWindow.h"

#include <iostream>

GameWindow::GameWindow(uint32_t width, uint32_t height, uint32_t bpp)
	: playerMoveThread(&GameWindow::handleMovement, this), isFocused(true), xCount(1), yCount(1)
	, isGameOver(true), status() {
	this->videoMode.width = width;
	this->videoMode.height = height;
	this->videoMode.bitsPerPixel = bpp;
	String title = "SFML Snake";
	ContextSettings cs;
	cs.antialiasingLevel = 8u;

	if (!font.loadFromFile("VCR_OSD_MONO.ttf")) {
		status = EXIT_ERROR;
	}
	pauseMessage.setFont(font);
	pauseMessage.setCharacterSize(40);
    pauseMessage.setString("Welcome to SFML Snake!\nPress Enter to start the game");
	FloatRect rect = pauseMessage.getGlobalBounds();
	pauseMessage.setPosition((width-rect.width) / 2, (height-rect.height) / 2);
    pauseMessage.setFillColor(sf::Color::White);

	window.create(videoMode, title, Style::Titlebar | Style::Close, cs);

	gridCount = window.getSize() / 20u;
	seed = std::chrono::steady_clock::now().time_since_epoch().count();
	this->initPlayground();
}

GameWindow::~GameWindow() { }

bool GameWindow::isRunning() const {
	return window.isOpen();
}

bool GameWindow::gameOver() const {
	return this->isGameOver;
}

void GameWindow::initPlayground() {
	Vector2u windowSize = window.getSize();
	Vector2i uSize(20, 20);
	xCount = int(windowSize.x / uSize.x);
	yCount = int(windowSize.y / uSize.y);
	if (!level.load("brick_wall.png", window.getSize(), 20u)) {
		std::cout << "Level cannot be loaded! Please restart your game!" << std::endl;
	}
	this->initFood();
}

void GameWindow::initPlayer() {
	player.init(20u);
	Vector2i lowerRight = Vector2i(xCount, yCount) * 3 / 4;
	player.setPosition(lowerRight);
}

void GameWindow::initFood() {
	food.setSize(entitySize);
	food.setFillColor(Color(0xFF6600FFu));
	this->makeNewFood();
}

void GameWindow::handleMovement() {
	float elapsedT = 0.f;

	while (!isGameOver && this->isRunning()) {
		float deltaT = clock.restart().asSeconds();
		elapsedT += deltaT;
		if (elapsedT <= 0.05f) continue;
		if (isFocused) {
			Vector2i heading = player.getDirection();
			if (heading.y == 0) {
				if (Keyboard::isKeyPressed(Keyboard::Up)) {
					if (heading.x < 0) player.turnLeft();
					else player.turnRight();
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down)) {
					if (heading.x > 0) player.turnLeft();
					else player.turnRight();
				}
			}
			else {
				if (Keyboard::isKeyPressed(Keyboard::Left)) {
					if (heading.y < 0) player.turnRight();
					else player.turnLeft();
				}
				else if (Keyboard::isKeyPressed(Keyboard::Right)) {
					if (heading.y > 0) player.turnRight();
					else player.turnLeft();
				}
			}
			player.move(heading);
			warpBounderies();
		}
		elapsedT = 0.f;
		this->checkCollision();
	}
}

void GameWindow::update() {
	while (window.pollEvent(event)) {
		switch (event.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::LostFocus:
				isFocused = false;
				break;
			case Event::GainedFocus:
				isFocused = true;
				break;
			case Event::KeyPressed:
				if ((event.key.code == Keyboard::Enter) && isGameOver) {
					this->makeNewFood();
					player.reset();
					this->initPlayer();
					isGameOver = false;
					playerMoveThread.launch();
				} else if (event.key.code == Keyboard::Escape && isGameOver) {
					window.close();
				}
				break;
		}
	}
	this->printFPS();
}

void GameWindow::render() {
	window.clear(Color(0x333333FFu));
	if (isGameOver) 
		window.draw(pauseMessage);
	else {
		window.draw(level);
		window.draw(food);
		window.draw(player);
	}
	window.display();
}

void GameWindow::checkCollision() {
	if (level.collidesWith(player.getGlobalBounds()) || player.collidesWithSelf()) {
		player.head().setFillColor(Color::Red - Color(0x00000070U));
		isGameOver = true;
		pauseMessage.setString("Game Over!\nPress Space to restart the game");
		return;
	}
	if (food.getGlobalBounds().intersects(player.getGlobalBounds())) {
		this->makeNewFood();
		player.addBodyPart();
		std::cout << "Food Eaten !" << std::endl;
	}
}

void GameWindow::LCG(unsigned int &pSeed) {
	uint32_t temp = pSeed;
	pSeed = 0xBC8Fu * pSeed % 0x7FFFFFFFu;
}

void GameWindow::makeNewFood() {
	int retries = -1;
	do {
		retries++;
		LCG(seed);
		uint32_t gridX = seed % xCount;
		LCG(seed);
		uint32_t gridY = seed % yCount;
		food.setPosition(Vector2f(gridX * food.getSize().x, gridY * food.getSize().y));
	} while (level.collidesWith(food.getGlobalBounds()));
	if (retries > 0) std::cout << "Retried " << retries << " times" << std::endl;
}

void GameWindow::warpBounderies() {
	Vector2i playerPos = player.getGridPosition();
	if (playerPos.x < 0)
		player.move(xCount, 0);
	else if (playerPos.y < 0)
		player.move(0, yCount);
	else if (playerPos.x >= xCount)
		player.move(-xCount, 0);
	else if (playerPos.y >= yCount)
		player.move(0, -yCount);
}

void GameWindow::printFPS() {
	std::stringstream ss;
	float fps = 1.0f / frameTime.restart().asSeconds();
	ss << "SFML Snake - ";
	ss << std::fixed << std::setprecision(1);
	ss << std::setw(6);
	ss << fps << " FPS";
	String title = ss.str();
	window.setTitle(title);
}