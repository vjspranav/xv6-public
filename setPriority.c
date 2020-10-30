#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int pid=atoi(argv[2]);
  int newPriority=atoi(argv[1]);
  set_priority(newPriority, pid);
  exit();
}
