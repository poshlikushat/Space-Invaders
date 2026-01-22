#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "FrameBuffer.hpp"
#include "Player.hpp"
#include <Alien.hpp>
#include "Bullet.hpp"
#include <random>

class Game {
public:
	Game();
	void run();

private:
	void processEvents() const;
	void update();
	void render() const;

	void spawnAliens();

	void restartGame();

	template<typename T1, typename T2>
	static bool checkCollision(const T1& obj1, const T2& obj2) {
		return obj1.getBounds().findIntersection(obj2.getBounds()).has_value();
	}

	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<FrameBuffer> m_frameBuffer;
	std::unique_ptr<Player> m_player;

	std::vector<Alien> m_aliens;
	std::vector<Projectile> m_playerBullets;
	std::vector<Projectile> m_aliensBullets;

	// ЛОГИКА ИГРЫ
	int m_score = 0;
	int m_lives = 3;
	bool m_isGameOver = false;

	// Настройки сложности
	int m_alienMoveInterval = 30;
	int m_baseMoveInterval = 30;

	int m_alienDirection = 1;
	int m_alienMoveTimer = 0;

	int m_alienShootTimer = 0;
	const int m_alienShootInterval = 60;
	std::mt19937 m_rng;

	bool m_ufoActive = false;
	int m_ufoX = -20;
	int m_ufoTimer = 0;
	const int m_ufoSpawnTime = 600;
	int m_ufoDirection = 1;
};