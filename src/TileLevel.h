#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;
class TileLevel : public Drawable, public Transformable {
	public:
	TileLevel();
	TileLevel(RenderWindow &win);
	~TileLevel();

	//	Initialize the level
	bool load(const std::string &path, Vector2u levelSize, unsigned tileSize);
	//	whether the rectangle overlap with the tile
	bool collidesWith(const FloatRect &rect);

	private:
	virtual void draw(RenderTarget &target, RenderStates states) const;

	std::vector<RectangleShape> wall;
	std::vector<FloatRect> hitBoxs;
	VertexArray grids;
	Texture mat_texture;
};

