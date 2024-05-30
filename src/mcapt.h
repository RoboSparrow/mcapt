#ifndef __MCAPT_H__
#define __MCAPT_H__

extern FILE *dlog;

void exit_handler();

int dlog_open(char *path);
int dlog_write(char *format, ...);
void dlog_close();

/**
 * GFLW:
 * - https://www.glfw.org/docs/latest/window_guide.html
 *
 * - struct _GLFWwindow: https://github.com/glfw/glfw/blob/master/src/internal.h#L530
 * - struct _GLFWmonitor: https://github.com/glfw/glfw/blob/master/src/internal.h#L592
 * - struct _GLFWcontext: https://github.com/glfw/glfw/blob/master/src/internal.h#L489
 */
struct mscreen {
    int primary;
    char monitor[128];

    double width;
    double height;

    double mousex;
    double mousey;
};

void mscreen_print(FILE *fp, struct mscreen *sdata);
int mscreen_query(struct mscreen *sdata);

#endif
