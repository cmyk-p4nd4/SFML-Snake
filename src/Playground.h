#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Playground : public Drawable {
	public:
	Playground();
	~Playground();

	private:

	VertexArray vertexArray_mat;
	VertexBuffer vertexBuffer_mat;
};

