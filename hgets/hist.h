// Re-created hist.h header.

typedef struct HQUEUE {
    int hq_count;
    int hq_line;
    int hq_head;
} HQUEUE;
typedef struct HIST {
    int hi_next;
    int hi_argc;
    int hi_argv;
} HIST;

int h_errno;
// errno's:
int H_NOERROR;
int H_EMPTY;
int H_EOF;

