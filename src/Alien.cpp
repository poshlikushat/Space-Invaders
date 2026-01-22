#include "Alien.hpp"

void Alien::move_alien(int dx, int dy) {
	x += dx; y += dy;
}
bool Alien::isAtRightEdge() const {
	return x >= (224 - sprite_->width);
}

bool Alien::isAtLeftEdge() const {
	return x <= 0;
}

void Alien::draw(FrameBuffer& buffer) const {
	buffer.drawSprite(x, y, sprite_, color);
}

 Alien::Alien(int x, int y, std::shared_ptr<Sprite> sprite, const sf::Color color)
: x(x), y(y), sprite_(std::move(sprite)), color(color) {}
