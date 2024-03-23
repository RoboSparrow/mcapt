#ifndef __MLOG_H__
#define __MLOG_H__

extern int verbose;

#define LOG_ERROR_F(format, ...) do { fprintf(stderr, "[error](%s:%d) " format "\n", __FILE__, __LINE__, __VA_ARGS__); } while(0)
#define LOG_INFO_F(format, ...) do { if (verbose) { fprintf(stdout, "[info](%s:%d) " format "\n", __FILE__, __LINE__, __VA_ARGS__); } } while(0)

#define LOG_ERROR(format) do { fprintf(stderr, "[error](%s:%d) " format "\n", __FILE__, __LINE__); } while(0)
#define LOG_INFO(format) do { if (verbose) { fprintf(stdout, "[info](%s:%d) " format "\n", __FILE__, __LINE__); } } while(0)


#endif
