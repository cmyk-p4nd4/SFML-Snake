#include "TileLevel.h"
#include <iostream>
TileLevel::TileLevel() { }

TileLevel::~TileLevel() { }

bool TileLevel::load(const std::string &path, Vector2u levelSize, unsigned tileSize) {
	Vector2u textureSize = Vector2u(tileSize, tileSize);
	uint32_t xCount = levelSize.x / textureSize.x;
	uint32_t yCount = levelSize.y / textureSize.y;

	// std::cout << xCount << ' ' << yCount << std::endl;

	grids.setPrimitiveType(Lines);
	// M * N grids requires M + N splitters
	grids.resize((xCount + yCount+1) * 2u);

	// Populate the grids (vertical)
	for (size_t vIdx = 1; vIdx <= xCount; vIdx++) {
		Vertex * point = &grids[vIdx * 2u];
		point[0].position = Vector2f(vIdx * tileSize, 0);
		point[1].position = Vector2f(vIdx * tileSize, levelSize.y);
		point[0].color = Color(0xFFFFFF2A);
		point[1].color = Color(0xFFFFFF2A);
	}
	// Populate the grids (horizontal)
	for (size_t hIdx = 1; hIdx <= yCount; hIdx++) {
		Vertex * point = &grids[(hIdx + xCount) * 2u];
		point[0].position = Vector2f(0, hIdx * tileSize);
		point[1].position = Vector2f(levelSize.x, hIdx * tileSize - 1);
		point[0].color = Color(0xFFFFFF2A);
		point[1].color = Color(0xFFFFFF2A);
	}

	wall.reserve(size_t(xCount * yCount));
	hitBoxs.reserve(size_t(xCount * yCount));

	if (!mat_texture.loadFromFile(path)) return false;
	RectangleShape block(static_cast<Vector2f>(textureSize));
	block.setTexture(&mat_texture);
	block.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(20, 20)));

	// Sample level design with outer ring filled with walls
	for (size_t i = 0; i < xCount; i++) {
		for (size_t j = 0; j < yCount; j++) {
			// if (i*j > 0 && j < yCount - 1 && i < xCount - 1) continue;
			if (j > 0 && j < yCount - 1) continue;
			block.setPosition(i * textureSize.x, j * textureSize.y);
			wall.push_back(block);
			hitBoxs.push_back(block.getGlobalBounds());
		}
	}
	return true;
}

bool TileLevel::collidesWith(const FloatRect &rect) {
	for (FloatRect box : hitBoxs)
		if (box.intersects(rect)) return true;
	return false;
}

void TileLevel::draw(RenderTarget &target, RenderStates states) const {
	// apply the transform
	states.transform *= getTransform();

	// draw the grid lines
	target.draw(grids, states);

	states.texture = &mat_texture;
	// draw the blocks
	for (RectangleShape b : wall) {
		target.draw(b, states);
	}
}
