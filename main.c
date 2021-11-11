#include <stdio.h>
#include "RPMSG.h"

RPMSG_t rpmsg;
char buff[BUFF_SIZE];

int main() {
    printf("Hello, World!\n");

    RPMSG_Start(&rpmsg, "/dev/RPMSG0");

    bool isRun = true;
    while (isRun) {
        if (RPMSG_GetLine(&rpmsg, &buff[0]) > 0) {
            printf("-> %s\n", &buff[0]);
        }
    }

    return 0;
}
