#pragma once
#include "FrameBuffer.hpp"
#include "Sprite.hpp"

class Alien final {
	int x, y;
	std::shared_ptr<Sprite> sprite_;
	sf::Color color;

	public:
	Alien(int x, int y, std::shared_ptr<Sprite> sprite, sf::Color color);

	void draw(FrameBuffer& buffer) const;

	void move_alien(int dx, int dy);

	[[nodiscard]] bool isAtLeftEdge() const;
	[[nodiscard]] bool isAtRightEdge() const;

	sf::IntRect getBounds() const {
		return sf::IntRect(
		    {x, y},
		    {static_cast<int>(sprite_->width), static_cast<int>(sprite_->height)}
		);
	}
	int getX() const { return x; }
	int getY() const { return y; }
};