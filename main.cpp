#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include "./SystemAbstraction/system_abstraction.hpp"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

extern struct nk_context ctx;
GLFWwindow *win;
int window_width = 0;
int window_height = 0;
int framebuffer_width=0;
int framebuffer_height=0;
struct nk_vec2 scale;


static void error_callback(int e, const char *d)
{
    printf("Error %d: %s\n", e, d);
}

void mouseButtonCallback(GLFWwindow* win,int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(win, &x, &y);

    x = x*scale.x;
    y = y*scale.y;

    if(action == GLFW_PRESS)
    {
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_LEFT_BUTTON, SystemAbstraction::EVENT_DOWN, x, y);
        }
        else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_MIDDLE_BUTTON, SystemAbstraction::EVENT_DOWN, x, y);
        }
        else if(button == GLFW_MOUSE_BUTTON_RIGHT)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_RIGHT_BUTTON, SystemAbstraction::EVENT_DOWN, x, y);
        }
    }
    else if(action == GLFW_RELEASE)
    {
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_LEFT_BUTTON, SystemAbstraction::EVENT_UP, x, y);
        }
        else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_MIDDLE_BUTTON, SystemAbstraction::EVENT_UP, x, y);
        }
        else if(button == GLFW_MOUSE_BUTTON_RIGHT)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_RIGHT_BUTTON, SystemAbstraction::EVENT_UP, x, y);
        }
    }
}

void cursorPosCallback(GLFWwindow* win,double xpos, double ypos)
{
    PointerCoords coords;
    coords.x = xpos*scale.x;
    coords.y = ypos*scale.y;
    SystemAbstraction::onPointerMove(0, &coords);
}

void keyCallback(GLFWwindow *win,int key, int scancode, int action, int mods)
{


    nk_input_key(&ctx, NK_KEY_DEL, ((key == GLFW_KEY_DELETE) && (action == GLFW_PRESS)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_ENTER, ((key == GLFW_KEY_ENTER) && (action == GLFW_PRESS)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_TAB, ((key == GLFW_KEY_TAB) && (action == GLFW_PRESS)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_BACKSPACE, ((key == GLFW_KEY_BACKSPACE) && (action == GLFW_PRESS)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_LEFT, ((key == GLFW_KEY_LEFT) && (action == GLFW_PRESS)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_RIGHT, ((key == GLFW_KEY_RIGHT) && (action == GLFW_PRESS)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_UP, ((key == GLFW_KEY_UP) && (action == GLFW_PRESS)) ? 1 : 0 );
    nk_input_key(&ctx, NK_KEY_DOWN, ((key == GLFW_KEY_DOWN) && (action == GLFW_PRESS)) ? 1 : 0 );


    if (mods & GLFW_MOD_CONTROL)
    {
        nk_input_key(&ctx, NK_KEY_COPY, ((key == GLFW_KEY_C) && (action == GLFW_PRESS)) ? 1 : 0 );
        nk_input_key(&ctx, NK_KEY_PASTE, ((key == GLFW_KEY_V) && (action == GLFW_PRESS)) ? 1 : 0 );
        nk_input_key(&ctx, NK_KEY_CUT, ((key == GLFW_KEY_X) && (action == GLFW_PRESS)) ? 1 : 0 );
        nk_input_key(&ctx, NK_KEY_CUT, ((key == GLFW_KEY_E) && (action == GLFW_PRESS)) ? 1 : 0 );
        nk_input_key(&ctx, NK_KEY_SHIFT, 1);
    }
    else
    {
        nk_input_key(&ctx, NK_KEY_COPY, 0);
        nk_input_key(&ctx, NK_KEY_PASTE, 0);
        nk_input_key(&ctx, NK_KEY_CUT, 0);
        nk_input_key(&ctx, NK_KEY_SHIFT, 0);
    }

}

void charCallback(GLFWwindow *win, unsigned int codepoint)
{
    SystemAbstraction::onChar(codepoint);
}

static void scrollCallback(GLFWwindow *win, double, double yoff)
{
    yoff = yoff*scale.y;
    SystemAbstraction::onScroll(yoff);
}

static void framebufferSizeCallback(GLFWwindow* win,int width,int height)
{
    width = width*scale.x;
    height = height*scale.y;
    SystemAbstraction::onFramebufferResize(width, height);
}

int main(int argc, char *argv[])
{
    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    // OPEN GL ES
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
    glfwMakeContextCurrent(win);

    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    glfwSetCursorPosCallback(win, cursorPosCallback);
    glfwSetCharCallback(win, charCallback);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetScrollCallback(win, scrollCallback);
    glfwSetFramebufferSizeCallback(win, framebufferSizeCallback);

    glfwGetWindowSize(win, &window_width, &window_height);
    glfwGetFramebufferSize(win, &framebuffer_width, &framebuffer_height);

    scale.x = (float)framebuffer_width/(float)window_width;
    scale.y = (float)framebuffer_height/(float)window_height;

    SystemAbstraction::onInit(framebuffer_width, framebuffer_height);

    while (!glfwWindowShouldClose(win))
    {
        glfwPollEvents();
        SystemAbstraction::onRenderFrame();
        glfwSwapBuffers(win);
    }

    //DELETE
    SystemAbstraction::onUninit();
    glfwTerminate();
    return 0;
}
