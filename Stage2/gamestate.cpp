#include "gamestate.h"
#include "level.h"
#include "player.h"
#include <thread>
#include <chrono>
GameState::GameState()
{
}

GameState::~GameState()
{
	if (m_current_level)
		delete m_current_level;
}

GameState* GameState::getInstance()
{
	if (!m_unique_instance)
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance;
}

bool GameState::init()
{
	
		m_current_level = new Level("1.lvl");
		m_current_level->init();
	
		m_player = new Player("Player");
		m_player->init();

		graphics::preloadBitmaps(getAssetDir());
		graphics::setFont(m_asset_path + "OpenSans-Regular.ttf");

		return true;
}

void GameState::draw()
{
	
	
		if (!m_current_level)
			return;

		m_current_level->draw();
	
	
}

void GameState::update(float dt)
{
	// Skip an update if a long delay is detected 
	// to avoid messing up the collision simulation
	
		if (dt > 500) // ms
			return;
		float sleep_time = std::max(0.0f, 10.0f - dt);
		std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));

		if (!m_current_level)
			return;

		m_current_level->update(dt);

		m_debugging = graphics::getKeyState(graphics::SCANCODE_0);
	

}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}

GameState* GameState::m_unique_instance = nullptr;