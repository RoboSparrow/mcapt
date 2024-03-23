#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

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

    int open = 1;
    sdata->display = 0; // current display TODO superficial, because zeroed struct, but left for readability
    sdata->mousex = 0; // forcibly ensure zero-ness, we will test for success on this field

    unsigned long start, now;
    start = (unsigned long) time(NULL);

    SDL_Window *window;
    SDL_Event event;
    SDL_DisplayMode mode = { .w = 0, .h = 0 };

    if (SDL_Init(SDL_INIT_VIDEO) ) {
        LOG_ERROR_F("SDL_Init() failed: '%s'", SDL_GetError());
        return -1;
    }

    // create window

    Uint32 flags = SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED| SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("mcapt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, flags);
    if(!window) {
        LOG_ERROR_F("SDL_CreateWindow() failed: '%s'", SDL_GetError());
        return -1;
    }

    // read data

    while (open > 0) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                open = 0;
                break;
        }

        SDL_GetGlobalMouseState(&(sdata->mousex), &(sdata->mousey));
        // printf(" - mx %d, my: %d\n", sdata->mousex, sdata->mousey);

        if (SDL_GetDesktopDisplayMode(sdata->display, &mode)) {
            LOG_ERROR_F("SDL_GetDesktopDisplayMode() failed: '%s'", SDL_GetError());
            open = -1;
        }

        // success

        if (sdata->mousex >= 0 && mode.w > 0) {
            sdata->screenw = mode.w;
            sdata->screenh = mode.h;
            open = 0;
        }

        // timeout

        now = (unsigned long) time(NULL);
        if (now - start > MSCREEN_MAX_SEC) {
            LOG_ERROR_F("No results after %d seconds, closing testapp", MSCREEN_MAX_SEC);
            open = -1;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return open;
}

