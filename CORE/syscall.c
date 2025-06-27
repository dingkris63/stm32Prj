#include <errno.h>
#include <sys/unistd.h>

int _close(int fd) {
    (void)fd;
    errno = EBADF;
    return -1;
}

int _getpid(void) {
    return 1;
}

int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _lseek(int fd, int ptr, int dir) {
    (void)fd;
    (void)ptr;
    (void)dir;
    errno = EBADF;
    return -1;
}

int _read(int fd, char *ptr, int len) {
    (void)fd;
    (void)ptr;
    (void)len;
    errno = EBADF;
    return -1;
}

int _write(int fd, char *ptr, int len) {
    (void)fd;
    // Implement basic UART output here if needed
    // Otherwise return success but do nothing
    return len;
}

void _exit(int status) {
    (void)status;
    while(1) {} // Infinite loop
}