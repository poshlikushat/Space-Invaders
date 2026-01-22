#include "Game.hpp"
#include "Sprite.hpp"
#include <algorithm>

Game::Game() {
	m_window = std::make_unique<sf::RenderWindow>(
			sf::VideoMode({224 * 3, 256 * 3}),
			"Space Invaders",
			sf::Style::Titlebar | sf::Style::Close
	);
	m_window->setFramerateLimit(60);

	m_frameBuffer = std::make_unique<FrameBuffer>(224, 256);
	m_player = std::make_unique<Player>();

	std::random_device rd;
	m_rng = std::mt19937(rd());

	restartGame();
}

void Game::spawnAliens() {
	m_aliens.clear();

	for (int row = 0; row < 5; ++row) {
		for (int col = 0; col < 11; ++col) {
			constexpr int gapY = 16;
			constexpr int gapX = 16;
			constexpr int startX = 24;
			constexpr int startY = 32;
			int x = startX + (col * gapX);
			int y = startY + (row * gapY);

			if (row == 0) m_aliens.emplace_back(x, y, alienSprite1, sf::Color::White);
			else if (row < 3) m_aliens.emplace_back(x, y, alienSprite2, sf::Color::Green);
			else m_aliens.emplace_back(x, y, alienSprite3, sf::Color::Cyan);
		}
	}
}

void Game::restartGame() {
	m_score = 0;
	m_lives = 3;
	m_isGameOver = false;
	m_baseMoveInterval = 30;
	m_alienMoveInterval = m_baseMoveInterval;

	m_playerBullets.clear();
	m_aliensBullets.clear();
	m_player = std::make_unique<Player>();

	spawnAliens();
}

void Game::run() {
	while (m_window->isOpen()) {
		processEvents();
		update();
		render();
	}
}

void Game::processEvents() const {
	while (const std::optional event = m_window->pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			m_window->close();
		}
	}
}

void Game::update() {
    // 0. СОСТОЯНИЕ ИГРЫ (GAME OVER)
    if (m_isGameOver) {
        if (isKeyPressed(sf::Keyboard::Key::Enter)) {
            restartGame();
        }
        return;
    }

		const std::string title = "Space Invaders | Score: " + std::to_string(m_score) +
                        " | Lives: " + std::to_string(m_lives);
    m_window->setTitle(title);

    // 1. ПРОВЕРКА ПОБЕДЫ В ВОЛНЕ
    if (m_aliens.empty()) {
        m_baseMoveInterval = std::max(2, m_baseMoveInterval - 5);
        m_alienMoveInterval = m_baseMoveInterval;

        m_playerBullets.clear();
        m_aliensBullets.clear();
        spawnAliens();
        return;
    }

    // 2. ИГРОК (Движение и Стрельба)
    int playerSpeed = 2;
    if (isKeyPressed(sf::Keyboard::Key::Left)) {
        m_player->move(-playerSpeed);
    }
    if (isKeyPressed(sf::Keyboard::Key::Right)) {
        m_player->move(playerSpeed);
    }

    // Стрельба (только если нет активной пули игрока)
    if (isKeyPressed(sf::Keyboard::Key::Space) && m_playerBullets.empty()) {
        m_playerBullets.emplace_back(m_player->getX() + 5,
        	m_player->getY() - 4, -4, playerBulletSprite, sf::Color::White);
    }

    // 3. ПУЛИ ИГРОКА И НЛО
    // Обновляем пули игрока
    for (auto& bullet : m_playerBullets) bullet.update();
    // Удаляем улетевшие
    std::erase_if(m_playerBullets, [](const Projectile &p) { return !p.active; });

    // Логика НЛО
    if (!m_ufoActive) {
        m_ufoTimer++;
        if (m_ufoTimer >= m_ufoSpawnTime) {
            m_ufoActive = true;
            m_ufoTimer = 0;
            if (m_rng() % 2 == 0) { m_ufoX = -16; m_ufoDirection = 1; }
            else { m_ufoX = 224; m_ufoDirection = -1; }
        }
    } else {
        m_ufoX += m_ufoDirection;
        if (m_ufoX < -20 || m_ufoX > 240) m_ufoActive = false;
    }

    // 4. ДВИЖЕНИЕ ПРИШЕЛЬЦЕВ
    m_alienMoveTimer++;
    if (m_alienMoveTimer >= m_alienMoveInterval) {
        m_alienMoveTimer = 0;
        bool hitEdge = false;

        for (const auto& alien : m_aliens) {
            // УСЛОВИЕ ПРОИГРЫША #1: Вторжение (коснулись линии бункеров)
            if (alien.getY() >= 192) {
                m_isGameOver = true;
            }

            if (m_alienDirection == 1 && alien.isAtRightEdge()) hitEdge = true;
            if (m_alienDirection == -1 && alien.isAtLeftEdge()) hitEdge = true;
        }

        if (hitEdge) {
            m_alienDirection = -m_alienDirection;
            for (auto& alien : m_aliens) alien.move_alien(0, 8);
        } else {
            for (auto& alien : m_aliens) alien.move_alien(4 * m_alienDirection, 0);
        }
    }

    // 5. СТРЕЛЬБА ПРИШЕЛЬЦЕВ
    m_alienShootTimer++;
    if (m_alienShootTimer >= m_alienShootInterval && !m_aliens.empty()) {
        m_alienShootTimer = 0;
        std::uniform_int_distribution<> dist(0, m_aliens.size() - 1);
        const auto& shooter = m_aliens[dist(m_rng)];

        auto bulletSprite = (m_rng() % 2 == 0) ? alienBulletSprite1 : alienBulletSprite2;
        m_aliensBullets.emplace_back(shooter.getX() + 4, shooter.getY() + 8,
        	2, bulletSprite, sf::Color::White);
    }

    for (auto& bullet : m_aliensBullets) bullet.update();
    std::erase_if(m_aliensBullets, [](const Projectile &p) { return !p.active; });

    // 6. ВСЕ КОЛЛИЗИИ

    // Координаты 4-х бункеров
		const std::vector bunkerXs = {24, 72, 120, 168};
    int bunkerYCoord = 192;
    sf::Vector2i bunkerSize(24, 16);

    // --- А) ПУЛИ ИГРОКА ---
    for (auto& bullet : m_playerBullets) {
        if (!bullet.active) continue;

        // 1. Попадание в ВРАГОВ
        for (auto it = m_aliens.begin(); it != m_aliens.end(); ) {
            if (checkCollision(bullet, *it)) {
                bullet.active = false;

							if (const int y = it->getY(); y < 64) m_score += 30;  // Кальмар (верхний)
                else if (y < 96) m_score += 20; // Краб (средний)
                else m_score += 10;             // Осьминог (нижний)

                it = m_aliens.erase(it);
                break;
            } else {
                ++it;
            }
        }
        if (!bullet.active) continue;

        // 2. Попадание в НЛО
        if (m_ufoActive) {
        	if (Alien fakeUfo(m_ufoX, 20, ufoSprite, sf::Color::Red); checkCollision(bullet, fakeUfo)) {
                 bullet.active = false;
                 m_ufoActive = false;
                 m_ufoTimer = 0;
                 m_score += 100;
             }
        }
        if (!bullet.active) continue;

        // 3. Попадание в БУНКЕРЫ
        for (int bx : bunkerXs) {
					if (sf::IntRect bunkerRect({bx, bunkerYCoord}, {bunkerSize.x, bunkerSize.y});
							bullet.getBounds().findIntersection(bunkerRect).has_value()) {
                bullet.active = false;
                break;
            }
        }
    }

    // --- Б) ПУЛИ ВРАГОВ ---
    for (auto& bullet : m_aliensBullets) {
        if (!bullet.active) continue;

        // 1. Попадание в ИГРОКА
        if (checkCollision(bullet, *m_player)) {
            bullet.active = false;
            m_lives--;

            m_playerBullets.clear();
            m_aliensBullets.clear();

            sleep(sf::milliseconds(500));

            // УСЛОВИЕ ПРОИГРЫША #2: Жизни кончились
            if (m_lives <= 0) {
                m_isGameOver = true;
            }
            continue;
        }

        // 2. Попадание в БУНКЕРЫ
        for (int bx : bunkerXs) {
					if (sf::IntRect bunkerRect({bx, bunkerYCoord}, {bunkerSize.x, bunkerSize.y});
							bullet.getBounds().findIntersection(bunkerRect).has_value()) {
                bullet.active = false;
                break;
            }
        }

        // 3. Попадание пули в пулю (Аннигиляция)
        for (auto& pBullet : m_playerBullets) {
            if (pBullet.active && checkCollision(bullet, pBullet)) {
                bullet.active = false;
                pBullet.active = false;
                m_score += 1;
                break;
            }
        }
    }
}

void Game::render() const {
    m_frameBuffer->clear(sf::Color::Black);

    // ОТРИСОВКА ОБЪЕКТОВ

    // 1. НЛО (Красный корабль)
    if (m_ufoActive) {
        m_frameBuffer->drawSprite(m_ufoX, 20, ufoSprite, sf::Color::Red);
    }

    // 2. Пришельцы
    for (const auto& alien : m_aliens) {
        alien.draw(*m_frameBuffer);
    }

    // 3. Игрок
    m_player->draw(*m_frameBuffer);

    // 4. Пули (игрока и врагов)
    for (const auto& bullet : m_playerBullets) {
        bullet.draw(*m_frameBuffer);
    }
    for (const auto& bullet : m_aliensBullets) {
        bullet.draw(*m_frameBuffer);
    }

    // 5. Бункеры

		for (const std::vector bunkerXs = {24, 72, 120, 168}; const int bx : bunkerXs) {
			int bunkerY = 192;
        m_frameBuffer->drawSprite(bx, bunkerY, bunkerSprite, sf::Color::Green);
    }

    // ЭКРАН GAME OVER
	if (m_isGameOver) {
		// Рисуем красные полосы поверх игры
		for (size_t y = 0; y < 256; y += 2) {
			for (size_t x = 0; x < 224; ++x) {
				m_frameBuffer->setPixel(x, y, sf::Color::Red);
			}
		}
	}

    // ВЫВОД НА ЭКРАН

    // Обновляем текстуру из буфера пикселей
    m_frameBuffer->update();

    m_window->clear();
    m_frameBuffer->draw(*m_window);
    m_window->display();
}