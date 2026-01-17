// src/Game.cpp
#include "Game.hpp"
#include "Sprite.hpp"

Game::Game() {
	m_window = std::make_unique<sf::RenderWindow>(
			sf::VideoMode({224 * 3, 256 * 3}),
			"Space Invaders",
			sf::Style::Titlebar | sf::Style::Close
	);

	// Ограничим FPS, чтобы игра не летела со скоростью 5000 кадров в секунду
	m_window->setFramerateLimit(60);

	// 2. Создаем наш экранный буфер (размер как в оригинале)
	m_frameBuffer = std::make_unique<FrameBuffer>(224, 256);
}

void Game::run() const {
	// ГЛАВНЫЙ ЦИКЛ ИГРЫ
	while (m_window->isOpen()) {
		processEvents();
		update();
		render();
	}
}

void Game::processEvents() const {
	// Обработка очереди событий SFML 3 (pollEvent возвращает std::optional)
	while (const std::optional event = m_window->pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			m_window->close();
		}
		// Тут потом добавим управление:
		// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) ...
	}
}

void Game::update() {}

void Game::render() const {
	m_frameBuffer->clear(sf::Color::Black);

	m_frameBuffer->drawSprite(10, 10, alienSprite1, sf::Color::White);

	// Рисуем другого пришельца рядом
	m_frameBuffer->drawSprite(30, 10, alienSprite2, sf::Color::Green);

	// 3. Загружаем пиксели в видеокарту
	m_frameBuffer->update();

	// 4. Очищаем реальное окно и рисуем буфер
	m_window->clear();
	m_frameBuffer->draw(*m_window);
	m_window->display();
}