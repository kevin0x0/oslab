#include <asm/segment.h>
#include <string.h>
#include <errno.h>

#define WHO_NAMELEN (23)

/* initialize it with a NULL string */
static char name_buf[WHO_NAMELEN + 1] = "";


int sys_iam(const char* name) {
  char tmp_buf[WHO_NAMELEN + 1];
  char ch = 0;
  int len = 0;
  while (len <= WHO_NAMELEN && (ch = get_fs_byte(name + len)) != '\0') {
    tmp_buf[len++] = ch;
  }

  if (len > WHO_NAMELEN)  /* name too long */
    return -EINVAL;

  tmp_buf[len] = '\0';
  strcpy(name_buf, tmp_buf);
  return len;
}

int sys_whoami(char* name, unsigned int size) {
  unsigned int len = 0;
  while (len < size && name_buf[len] != '\0') {
    put_fs_byte(name_buf[len], name + len);
    ++len;
  }

  if (len >= size)  /* buffer overflow */
    return -EINVAL;

  put_fs_byte('\0', name + len);
  return len;
}
