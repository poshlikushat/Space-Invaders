#include "Player.hpp"

 Player::Player() : x((224 - 11) / 2), y(256-32) {}

void Player::draw(FrameBuffer &fb) const {
	 fb.drawSprite(x, y, playerSprite, sf::Color::Green);
 }

void Player::move(int dx) {
	 	x += dx;
	 	if (x < 0) x = 0;
	 	if (x > 224 - 11) x = 224 - 11;
 }

