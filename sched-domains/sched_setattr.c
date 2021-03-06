#include <sched.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <inttypes.h>
#include <sched.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct sched_attr {
        u_int32_t size;

        u_int32_t sched_policy;
        u_int64_t sched_flags;

        /* SCHED_NORMAL, SCHED_BATCH */
        int32_t sched_nice;

        /* SCHED_FIFO, SCHED_RR */
        u_int32_t sched_priority;

        /* SCHED_DEADLINE */
        u_int64_t sched_runtime;
        u_int64_t sched_deadline;
        u_int64_t sched_period;
};

#ifndef __NR_sched_setattr
#define __NR_sched_setattr		380
#endif

#ifndef __NR_sched_getattr
#define __NR_sched_getattr		381
#endif

#ifndef SCHED_DEADLINE
#define SCHED_DEADLINE          6
#endif

        static int
sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
        return syscall(__NR_sched_setattr, pid, attr, flags);
}

__attribute__ ((unused))
        static int
sched_getattr(pid_t pid, const struct sched_attr *attr, unsigned int size, unsigned int flags)
{
        return syscall(__NR_sched_getattr, pid, attr, flags);
}

int main(int argc, char *argv[])
{
        int ret = 0;

        if (argc != 4) {
                printf("usage: sched_dead_test <pid> <deadline> <runtime>\n");
                return 0;
        }

        struct sched_attr *attr = malloc(sizeof(struct sched_attr));

        memset(attr, 0, sizeof(struct sched_attr));

        pid_t pid = (pid_t)(atoi(argv[1]));

        printf("running sched_dead_test for %d\n", pid);

        attr->size = sizeof(struct sched_attr);
        attr->sched_policy = SCHED_DEADLINE;
        attr->sched_deadline = atol(argv[2]);
        attr->sched_period = atol(argv[2]);
        attr->sched_runtime = atol(argv[3]);

        ret = sched_setattr(pid, attr, 0);

        if (ret){
                printf("ret=%d for sched_setattr (%s)\n", ret, strerror(errno));
                return ret;
        }

        printf("\targs: %d,%p,%u\n", pid, attr, 0);

        ret = 0;

        free(attr);

        return 0;
}
