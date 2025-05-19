/* include <stdint.h> */

#define TmpDirPath "/tmp/ct.XXXXXX"

#define MAX_TMPPATH_SIZE 14


typedef int64_t int64;
typedef struct Test Test;
typedef struct Benchmark Benchmark;

struct Test {
    void (*f)(void);
    const char *name;
    int  status;
    int  fd;
    int  pid;
    char dir[MAX_TMPPATH_SIZE];
};

struct Benchmark {
    void  (*f)(int);
    const char  *name;
    int   status;
    int64 dur;
    int64 bytes;
    char  dir[MAX_TMPPATH_SIZE];
};

extern Test ctmaintest[];
extern Benchmark ctmainbench[];
