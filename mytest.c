#include "libmini.h"

typedef void (*proc_t)();
static jmp_buf jb;


#define FUNBODY(m, from)    \
    {                       \
    write(1, m, strlen(m)); \
    sigset_t tmp;           \
    sigemptyset(&tmp);      \
    sigaddset(&tmp, from);  \
    sigprocmask(SIG_BLOCK, &tmp, NULL);     \
    sigprocmask(SIG_SETMASK, NULL, &tmp);   \
    write(1, "\nBefore LONGJMP\n", 16);     \
    sigtest(&tmp);          \
    longjmp(jb, from);      \
    }

void a() FUNBODY("This is function a().\n", 1);
void b() FUNBODY("This is function b().\n", 2);
void c() FUNBODY("This is function c().\n", 3);
void d() FUNBODY("This is function d().\n", 4);
void e() FUNBODY("This is function e().\n", 5);
void f() FUNBODY("This is function f().\n", 6);
void g() FUNBODY("This is function g().\n", 7);
void h() FUNBODY("This is function h().\n", 8);
void i() FUNBODY("This is function i().\n", 9);
void j() FUNBODY("This is function j().\n", 0);

proc_t funs[] = { a, b, c, d, e, f, g, h, i, j };

int main()
{
    sigset_t new;
    sigset_t old;

    sigemptyset(&new);
    sigfillset(&new);
    write(1, "After Fill\n", 11);
    sigtest(&new);

    for(int i = 16; i < 30; i++)
    {
        sigdelset(&new, i);
    }
    for(int i = 16; i < 30; i++)
    {
        sigdelset(&new, i);
    }
    write(1, "\nAfter DEL\n", 11);
    sigtest(&new);



    sigemptyset(&new);
    write(1, "\nAfter EMT\n", 11);
    sigtest(&new);

    for(int i = 10; i < 21; i++){
        sigaddset(&new, i);
    }
    write(1, "\ntest add new\n", 14);
    sigtest(&new);


    sigprocmask(SIG_SETMASK, &new, NULL);
    sigprocmask(SIG_SETMASK, NULL, &old);
    write(1, "\ntest old\n", 10);
    sigtest(&old);

    sigset_t mask;
    sigemptyset(&mask);
    for(int i = 21; i < 30; i = i + 2)
    {
        sigaddset(&mask, i);
    }
    write(1, "\nMASK\n", 6);
    sigtest(&mask);
    sigprocmask(SIG_BLOCK, &mask, &old);
    write(1, "\nOLD\n", 5);
    sigtest(&old);
    sigprocmask(SIG_SETMASK, NULL, &old);
    write(1, "\nNOW\n", 5);
    sigtest(&old);


    sigprocmask(SIG_UNBLOCK, &new, NULL);
    sigprocmask(SIG_SETMASK, NULL, &old);
    write(1, "\nUNBLOCK\n", 9);
    sigtest(&old);

    volatile int j = 0;
    if(setjmp(jb) != 0){
        j++;
        write(1, "\nLONGJMP\n", 9);
        sigprocmask(SIG_SETMASK, NULL, &old);
        sigtest(&old);
    }else{
        write(1, "\nSETJMP\n", 8);
        sigprocmask(SIG_SETMASK, NULL, &old);
        sigtest(&old);
        write(1, "\nJBMASK\n", 8);
        sigtest(&jb->mask);
    }
    if(j < 10) funs[j]();
    return 0;

}
