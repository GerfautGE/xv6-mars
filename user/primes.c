#include <kernel/stat.h>
#include <kernel/types.h>
#include <user/user.h>

void exec_pipe(int fd) {
    int num;
    read(fd, &num, 4);
    printf("prime %d\n", num);

    int p[2];
    pipe(p);
    int tmp = -1;
    while (1) {
        int n = read(fd, &tmp, 4);
        if (n <= 0) {
            break;
        }
        if (tmp % num != 0) {
            write(p[1], &tmp, 4);
        }
    }
    if (tmp == -1) {
        close(p[1]);
        close(p[0]);
        close(fd);
        return;
    }
    int pid = fork();
    if (pid == 0) {
        close(p[1]);
        close(fd);
        exec_pipe(p[0]);
        close(p[0]);
    } else {
        close(p[1]);
        close(p[0]);
        close(fd);
        wait(0);
    }
}

int main(int argc, char **argv[]) {
    int p[2];
    pipe(p);
    for (int i = 2; i < 35; i++) {
        int n = i;
        write(p[1], &n, 4);
    }
    close(p[1]);
    exec_pipe(p[0]);
    close(p[0]);
}
