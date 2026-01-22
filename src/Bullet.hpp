#pragma once
#include "FrameBuffer.hpp"
#include "Sprite.hpp"

class Projectile final {
	int x, y;
	int dy;
	std::shared_ptr<Sprite> sprite;
	sf::Color color;
public:
	bool active;
	Projectile(int startX, int startY, int speedY, std::shared_ptr<Sprite> s, sf::Color c);

	int getX() const;
	int getY() const;
	sf::Color getColor() const { return color; }

	void update();

	void draw(FrameBuffer& buffer) const;

	[[nodiscard]] sf::IntRect getBounds() const;
};