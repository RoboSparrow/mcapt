#ifndef __MSCREEN_H__
#define __MSCREEN_H__

#define MSCREEN_MAX_SEC 5 // max query time seconds

struct mscreen {
    int display;

    int screenw;
    int screenh;

    int mousex;
    int mousey;
};

void mscreen_print(FILE *fp, struct mscreen *sdata);
int mscreen_dlog(FILE *fp, struct mscreen *sdata);
int mscreen_query(struct mscreen *sdata);

#endif
