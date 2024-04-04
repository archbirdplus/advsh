// Re-created hist.h header.

typedef struct hist HIST;

struct hist {
    HIST *hi_next;
    int hi_argc;
    char *hi_argv;
};

typedef struct hqueue {
    int hq_count;
    int hq_line;
    HIST *hq_head;
} HQUEUE;

int h_errno;

#define H_NOERROR 0
#define H_EMPTY 1
#define H_EOF 2

