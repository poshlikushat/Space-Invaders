#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "FrameBuffer.hpp"

class Game {
public:
	Game();
	void run() const;

private:
	void processEvents() const;
	static void update();
	void render() const;

	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<FrameBuffer> m_frameBuffer;
};