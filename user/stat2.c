#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

// Number of copies for mirrored files.
#define NUM_MIRRORED_COPIES 2

int ppid;
#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

int
main(int argc, char *argv[])
{
  ppid = getpid();
  int fd;
  int n = 1;

  int size = 512;
  int i;
  char buf[size];


  fd = open("file1", O_MIRRORED | O_CREATE | O_RDWR);

  memset(buf, 0, size);

  for (i = 0; i < n; i++) {
     buf[0] = (char)('A' + i);
     write(fd, buf, size);
  }

  struct stat st;
  fstat(fd, &st);
  
  printf(1, "Type: %d\n", st.type);
  assert(st.type == 4);

  printf(1, "logical_size: %d\n", st.logical_size);
  assert(st.logical_size == 512);

  printf(1, "physical_size: %d\n", st.physical_size);
  //Works for both regular and extra credit
  assert(st.physical_size == 512*2 || st.physical_size == 512*3);
  
  close(fd);

  printf(1, "TEST PASSED\n");
  
  exit();
}
