#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include "Sprite.hpp"

class FrameBuffer {
  uint width_, height_;
	std::vector<std::uint8_t> m_pixels;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
  public:
    explicit FrameBuffer(uint width, uint height);
		void clear(const sf::Color& color);

		void setPixel(int x, int y, const sf::Color& color);

		void update();
		void draw (sf::RenderWindow& window) const;

		void drawSprite(int x, int y, const std::shared_ptr<Sprite>& sprite, const sf::Color& color);
};

#endif //FRAMEBUFFER_HPP
