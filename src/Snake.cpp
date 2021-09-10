#include "Snake.h"
#include <iostream>
Snake::Snake() { }

Snake::~Snake() { }

void Snake::addBodyPart() {
	RectangleShape newtail(snake.back());
	newtail.setPosition(lastPos);
	snake.push_back(newtail);
}

void Snake::init(unsigned int size) {
	Vector2f sz(size, size);
	RectangleShape body(sz);
	body.setFillColor(Color::White);
	for (int i = 0; i < 3; i++) {
		Vector2f pos = body.getSize();
		body.setPosition(pos.x * i, pos.y);
		snake.push_back(body);
	}
	this->size = sz;
	direction = Vector2i(-1, 0);
}

void Snake::reset() {
	snake.clear();
}

void Snake::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= this->getTransform();
	for (RectangleShape part : snake) target.draw(part, states);
}

void Snake::move(int offsetX, int offsetY) {
	this->move(Vector2i(offsetX, offsetY));
}

void Snake::move(const Vector2i &offset) {
	RectangleShape newhead(snake.front());
	snake.front().setFillColor(Color::White);

	Vector2f offsetMul = Vector2f(offset.x * size.x, offset.y * size.y);
	newhead.setPosition(snake.front().getPosition() + offsetMul);
	snake.push_front(newhead);

	lastPos = snake.back().getPosition();
	snake.pop_back();
}

void Snake::setPosition(Vector2i pos) {
	for (int i = 0; i < snake.size(); i++) {
		snake.at(i).setPosition(size.x * (i + pos.x), size.y * pos.y);
	}
}

RectangleShape &Snake::head() {
	return snake.front();
}

Vector2f Snake::getSize() {
	return this->size;
}

FloatRect Snake::getGlobalBounds() const {
	return snake.front().getGlobalBounds();
}

bool Snake::collidesWithSelf() {
	FloatRect head = snake.front().getGlobalBounds();
	for (auto bodyPart = snake.begin() + 1; bodyPart != snake.end(); bodyPart++)
		if (bodyPart->getGlobalBounds().intersects(head))
			return true;
	return false;
}

Vector2i Snake::getGridPosition() {
	Vector2f hPos = head().getPosition();
	return Vector2i(hPos.x / size.x, hPos.y / size.y);
}

Vector2i Snake::getDirection() const {
	return direction;
}

void Snake::turnLeft() {
	direction = Vector2i(-direction.y, direction.x);
}

void Snake::turnRight() {
	direction = Vector2i(direction.y, -direction.x);
}