#include <sgg/graphics.h>
#include "config.h"
#include "gamestate.h"


void draw()
{
        GameState::getInstance()->draw();
}

void update(float dt)
{
        GameState::getInstance()->update(dt);
}
void init()
{
 
}

int main(int argc, char** argv)
{
    
        graphics::createWindow(800, 800, "Boing");

        GameState::getInstance()->init();
        graphics::playMusic(GameState::getInstance()->getFullAssetPath("maze.mp3"), 0.1f);
        graphics::setDrawFunction(draw);
        graphics::setUpdateFunction(update);
        graphics::setCanvasSize(GameState::getInstance()->getCanvasWidth(), GameState::getInstance()->getCanvasHeight());
        graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
        graphics::startMessageLoop();
	    return 0;
}

