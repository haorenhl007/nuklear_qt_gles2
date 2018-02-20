#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>


#include "./SystemAbstraction/system_abstraction.hpp"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

extern struct nk_context ctx;

/* glfw callbacks (I don't know if there is a easier way to access text and scroll )*/
static void error_callback(int e, const char *d)
{
    printf("Error %d: %s\n", e, d);
}

void mouseButtonCallback(GLFWwindow* win,int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(win, &x, &y);
    nk_input_button(&ctx, NK_BUTTON_LEFT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    nk_input_button(&ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    nk_input_button(&ctx, NK_BUTTON_RIGHT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
}

void cursorPosCallback(GLFWwindow* win,double xpos, double ypos)
{
    nk_input_motion(&ctx, (int)xpos, (int)ypos);
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


    if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        nk_input_key(&ctx, NK_KEY_COPY, glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS);
        nk_input_key(&ctx, NK_KEY_PASTE, glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS);
        nk_input_key(&ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS);
        nk_input_key(&ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);
        nk_input_key(&ctx, NK_KEY_SHIFT, 1);
    }else
    {
        nk_input_key(&ctx, NK_KEY_COPY, 0);
        nk_input_key(&ctx, NK_KEY_PASTE, 0);
        nk_input_key(&ctx, NK_KEY_CUT, 0);
        nk_input_key(&ctx, NK_KEY_SHIFT, 0);
    }
}

void charCallback(GLFWwindow *win, unsigned int codepoint)
{
    nk_input_unicode((struct nk_context*)glfwGetWindowUserPointer(win), codepoint);
}

static void scroll_input(GLFWwindow *win, double, double yoff)
{
    nk_input_scroll((struct nk_context*)glfwGetWindowUserPointer(win), nk_vec2(0, (float)yoff));
}

static GLFWwindow *win;
int window_width = 0;
int window_height = 0;
int framebuffer_width=0;
int framebuffer_height=0;
struct nk_vec2 scale;

int main(int argc, char *argv[])
{
    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    // OPEN GL ES
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwSetWindowUserPointer(win, &ctx);
    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    glfwSetCursorPosCallback(win, cursorPosCallback);
    glfwSetCharCallback(win, charCallback);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetScrollCallback(win, scroll_input);
    glfwGetWindowSize(win, &window_width, &window_height);
    glfwGetFramebufferSize(win, &framebuffer_width, &framebuffer_height);


    SystemAbstraction::onInit(window_width, window_height);


    while (!glfwWindowShouldClose(win))
    {
        /* High DPI displays */
        glfwGetWindowSize(win, &window_width, &window_height);
        glfwGetFramebufferSize(win, &framebuffer_width, &framebuffer_height);
        scale.x = (float)framebuffer_width/(float)window_width;
        scale.y = (float)framebuffer_height/(float)window_height;

        /* Input */

        nk_input_begin(&ctx);
        {
            glfwPollEvents();
        }
        nk_input_end(&ctx);

        SystemAbstraction::onRenderFrame();

        glfwSwapBuffers(win);
    }

    SystemAbstraction::onUninit();


    glfwTerminate();
    return 0;
}
