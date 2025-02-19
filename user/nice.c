#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  if(argc != 3){
    fprintf(2, "usage: nice pid priority\n");
    exit(1);
  }
  nice(atoi(argv[1]), atoi(argv[2]));
  exit(0);
}
