#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(const uint width, const uint height): width_(width), height_(height), m_sprite(m_texture) {
	// 1. Выделяем память в RAM (для процессора)
	m_pixels.resize(width_ * height_ * 4);

	// 2. Выделяем память в VRAM (для видеокарты)
	if (!m_texture.resize({width_, height_})) {
		throw std::runtime_error("FrameBuffer::FrameBuffer(): Failed to allocate texture");
	}

	m_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(width_), static_cast<int>(height_)}));
	m_texture.setSmooth(false);
	m_sprite.setTexture(m_texture);
	m_sprite.setScale({3.f, 3.f});
}

void FrameBuffer::clear(const sf::Color& color) {
	for (int i = 0; i < m_pixels.size(); i+=4) {
		m_pixels[i+0] = color.r;
		m_pixels[i+1] = color.g;
		m_pixels[i+2] = color.b;
		m_pixels[i+3] = color.a;
	}
}

void FrameBuffer::setPixel(int x, int y, const sf::Color& color) {
	if (x < 0 || x >= width_ || y < 0 || y >= height_)
		return;
	const size_t index = (y * width_ + x) * 4;
	m_pixels[index+0] = color.r;
	m_pixels[index+1] = color.g;
	m_pixels[index+2] = color.b;
	m_pixels[index+3] = color.a;
}

void FrameBuffer::update() {
	m_texture.update(m_pixels.data());
}

void FrameBuffer::draw(sf::RenderWindow& window) const {
	window.draw(m_sprite);
}

void FrameBuffer::drawSprite(int x, int y, const std::shared_ptr<Sprite>& sprite, const sf::Color& color) {
	{
		for (int ly = 0; ly < sprite->height; ++ly) {
			for (int lx = 0; lx < sprite->width; ++lx) {

				if (const size_t pixelIndex = ly * sprite->width + lx; sprite->data[pixelIndex] > 0) {
					setPixel(x + lx, y + ly, color);
				}
			}
		}
	}
}