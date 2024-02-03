#pragma once

#include "gameobject.h"
#include <vector>
#include <list>
#include <string>
#include <sgg/graphics.h>
#include "player.h"


class Level :public GameObject
{
	
	graphics::Brush m_brush_background;
	graphics::Brush br;

	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_dynamic_objects;
	std::vector<Box> m_blocks;
	std::vector<std::string>m_block_names;

	std::vector<Box> m_checkpoint;
	std::vector<std::string>m_checkpoint_names;
	const float m_checkpoint_size = 0.5f;
	graphics::Brush m_checkpoint_brush;
	graphics::Brush m_checkpoint_brush_debug;

	std::vector<Box> m_fake_checkpoint;
	std::vector<std::string>m_fake_checkpoint_names;
	const float m_fake_checkpoint_size = 0.5f;
	graphics::Brush m_fake_checkpoint_brush;
	graphics::Brush m_fake_checkpoint_brush_debug;
	

	const float m_block_size = 0.5f;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;
	float life = 2.0f;
	float m_center_x = 5.0f;
	float m_center_y = 5.0f;
	void drawBlock(int i);
	void drawCheckpoint(int i);
	void drawFakecheckpoint(int i);
	void checkCollisions();
public:
	float getLife() const {
		return life;
	};
	void drainlife(float amount) { life = std::max<float>(0.0f, life - amount); }

	float getRemainingLife() const { return life; }
	void update(float dt) override;
	void draw() override;
	void init() override;
	Level(const std::string & name = "Level0");
	~Level() override;
};