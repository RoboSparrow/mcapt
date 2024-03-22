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

#include "mcapt.h"

extern FILE *dlog;
extern int fid;

extern int verbose;


void usage() {
    fprintf(stderr,
        "Record mouse movement into a log.\n"
        "Usage: (sudo) mcapt [OPTION]...\n"
        "       -v, --verbose \t\t enable verbose output\n"
        "       -h, --help \t\t show this help :)\n");
}

int main(int argc, char **argv) {

    if (argc >= 0) {
      for (int i = 1; i < argc; i++) {
          if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
              verbose = 1;
          }
          if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
              usage();
              exit(0);
          }
      }
    }

    // open device
    if (input_open(DEVICE)) {
        return EXIT_FAILURE;
    }

    // open log
    if (dlog_open(LOG)) {
        return EXIT_FAILURE;
    }

    fprintf(stdout, " - mcapt listening to '%s'\n", DEVICE);
    fprintf(stdout, " - mcapt recording to '%s'\n", LOG);

    signal(SIGTERM,  signal_handler);
    signal(SIGKILL,  signal_handler);
    signal(SIGINT,  signal_handler); // keyboard interrrupt (Ctrl+C)
    atexit(exit_handler);

    int err = dlog_listen_mousedev();
    if (err) {
        exit(err); // logging is done inside function
    }

    exit_handler();
}

