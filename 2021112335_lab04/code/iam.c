#include <stdio.h>
#include <stdlib.h>

#define __LIBRARY__
#include <unistd.h>

_syscall1(int, iam, const char*, name);
_syscall2(int, whoami, char*, name, unsigned int, size);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "you must specify a name!\n");
    exit(1);
  }

  return iam(argv[1]);
}
