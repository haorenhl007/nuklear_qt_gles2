#include "system_abstraction.hpp"
#include "demo.hpp"

int SystemAbstraction::framebuffer_width = 0;
int SystemAbstraction::framebuffer_height = 0;

void SystemAbstraction::onInit(unsigned int fb_width, unsigned int fb_height)
{
    framebuffer_width = fb_width;
    framebuffer_height = fb_height;
    demo_init(framebuffer_width, framebuffer_height);
}

void SystemAbstraction::onPause()
{

}

void SystemAbstraction::onResume()
{

}

void SystemAbstraction::onFramebufferResize(unsigned int fb_width, unsigned int fb_height)
{
    framebuffer_width = fb_width;
    framebuffer_height = fb_height;
}

void SystemAbstraction::onRenderFirstFrame()
{

}

void SystemAbstraction::onRenderFrame()
{
    /* Draw */
    glViewport(0, 0, framebuffer_width, framebuffer_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    demo_render(framebuffer_width,framebuffer_height);
}

void SystemAbstraction::onScroll(double yoffset)
{
    demo_onScrollCallback(yoffset);
}

void SystemAbstraction::onMouseButton(MouseButton mouseButton, ButtonEvent event, int x, int y)
{
    demo_onMouseButtonCallback(mouseButton, event, x, y);
}

void SystemAbstraction::onKey(ButtonEvent event, Key key, Mods mods, int x, int y )
{
    demo_onKeyCallback(event, key, mods, x, y);
}

void SystemAbstraction::onChar(unsigned int codepoint)
{
    demo_onCharCallback(codepoint);
}

bool SystemAbstraction::onBackKeyPressed()
{

}

void SystemAbstraction::onJoyUpdate(float joyX, float joyY)
{

}

void SystemAbstraction::onPointerDown(int pointerId, const struct PointerCoords *coords)
{

}

void SystemAbstraction::onPointerUp(int pointerId, const struct PointerCoords *coords)
{

}

void SystemAbstraction::onPointerMove(int pointerId, const struct PointerCoords *coords)
{
    demo_onPointerMoveCallback(pointerId, coords);
}

void SystemAbstraction::onTimerTick()
{

}

void SystemAbstraction::onUninit()
{
    demo_uninit();
}


