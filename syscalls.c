// taken from https://sourceware.org/newlib/libc.html#Syscalls
#define STM32F051
#include <stdint.h>
#include <stm32f0xx.h>
#include <stdlib.h>

#include <sys/stat.h> // required for fstat

int _close(int file) {
  return -1;
}

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) {
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}

int _read(int file, char *buf, int len) {
  int i = 0;
  while(i < 1) {
    while((USART1->ISR & USART_ISR_RXNE) == 0); // hange while receive IS empty
    buf[i++] = USART1->RDR;
  }
  return len;
}


int _write(int file, const char *ptr, int len) {
  int todo;

  for (todo = 0; todo < len; todo++) {
    while((USART1->ISR & USART_ISR_TXE) == 0); // hang while trasnmit not empty
    USART1->TDR = (*ptr++);
  }
  return len;
}

void *_sbrk(int incr) {
  extern char _ebss;           /* Defined by the linker */
  extern char _heap_end;
  extern char _heap_start;
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0) {
    heap_end = &_heap_start;
  }
  prev_heap_end = heap_end;
  if (heap_end + incr >= &_heap_end) {
    _write (1, "Heap and stack collision\n", 25);
    for(;;);
  }

  heap_end += incr;
  return (void *) prev_heap_end;
}
