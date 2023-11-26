#include <stdio.h>
#include <stdlib.h>

#define __LIBRARY__
#include <unistd.h>

_syscall1(int, iam, const char*, name)
_syscall2(int, whoami, char*, name, unsigned int, size)

int main(int argc, char *argv[]) {
  char myname[24];
  whoami(myname, sizeof (myname));
  printf("%s", myname);
  return 0;
}
