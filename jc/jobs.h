// skeleton header

int JC_BACKGROUND;
int JC_COREDUMP;
int JC_EXITED;
int JC_FOREGROUND;
int JC_LEAVERUN;
int JC_REPORT;
int JC_RUNNING;
int JC_STOPPED;

typedef struct PROC {
    int pr_next;
    int pr_name;
    int pr_pid;
    int pr_pgrp;
    int pr_exit;
    int pr_sig;
    int pr_flags;
} PROC ;

PROC *proc_list;

