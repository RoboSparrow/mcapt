#ifndef __MCAPT_H__
#define __MCAPT_H__

#define DEVICE "/dev/input/mice"
#define LOG "./data/dlog.csv"

// mousedev stream packet size
// - https://wiki.osdev.org/Mouse_Input
// - https://www.kernel.org/doc/html/v5.5/input/input.html#mousedev
#define PS2 3 // PS/2 protocol, default: [x,y,lbtn|mbtn|rbtn] 3 bytes width
#define IMPS2 4 // IMPS/2 protoclo: additional 4th byte for mousewheel (TODO, curr unsupported)
#define MPACKET PS2

#define MICROSEC(TV) (TV.tv_sec * 1000000) + TV.tv_usec

extern FILE *dlog;
extern int fid;

void signal_handler(int signo);
void exit_handler();

int dlog_open(char *path);
void dlog_close();

int input_open(char *device);
void input_close();

int dlog_listen_mousedev();

#endif
