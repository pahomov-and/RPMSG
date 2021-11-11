//
// Created by tymbys on 11.11.2021.
//

#ifndef THERMO_CONTROLLER_RPMSG_H
#define THERMO_CONTROLLER_RPMSG_H

#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <stdbool.h>
#include <sys/types.h>


#define BUFF_SIZE 255
#define POLL_TIMEOUT 2000

enum STATES {
    STATE_STOP,
    STATE_RUN
};

typedef struct {
    char buff[BUFF_SIZE];
    int  start, end;
} buff_t;

typedef struct {
    int fd;
    enum STATES state;

    buff_t rx;
    buff_t tx;

    pthread_t thread;

} RPMSG_t;

int RPMSG_buff_put(buff_t *buff, char c);
bool RPMSG_buff_get(buff_t *buff, char *c);

size_t RPMSG_GetLine(RPMSG_t *rpmsg, char *str);

int RPMSG_Start(RPMSG_t *rpmsg, const char *dev_name);
size_t RPMSG_Receive(RPMSG_t *rpmsg);
int RPMSG_Transmit(RPMSG_t *rpmsg, ...);

#endif //THERMO_CONTROLLER_RPMSG_H
