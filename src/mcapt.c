#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

#include <time.h>
#include <string.h>

// To make sure there will be no header conflicts, you can define GLFW_INCLUDE_NONE before the GLFW header
//   to explicitly disable inclusion of the development environment header.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "mlog.h"
#include "mcapt.h"

FILE *dlog = NULL;
int verbose = 0;


int dlog_open(char *dpath) {
    if (!dpath) {
        LOG_ERROR("Could not open log (no dpath)");
        return 1;
    }

    dlog = fopen(dpath, "w");
    if (!dlog) {
        LOG_ERROR_F("Could not open log '%s'! error: (%d), '%s'", dpath, errno, strerror(errno));
        return 1;
    }

    LOG_INFO_F("Opened dlog: '%s'", dpath);
    return 0;
}

int dlog_write(char *format, ...) {
    if (!dlog) {
        LOG_ERROR("No dlog");
        return 1;
    }

    va_list args;
    va_start(args, format);
    vfprintf(dlog, format, args);
    va_end (args);
    return 0;
}

void dlog_close() {
    if (dlog)  {
        fclose(dlog);
    }
    dlog = NULL;
}

void mscreen_print(FILE *fp, struct mscreen *sdata) {
    if(!fp || !sdata) {
        return;
    }
    fprintf(
        fp,
        "{ primary: %d, monitor: %s, width: %f, height: %f, mousex: %f, mousey: %f }",
        sdata->primary,
        sdata->monitor,
        sdata->width,
        sdata->height,
        sdata->mousex,
        sdata->mousey
    );
}


static void _error_callback(int err, const char* reason) {
    LOG_ERROR_F("GFLW3 Error %d: %s\n", err, reason);
}

/**
 * @see https://www.glfw.org/docs/latest/input_guide.html#cursor_pos
 */
static void _cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    double now = glfwGetTime();
    dlog_write("%f,%f,%f\n", now, xpos, ypos); // TODO err

    if (verbose) {
        fprintf(stdout," - %f,%f\n", xpos, ypos);
    }
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
 */
int mscreen_query(struct mscreen *sdata) {
    if(!sdata) {
        LOG_ERROR("mscreen *sdata is NULL");
        return -1;
    }

    // init

    double mousex;
    double mousey;
    double now;

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

    // The returned string is allocated and freed by GLFW. You should not free it yourself.
    const char *mname = glfwGetMonitorName(monitor);
    if (!mname) {
        LOG_ERROR_F("glfwGetMonitorName() failed with code: '%d'", glfwGetError(NULL));
        return -1;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) {
        LOG_ERROR_F("glfwGetVideoMode() failed with code: '%d'", glfwGetError(NULL));
        return -1;
    }

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);

    // Top and transparent
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    // glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE); // TODO ‘GLFW_MOUSE_PASSTHROUGH’ undeclared, requires glfw >= 3.4

    glfwSwapInterval(1);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Overlay", monitor, NULL);

    if (!window) {
        LOG_ERROR_F("glfwCreateWindow() failed with code: '%d'", glfwGetError(NULL));
        return -1;
    }

    // set up imput event callbacks

    glfwSetCursorPosCallback(window, _cursor_position_callback);

    // fill sdata with already known values

    sdata->primary = 1; // TODO only supporting single (primary) monitor
    strncpy(sdata->monitor, mname, 128);
    sdata->width = mode->width;
    sdata->height = mode->height;

    glfwGetCursorPos(window, &mousex, &mousey);// TODO direct
    sdata->mousex = mousex;
    sdata->mousey = mousey;

    // Initialize timer
    now = glfwGetTime() - 0.01;

    dlog_write("; time=%f, monitor=%s, width=%f, height=%f, mousex=%f, mousey=%f\n", now, sdata->monitor, sdata->width, sdata->height, sdata->mousex, sdata->mousey);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void exit_handler() {
    LOG_INFO("exiting program");
    dlog_close();
}
