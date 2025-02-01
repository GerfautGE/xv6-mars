#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    if (argc != 1) {
        fprintf(2, "Usage: reboot\n");
        exit(1);
    }

    reboot();
    exit(0);
}
