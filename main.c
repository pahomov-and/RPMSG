#include <stdio.h>
#include <stdlib.h>

#include "RPMSG.h"

RPMSG_t rpmsg;
char buff[BUFF_SIZE];
pthread_t thread;
bool isRun;

void *loop(void*p) {

    isRun = true;
    while(RPMSG_Start(&rpmsg, "/dev/RPMSG0") < 0 && isRun) {
        printf("Not open: /dev/RPMSG0\n");
        sleep(1);
    }

    while (isRun) {
        if (RPMSG_GetLine(&rpmsg, &buff[0]) > 0) {
            printf("-> %s\n", &buff[0]);
        }
    }
}

void intHandler(int dummy) {
    isRun = false;
}

int main() {
    int status_addr;
    int status;
    printf("Hello, World!\n");

    signal(SIGINT, intHandler);

    if (pthread_create(&thread, NULL, loop, (void*) NULL) != 0) {
        printf("Error");
        return -1;
    }

    status = pthread_join(thread, (void**)&status_addr);
    if (status != 0) {
        printf("Error: can't join thread, status = %d\n", status);
        exit(-1);
    }

    getchar();
    return 0;
}
