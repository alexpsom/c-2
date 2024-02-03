#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"




class Player : public Box, public GameObject
{
	// float Stage 1 variables
	graphics::Brush m_brush_player;
	
	std::vector<std::string>m_sprites;

public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	Player(std::string name) : GameObject(name) {}
protected:
	void debugDraw();
};
