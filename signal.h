#ifndef __SIGNAL_H__
#define __SIGNAL_H__


typedef union sigval {
    int sival_int;
    void *sival_ptr;
} sigval_t;


#define __SIGINFO               \
struct {                        \
    int si_signo;               \
    int si_code;                \
    int si_errno;               \
    union __sifields _sifields; \
}

typedef struct siginfo {
    union{
        __SIGINFO;
        int _si_pad[SI_MAX_SIZE/sizeof(int)];
    };
}siginfo_t;
