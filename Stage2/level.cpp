#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "util.h"


void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	float x = box.m_pos_x + m_state->m_global_offset_x;
	float y = box.m_pos_y + m_state->m_global_offset_y;
	m_block_brush.texture = m_state->getFullAssetPath(m_block_names[i]);
	graphics::drawRect(x, y, 1.0f*m_block_size, 1.0f*m_block_size, m_block_brush);
	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);
}

void Level::drawCheckpoint(int i)
{
	Box& box = m_checkpoint[i];
	float x = box.m_pos_x + m_state->m_global_offset_x;
	float y = box.m_pos_y + m_state->m_global_offset_y;
	m_checkpoint_brush.texture = m_state->getFullAssetPath(m_checkpoint_names[i]);
	graphics::drawRect(x, y, 1.0f * m_checkpoint_size, 1.0f * m_checkpoint_size, m_checkpoint_brush);
	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_checkpoint_size, m_checkpoint_size, m_checkpoint_brush_debug);
}

void Level::drawFakecheckpoint(int i)
{
	Box& box = m_fake_checkpoint[i];
	float x = box.m_pos_x + m_state->m_global_offset_x;
	float y = box.m_pos_y + m_state->m_global_offset_y;
	m_fake_checkpoint_brush.texture = m_state->getFullAssetPath(m_fake_checkpoint_names[i]);
	graphics::drawRect(x, y, 1.0f * m_fake_checkpoint_size, 1.0f * m_fake_checkpoint_size, m_fake_checkpoint_brush);
	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_fake_checkpoint_size, m_fake_checkpoint_size, m_fake_checkpoint_brush_debug);
}

void Level::checkCollisions()
{
	for (auto& box : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(box))
		{
			m_state->getPlayer()->m_pos_y += offset;
			graphics::playSound(m_state->getFullAssetPath("Record_online-voice-recorder.com.mp3"), 0.5f);
			break;
		}
	}
	for (auto& box : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectUP(box))
		{
			m_state->getPlayer()->m_pos_y -= offset;
			graphics::playSound(m_state->getFullAssetPath("Record_online-voice-recorder.com.mp3"), 0.5f);
			break;
		}
	}
	for (auto& box : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSide(box))
		{
			m_state->getPlayer()->m_pos_x -= offset;
			graphics::playSound(m_state->getFullAssetPath("Record_online-voice-recorder.com.mp3"), 0.5f);
			break;
		}
	}
	
	for (auto& box : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(box))
		{
			m_state->getPlayer()->m_pos_x += offset;
			break;
		}
	}

	for (auto& box : m_fake_checkpoint)
	{
		float offset1 = 0.0f;
		if (offset1 = m_state->getPlayer()->intersect(box))
		{
			drainlife(0.03f);
		}
	}
	for (auto& box : m_checkpoint)
	{
		float offset1 = 0.0f;
		if (offset1 = m_state->getPlayer()->intersect(box))
		{
			
			graphics::destroyWindow();
		}
	}
}


void Level::update(float dt)
{
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	checkCollisions();

	GameObject::update(dt);
	
	
}

void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x / 1.0 + w / 2.0f;
	float offset_y = m_state->m_global_offset_y / 1.0 + h / 2.0f;

	//draw background
	graphics::drawRect(offset_x, offset_y, 2.0f * w, 4.0f * w, m_brush_background);

	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	for (int i = 0; i < m_blocks.size(); i++)
	{
		drawBlock(i);
	}
	for (int j = 0; j < m_checkpoint.size(); j++)
	{
		drawCheckpoint(j);
	}
	for (int k = 0; k < m_checkpoint.size(); k++)
	{
		drawFakecheckpoint(k);
	}

	if (getRemainingLife() == 0)
	{

		graphics::destroyWindow();
	}
	
	
	float player_life = getRemainingLife();
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[1] = 0.2f;
	br.fill_secondary_color[2] = 0.2 *(1.0f - player_life) + player_life * 1.0f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(m_state->getCanvasWidth() /2.0f-1.5f, 0.5f,player_life* 1.0f, 0.3f, br);
	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	graphics::drawRect(m_state->getCanvasWidth() / 2.0f - 1.5f, 0.5f,  1.0f+1.0f,  0.3f, br);

}

void Level::init()
{
	// Stage 1
	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();
	
	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();
	
	m_checkpoint.push_back(Box(14.5 * m_checkpoint_size, -15.5 * m_checkpoint_size, m_checkpoint_size, m_checkpoint_size));
	m_checkpoint.push_back(Box(-3.5 * m_checkpoint_size, -100.5 * m_checkpoint_size, m_checkpoint_size, m_checkpoint_size));
	m_checkpoint.push_back(Box(3.5 * m_checkpoint_size, 200.5 * m_checkpoint_size, m_checkpoint_size, m_checkpoint_size));
	m_checkpoint.push_back(Box(5.5 * m_checkpoint_size, 100.5 * m_checkpoint_size, m_checkpoint_size, m_checkpoint_size));
	m_checkpoint_names.push_back("checkpoint2.png");
	m_checkpoint_names.push_back("checkpoint1.png");
	m_checkpoint_names.push_back("checkpoint2.png");
	m_checkpoint_names.push_back("checkpoint1.png");
	m_checkpoint_brush.outline_opacity = 0.0f;
	m_checkpoint_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.2f, 1.0f, 0.2f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);



	m_fake_checkpoint.push_back(Box(14.5 * m_fake_checkpoint_size, 3.5 * m_fake_checkpoint_size, m_fake_checkpoint_size, m_fake_checkpoint_size));
	m_fake_checkpoint.push_back(Box(-3.5 * m_fake_checkpoint_size, -15.5 * m_fake_checkpoint_size, m_fake_checkpoint_size, m_fake_checkpoint_size));
	m_fake_checkpoint.push_back(Box(3.5 * m_fake_checkpoint_size, 23.5 * m_fake_checkpoint_size, m_fake_checkpoint_size, m_fake_checkpoint_size));
	m_fake_checkpoint.push_back(Box(5.5 * m_fake_checkpoint_size, 4.5 * m_fake_checkpoint_size, m_fake_checkpoint_size, m_fake_checkpoint_size));

	m_fake_checkpoint_names.push_back("checkpoint5.png");
	m_fake_checkpoint_names.push_back("checkpoint5.png");
	m_fake_checkpoint_names.push_back("checkpoint5.png");
	m_fake_checkpoint_names.push_back("checkpoint5.png");
	m_fake_checkpoint_brush.outline_opacity = 0.0f;
	m_fake_checkpoint_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.2f, 1.0f, 0.2f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);





	m_blocks.push_back(Box(-6.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -16.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -15.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -13.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -12.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -11.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -7.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -4.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -3.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -1.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, -0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 1.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 3.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 4.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 7.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 8.5 * m_block_size, m_block_size, m_block_size)); 
	m_blocks.push_back(Box(18.5 * m_block_size, 9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size,11.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 12.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 13.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 15.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size,16.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 19.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 20.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 22.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 23.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 24.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 26.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 27.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 28.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(16.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(15.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(14.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-4.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-3.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-2.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(0.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -16.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -15.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -13.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -12.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -11.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -7.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -4.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -3.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -1.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, -0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 1.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 3.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 4.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 7.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 11.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 12.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 13.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 15.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 16.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 19.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 20.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 22.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 23.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 24.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 26.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 27.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 28.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-6.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));

	m_blocks.push_back(Box(-6.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-4.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-3.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-2.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(0.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(14.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(15.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(16.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(18.5 * m_block_size, 30.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -17 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -16 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -15 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -13 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -12 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -11 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -10 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -9 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -8 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -7 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -4 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -3 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -2 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -1 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 0 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 1 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 2 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 3 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 4 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 7 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 8 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 9 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 10 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 11 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 12 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 13 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 15 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 16 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 17* m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 18 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 19 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 20 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 21 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 22 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 23 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5* m_block_size, 24 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 25 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 26 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 27 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 28 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 29 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-4.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-3.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-2.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(0.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(14.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(15.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(16.5 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17 * m_block_size, 29.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 29 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 28 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 27 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 26 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 25 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 24 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 23 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 22 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 21 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 20 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 19 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 18 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 17 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 16 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 15 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 13 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 12 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 11 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 10 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 9 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 8 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 7 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 4 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 3 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 2 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 1 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, 0 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -1 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -2 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -3 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -4 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -7 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -8 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -9 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -10 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -11 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -12 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -13 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -15 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -16 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -17 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(17.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(16.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(15.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(14.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(0.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-2.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-3.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-4.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, -18.5 * m_block_size, m_block_size, m_block_size));
	
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	
	
	m_blocks.push_back(Box(-4.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-3.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-2.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 24.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 25.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(16.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(15.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(14.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, 20.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, 19.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(14.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 23.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 22.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(0.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 20.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 19.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 16.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 15.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 7.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-2.5 * m_block_size, 0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-3.5 * m_block_size, 0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-4.5 * m_block_size, 0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-5.5 * m_block_size, 0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, 5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(0.5 * m_block_size, 5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, 5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 4.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 7.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 9.5 * m_block_size, m_block_size, m_block_size));
	
	m_blocks.push_back(Box(-0.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(0.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(0.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 15.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 16.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 17.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, 18.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, 19.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, 20.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, 21.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 4.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 7.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 11.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 12.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 13.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 14.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 13.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 12.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 11.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(16.5 * m_block_size, 6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(15.5 * m_block_size, 6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(14.5 * m_block_size, 6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(13.5 * m_block_size, 6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(12.5 * m_block_size, 6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 6 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 4 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 3 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 2 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 1 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, 0 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -1 * m_block_size, m_block_size, m_block_size)); 
	m_blocks.push_back(Box(11.5 * m_block_size, -2 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, -2 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, -2 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -2 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, -2 * m_block_size, m_block_size, m_block_size));
	
	m_blocks.push_back(Box(3.5 * m_block_size, -5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -7.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -1.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -3.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -4.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 3.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, 2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, 2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, 2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, 2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 1.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, 3.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -0.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -1.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -2.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -3.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -4.5 * m_block_size, m_block_size, m_block_size)); 
	m_blocks.push_back(Box(-1.5 * m_block_size, -5.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -6.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -7.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-1.5 * m_block_size, -8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-2.5 * m_block_size, -8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-3.5 * m_block_size, -8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-4.5 * m_block_size, -8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -16.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -16 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -15 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(11.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(2.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(1.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(0.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, -14 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, -15 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, -16 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(-0.5 * m_block_size, -16.5 * m_block_size, m_block_size, m_block_size));
	

	m_blocks.push_back(Box(11.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(10.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(9.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(7.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -10.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -11.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -12.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(8.5 * m_block_size, -13 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(6.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(5.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(4.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -9.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -8.5 * m_block_size, m_block_size, m_block_size));
	m_blocks.push_back(Box(3.5 * m_block_size, -7.5 * m_block_size, m_block_size, m_block_size));
	

	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	m_block_names.push_back("block6.png");
	m_block_names.push_back("block5.png");
	m_block_names.push_back("block4.png");
	m_block_names.push_back("block.png");
	




	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.2f, 1.0f, 0.2f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);

}

Level::Level(const std::string & name)
	: GameObject(name)
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("background.png");
}

Level::~Level()
{
	for (auto p_go : m_static_objects)
		delete p_go;
	for (auto p_go : m_dynamic_objects)
		delete p_go;
}
