/**
 * Mousedev event handler API
 * - https://www.kernel.org/doc/html/v5.5/input/input.html#mousedev
 *
 * PS/2 packet format(s)
 * - https://www.eecg.utoronto.ca/~jayar/ece241_08F/AudioVideoCores/ps2/ps2.html
 * - https://wiki.osdev.org/Mouse_Input
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <signal.h>

#include "mlog.h"
#include "mcapt.h"
#include "mscreen.h"

extern FILE *dlog;
extern int fid;

extern int verbose;


void usage() {
    fprintf(stderr,
        "Usage: (sudo) mcapt [OPTION]... [DATALOG]\n"
        "Record mouse movement into a comma-separated data file (DATALOG).\n"
        "       -v, --verbose \t\t enable verbose output\n"
        "       -h, --help \t\t show this help :)\n");
}

int main(int argc, char **argv) {

    char dpath[1024] = { '\0' };
    struct mscreen sdata = { 0 };

    if (argc >= 0) {
        int tick = 0;
        for (int i = 1; i < argc; i++) {
            if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
                usage();
                exit(0);
            }
            if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
                verbose = 1;
                tick = i;
            }
            if (i == argc - 1 && i > tick) { // last pos
                if (argv[i][0] != '-') { // not an option
                    strncpy(dpath, argv[i], 1024);
                }
            }
        }
    }

    if (dpath[0] == '\0') {
        strncpy(dpath, LOG, 1024);
    }

    // get screen data

    if (mscreen_query(&sdata)) {
        return EXIT_FAILURE;
    }

    if (verbose) {
        fprintf(stdout, " - ");
        mscreen_print(stdout, &sdata);
        fprintf(stdout, "\n");
    }

    // open device
    if (input_open(DEVICE)) {
        return EXIT_FAILURE;
    }

    // open log
    if (dlog_open(dpath)) {
        return EXIT_FAILURE;
    }

    if (!verbose) {
        fprintf(stdout, " - mcapt listening to '%s'\n", DEVICE);
        fprintf(stdout, " - mcapt recording to '%s'\n", dpath);
    }

    signal(SIGTERM, signal_handler);
    signal(SIGKILL, signal_handler);
    signal(SIGINT, signal_handler); // keyboard interrrupt (Ctrl+C)
    atexit(exit_handler);

    int err = dlog_listen_mousedev();
    if (err) {
        exit(err); // logging is done inside function
    }

    exit_handler();
}

