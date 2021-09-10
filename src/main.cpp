#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "GameWindow.h"

int main() {
	GameWindow gwindow(900, 600);

	while (gwindow.isRunning()) {
		gwindow.update();
		gwindow.render();
	}

	return 0;
}