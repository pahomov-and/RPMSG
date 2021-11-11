//
// Created by tymbys on 11.11.2021.
//

#include "RPMSG.h"

int RPMSG_buff_put(buff_t *buff, char c) {
    if ( buff->end != ((buff->start + BUFF_SIZE - 1) % BUFF_SIZE)) {
        buff->buff[buff->end] = c;
        buff->end ++;
        buff->end = buff->end % BUFF_SIZE;

        return 0;
    }

    return -1;
}

bool RPMSG_buff_get(buff_t *buff, char *c) {
    if (c==NULL) return false;

    if (buff->end != buff->start) {
        *c = (buff->buff[buff->start]);
        buff->start ++;
        buff->start = buff->start % BUFF_SIZE;
        return true;
    }

    return false;
}

size_t RPMSG_GetLine(RPMSG_t *rpmsg, char *str) {
    char get = 0;
    size_t count = 0;
    while (RPMSG_buff_get(&rpmsg->rx, &get ) || get != '\r' || get != '\n') {
        str[count++] = get;
    }

    if (count > 0 ) str[count] = 0;

    return count;
}

static void *RPMSG_rx_tx(void *p) {
    uint8_t buff[BUFF_SIZE];
    struct pollfd ufds;
    int res = 0;
    RPMSG_t *rpmsg = (RPMSG_t *)p;

    ufds.fd = rpmsg->fd;
    ufds.events = POLLIN;

    while (rpmsg->state == STATE_RUN) {
        res = poll(&ufds, 1, POLL_TIMEOUT);

        if (res > 0) {
            int count = read(rpmsg->fd, buff, BUFF_SIZE - 1);
            if (count > 0) {
                buff[count] = '\0';
                for (int i = 0; i < count; i++) {
                    RPMSG_buff_put(&rpmsg->rx, buff[i]);
                }

            } else if (count < 0) {
                printf("Error\r\n");
                break;
            }
        } else if (res < 0) {
            printf("Error");
            break;
        }

    }

    rpmsg->state = STATE_STOP;
    close(rpmsg->fd);
}

int RPMSG_Start(RPMSG_t *rpmsg, const char *dev_name) {
    if (rpmsg->state != STATE_RUN) {

        rpmsg->fd = open(dev_name, O_RDWR | O_NOCTTY);

        if (rpmsg->fd < 0) return -1;

        rpmsg->state = STATE_RUN;

        if (pthread_create(&rpmsg->thread, NULL, RPMSG_rx_tx, (void*) rpmsg) != 0) {
            rpmsg->state = STATE_STOP;
            return -1;
        }
        return 0;
    }

    return -1;
}
