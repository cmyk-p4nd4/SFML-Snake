#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

#include <chrono>
#include <random>

#include "Snake.h"
#include "TileLevel.h"

using namespace sf;

#define EXIT_ERROR -1

class GameWindow { 
	public:
	GameWindow(uint32_t width, uint32_t height, uint32_t bpp = 32U);
	~GameWindow();

	bool isRunning() const;
	void update();
	void render();
	bool gameOver() const;

	void initPlayground();
	void initPlayer();
	void initFood();

	private:

	void printFPS();

	/**
	 * @brief Called everytime the player eat the food \n 
	 * also perform collision check to avoid food entity clipping into the wall 
	 */
	void makeNewFood();

	void checkCollision();	// Collision Player <-> Wall, Player <-> Food
	void handleMovement();	// method binded to sf::Thread to move the snake
	void warpBounderies();	// Warp player once it reaches the border

	void LCG(unsigned int &pSeed);
	

	RenderWindow window;
	Event event;
	VideoMode videoMode;
	Clock clock;
	Clock frameTime;

	uint32_t seed;

	Snake player;
	TileLevel level;
	RectangleShape food;

	Font font;
	Text pauseMessage;
	
	int status;	

	Vector2f entitySize = Vector2f(20.f, 20.f);
	Vector2u gridCount;
	int xCount = 0, yCount = 0;

	bool isFocused;				///<	flag for window's focus
	bool isGameOver;			///<	flag for game over event
	Thread playerMoveThread; 	///<	thread for movement handling
};

