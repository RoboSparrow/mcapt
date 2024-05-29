#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// To make sure there will be no header conflicts, you can define GLFW_INCLUDE_NONE before the GLFW header
//   to explicitly disable inclusion of the development environment header.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "mlog.h"
#include "mscreen.h"


extern int verbose;


void mscreen_print(FILE *fp, struct mscreen *sdata) {
    if(!fp || !sdata) {
        return;
    }
    fprintf(
        fp,
        "{ display: %d, screenw: %d, screenh: %d, mousex: %d, mousey: %d }",
        sdata->display,
        sdata->screenw,
        sdata->screenh,
        sdata->mousex,
        sdata->mousey
    );
}

int mscreen_dlog(FILE *fp, struct mscreen *sdata) {
    if(!fp) {
        LOG_ERROR("Could not write to dlog file (NULL)");
        return 1;
    }
    if(!sdata) {
        LOG_ERROR("no screendata to write (NULL)");
        return 1;
    }
    fprintf(
        fp,
        "; display=%d, screenw=%d, screenh=%d, mousex=%d, mousey=%d\n",
        sdata->display,
        sdata->screenw,
        sdata->screenh,
        sdata->mousex,
        sdata->mousey
    );
    return 0;
}



static void _error_callback(int err, const char* reason) {
    LOG_ERROR_F("GFLW3 Error %d: %s\n", err, reason);
}

/**
 * Spawns a temporary hidden fullscreen SDL window in order to capture screen and absolute mouse position.
 * returns 0 on success reading or -1 on error
 *
 * Usage example:
 *     struct mscreen sdata = { 0 };
 *     int res = mscreen_query(&sdata);
 *
 *     fprintf(stdout, "(%d) ", state);
 *     mscreen_print(stdout, &sdata);
 *     fprintf(stdout, "\n");
 *
 * GFLW:
 *  - https://www.glfw.org/docs/latest/window_guide.html
 *  - https://github.com/glfw/glfw/blob/master/src/internal.h#L530
 */
int mscreen_query(struct mscreen *sdata) {
    if(!sdata) {
        LOG_ERROR("mscreen *sdata is NULL");
        return -1;
    }

    // init
    double mousex, mousey;

    sdata->display = 0; // current display TODO superficial, because zeroed struct, but left for readability
    sdata->mousex = 0; // forcibly ensure zero-ness, we will test for success on this field

    unsigned long start, now;
    start = (unsigned long) time(NULL);

    glfwSetErrorCallback(_error_callback);

    if (!glfwInit()) {
        LOG_ERROR_F("glfwInit() failed with code: '%d'", glfwGetError(NULL));
        return -1;
    }

    // TODO only supporting single (primary) monitor
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        LOG_ERROR_F("glfwGetPrimaryMonitor() failed with code: '%d'", glfwGetError(NULL));
        return -1;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) {
        LOG_ERROR_F("glfwGetVideoMode() failed with code: '%d'", glfwGetError(NULL));
        return -1;
    }

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);

    if (!window) {
        LOG_ERROR_F("glfwCreateWindow() failed with code: '%d'", glfwGetError(NULL));
        return -1;
    }

    while (1) {
        glfwGetCursorPos(window, &mousex, &mousey);
        break;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    sdata->screenw = (int) mode->width;
    sdata->screenh = (int) mode->height;
    sdata->mousex = (int) mousex;
    sdata->mousey = (int) mousey;

    return 0;
}
