#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <deque>

using namespace sf;
using namespace std;

class Snake : public Transformable, public Drawable {
	public:
	Snake();
	~Snake();
	void init(unsigned int size);
	void reset();
	void addBodyPart();
	//	Move the snake by (offsetX, offsetX) grids
	virtual void move(int offsetX, int offsetY);
	//	Move the snake by <offset> number of grids
	virtual void move(const Vector2i &offset);
	void turnLeft();
	void turnRight();
	//	Move the snake to grid (X, Y). 
	//	This method will wrap the whole snake to this position
	virtual void setPosition(Vector2i pos);

	virtual FloatRect getGlobalBounds() const;
	//	Get the direction vector of the snake
	Vector2i getDirection() const;
	//	Whether the snake collides with itself
	bool collidesWithSelf();
	//	Returns player position in Grid(X, Y) format relative to the head
	Vector2i getGridPosition();
	Vector2f getSize();
	RectangleShape &head();

	private:
	virtual void draw(RenderTarget &target, RenderStates states) const;

	Vector2f size;	//	The body dimension of the snake
	std::deque<RectangleShape> snake;	//	the actual snake
	Vector2f lastPos;	//	Position of the tail
	Vector2i direction; //	Direction vector of the snake
};

#endif