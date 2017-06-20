#define _BSD_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>


#define DEV 244
#define FILENAME "/dev/configuresc"

typedef struct args {
    uint64_t budget;
    uint64_t period;
} args_t;

int main(int argc, char *argv[]) {

    assert(argc == 3);

    args_t args = {
        .budget = atol(argv[1]),
        .period = atol(argv[2])
    };

    printf("%s: \n", argv[0]);
    printf("Setting budget/period %llu/%llu\n", args.budget, args.period);

    int error = 0;
    int fd = open(FILENAME, O_RDWR);
    if (fd == -1) {
        error = mknod(FILENAME, S_IFCHR, makedev(DEV, 0));
        assert(error == 0);
        fd = open(FILENAME, O_RDWR);
        assert(fd != -1);
    }

    ioctl(fd, _IOWR('a', 0, args), &args);
    close(fd);

    return 0;
}
