#pragma once
#include "FrameBuffer.hpp"
#include "Sprite.hpp"

class Player final {
	int x, y;
	public:
		Player();
		void draw(FrameBuffer& fb) const;
		void move(int dx);
		void shoot();
	sf::IntRect getBounds() const {
		return sf::IntRect(
		    {x, y},
		    {static_cast<int>(playerSprite->width), static_cast<int>(playerSprite->height)}
		);
	}
	int getX() const { return x; }
	int getY() const { return y; }
};