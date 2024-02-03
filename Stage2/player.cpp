#include "player.h"
#include "util.h"
#include <cmath>



void Player::update(float dt)
{
	float delta_time = dt / 1000.0f;

	const float velocity = 5.f;
	if (graphics::getKeyState(graphics::SCANCODE_A))
		m_pos_x -= delta_time * velocity;
	if (graphics::getKeyState(graphics::SCANCODE_D))
		m_pos_x += delta_time * velocity;
	if (graphics::getKeyState(graphics::SCANCODE_W))
		m_pos_y -= delta_time * velocity;
	if (graphics::getKeyState(graphics::SCANCODE_S))
		m_pos_y += delta_time * velocity;
	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
			m_pos_y = 14.0f;
	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
			m_pos_x = -2.0f;

	//printf("vx = %f, vy = %f, x = %f, y = %f\n", m_vx, m_vy, m_center_x, m_center_y);

	// update offset for other game objects
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;
	
	GameObject::update(dt);

}

void Player::draw()
{
	int s = (int)fmodf(1000.0f-m_pos_x*1.f, m_sprites.size());
	m_brush_player.texture = m_sprites[s];
	
	
	graphics::drawRect(m_state->getCanvasWidth()*0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
	graphics::resetPose();
	if (m_state->m_debugging)
		debugDraw();

	

	
}

void Player::init()
{
	// stage 1
	m_pos_x = -2.0f;
	m_pos_y = 14.0f;
	m_width /= 1.4f;
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.texture = m_state->getFullAssetPath("Boing-turn.png");

	m_sprites.push_back(m_state->getFullAssetPath("Boing-turn3.png"));
	m_sprites.push_back(m_state->getFullAssetPath("Boing-turn4.png"));
	m_sprites.push_back(m_state->getFullAssetPath("Boing-turn5.png"));
	m_sprites.push_back(m_state->getFullAssetPath("Boing-turn6.png"));
	m_sprites.push_back(m_state->getFullAssetPath("Boing-turn7.png"));
	m_sprites.push_back(m_state->getFullAssetPath("Boing-turn8.png"));

	
	
	
}



void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth()*0.5f, m_state->getCanvasHeight() * 0.5f, m_width, m_height, debug_brush);
	
	char s[20];
	sprintf_s(s,"(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f, m_state->getCanvasHeight() * 0.5f - 0.6f, 0.15f, s, debug_brush);
}

