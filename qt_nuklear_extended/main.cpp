#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include "./SystemAbstraction/system_abstraction.hpp"
#include "./SystemAbstraction/system_paths.hpp"

using namespace std;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

GLFWwindow *win;
int window_width = 0;
int window_height = 0;
int framebuffer_width=0;
int framebuffer_height=0;
float scale_x;
float scale_y;


//****************************************************************
//**************** GLFW CALLBACK *********************************
//****************************************************************

void errorCallback(int e, const char *d)
{
    printf("Error %d: %s\n", e, d);
}

void mouseButtonCallback(GLFWwindow* win,int button, int action, int mods)
{
    double win_cursor_x, win_cursor_y;
    glfwGetCursorPos(win, &win_cursor_x, &win_cursor_y);

    int fb_cursor_x = win_cursor_x*scale_x;
    int fb_cursor_y = win_cursor_y*scale_y;

    if(action == GLFW_PRESS)
    {
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_LEFT_BUTTON, SystemAbstraction::EVENT_DOWN, fb_cursor_x, fb_cursor_y);
        }
        else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_MIDDLE_BUTTON, SystemAbstraction::EVENT_DOWN, fb_cursor_x, fb_cursor_y);
        }
        else if(button == GLFW_MOUSE_BUTTON_RIGHT)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_RIGHT_BUTTON, SystemAbstraction::EVENT_DOWN, fb_cursor_x, fb_cursor_y);
        }
    }
    else if(action == GLFW_RELEASE)
    {
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_LEFT_BUTTON, SystemAbstraction::EVENT_UP, fb_cursor_x, fb_cursor_y);
        }
        else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_MIDDLE_BUTTON, SystemAbstraction::EVENT_UP, fb_cursor_x, fb_cursor_y);
        }
        else if(button == GLFW_MOUSE_BUTTON_RIGHT)
        {
               SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_RIGHT_BUTTON, SystemAbstraction::EVENT_UP, fb_cursor_x, fb_cursor_y);
        }
    }

    PointerCoords coords;
    memset(&coords, 0, sizeof(coords));
    coords.x = fb_cursor_x;
    coords.y = fb_cursor_y;
    coords.minX = 0;
    coords.maxX = 0;
    coords.minY = 0;
    coords.maxY = 0;
    coords.isScreen = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        SystemAbstraction::onPointerDown(0, &coords);
    }else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        SystemAbstraction::onPointerUp(0, &coords);
    }
}

void cursorPosCallback(GLFWwindow* win,double xpos, double ypos)
{
//    if(GLFW_RELEASE == glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT))
//    {
//        return;
//    }

    PointerCoords coords;
    memset(&coords, 0, sizeof(coords));
    coords.x = xpos*scale_x;
    coords.y = ypos*scale_y;
    coords.minX = 0;
    coords.maxX = 0;
    coords.minY = 0;
    coords.maxY = 0;
    coords.isScreen = true;
    SystemAbstraction::onPointerMove(0, &coords);
}

void charCallback(GLFWwindow *win, unsigned int codepoint)
{
    SystemAbstraction::onChar(codepoint);
}

void keyCallback(GLFWwindow *win,int key, int scancode, int action, int mods)
{
    SystemAbstraction::onKey((SystemAbstraction::ButtonEvent)action, (SystemAbstraction::Key)key, (SystemAbstraction::Mods)mods, 0, 0);

    if(action == GLFW_RELEASE && (key == 'q' || key == 'Q'))
    {
        SystemAbstraction::onBackKeyPressed();
    }
}

void scrollCallback(GLFWwindow *win, double, double yoff)
{
    yoff = yoff*scale_y;
    SystemAbstraction::onScroll(yoff);
}

void framebufferSizeCallback(GLFWwindow* win,int width,int height)
{
    int fb_width = width*scale_x;
    int fb_height = height*scale_y;
    SystemAbstraction::onFramebufferResize(fb_width, fb_height);
}

//****************************************************************
//**************** END CALLBACK **********************************
//****************************************************************

int main(int argc, char *argv[])
{
    /* GLFW */
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
    {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(EXIT_FAILURE);
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

    //SCALE FAMEBUFFER TO WINDOW SIZE
    glfwGetWindowSize(win, &window_width, &window_height);
    glfwGetFramebufferSize(win, &framebuffer_width, &framebuffer_height);
    scale_x = (float)framebuffer_width/(float)window_width;
    scale_y = (float)framebuffer_height/(float)window_height;

    //SYSTEM ABSTRACTION INIT
    string homePath = getenv("HOME");
    string configPath = homePath + "/.config";
    systemInput_initConfigPath(configPath);
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
