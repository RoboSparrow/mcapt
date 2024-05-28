#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "mlog.h"
#include "mcapt.h"


FILE *dlog = NULL;
int fid = -1;

int verbose = 0;


int dlog_open(char *dpath) {
    if (!dpath) {
        LOG_ERROR("Could not open log (no dpath)");
        return 0;
    }

    dlog = fopen(dpath, "w");
    if (!dlog) {
        LOG_ERROR_F("Could not open log '%s'! error: (%d), '%s'", dpath, errno, strerror(errno));
        return 0;
    }

    // write header
    fprintf(dlog, "; time=%ld device=%s\n", time(NULL), dpath);

    LOG_INFO_F("Opened dlog: '%s'", dpath);
    return 0;
}

void dlog_close() {
    if (dlog)  {
        fclose(dlog);
    }
    dlog = NULL;
}

int input_open(char *device) {
    if (!device) {
        LOG_ERROR("Could not open log (no input device)");
        return 1;
    }

    fid = open(device, O_RDONLY);
    if (fid < 0) {
        LOG_ERROR_F("Could not open %s input device! (fid = %d), error: (%d), '%s'", device, fid, errno, strerror(errno));
        return EXIT_FAILURE;
    }
    LOG_INFO_F("Opened %s device!", device);// TODO close onexit

    return 0;
}

void input_close() {
    if (fid >= 0)  {
        close(fid);
    }
    fid = -1;
}

void exit_handler() {
    LOG_INFO("exiting program");
    input_close();
    dlog_close();
}

int dlog_listen_mousedev() {

    int bytes;
    unsigned char data[MPACKET];

    int lb, mb, rb;   // mouse buttons
    unsigned long ts; // timestamp
    signed char rx, ry; // mouse REL_X, REL_Y

    struct timeval tv;
    gettimeofday(&tv, NULL);

    int st = MICROSEC(tv);
    int el = 0;

    while (1) {

        if (fid < 0) {
            continue; // exiting...
        }

        bytes = read(fid, data, sizeof(data));
        if (bytes <= 0) {
            LOG_ERROR_F("Error reading events! (fid = %d), error: (%d), '%s'", bytes, errno, strerror(errno));
            return EXIT_FAILURE;
        }

        lb = data[0] & 0x1;
        rb = data[0] & 0x2;
        mb = data[0] & 0x4;

        rx = data[1];
        ry = data[2];

        // wh = data[3]; // mousewheel or extrabuttons

        gettimeofday(&tv, NULL);
        el = MICROSEC(tv) - st;

        fprintf(dlog, "%d,%d,%d,%d,%d,%d\n", el, rx, ry, lb, rb, mb);

        if (verbose) {
            fprintf(stdout, "(%d) rx=%d, ry=%d, lb=%d, mb=%d, rb=%d\n", el, rx, ry, lb, mb, rb);
        }

    } // /while

    return 0;
}
