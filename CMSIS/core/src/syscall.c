#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

// Heap management (for malloc)
extern char _end; // Defined by the linker (end of .bss)
static char *heap_end = &_end;

int _close(int fd) {
  (void)fd;
  errno = EBADF;
  return -1;
}

int _getpid(void) { return 1; }

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

void _exit(int status) {
  (void)status;
  while (1) {
  } // Infinite loop
}

void *_sbrk(int incr) {
  char *prev_heap_end = heap_end;
  heap_end += incr;
  return (void *)prev_heap_end;
}

// File status (for printf)
int _fstat(int fd, struct stat *st) {
  st->st_mode = S_IFCHR; // Pretend it's a terminal
  return 0;
}

// Terminal check (for printf)
int _isatty(int fd) {
  return 1; // Return "1" to indicate a terminal
}

// Minimal write (for printf over UART)
int _write(int fd, char *buf, int len) {
  // Implement UART transmit here (e.g., HAL_UART_Transmit)
  for (int i = 0; i < len; i++) {
    // Send buf[i] via UART
  }
  return len;
}
