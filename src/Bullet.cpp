#include "Bullet.hpp"

 Projectile::Projectile(int startX, int startY, int speedY, std::shared_ptr<Sprite> s, const sf::Color c)
: x(startX), y(startY), dy(speedY), sprite(std::move(s)), color(c), active(true) {}

void Projectile::update() {
 	y += dy;
 	if (y < 0 || y > 256) {
 		active = false;
 	}
 }

void Projectile::draw(FrameBuffer& buffer) const {
 	buffer.drawSprite(x, y, sprite, color);
 }

sf::IntRect Projectile::getBounds() const {
 	return {
							{x, y},
							{static_cast<int>(sprite->width), static_cast<int>(sprite->height)}
 	};
 }

int Projectile::getX() const { return x; }
int Projectile::getY() const { return y; }