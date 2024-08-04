#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 10

int main() {
    int epoll_fd, nfds;
    struct epoll_event ev, events[MAX_EVENTS];

    // Create an epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Add stdin (file descriptor 0) to the epoll instance
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
        perror("epoll_ctl: stdin");
        exit(EXIT_FAILURE);
    }

    printf("Listening for input on stdin (press Ctrl+D to exit)...\n");
    while (1) {
        // Wait for events
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        // Handle events
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == STDIN_FILENO) {
                char buf[256];
                ssize_t count = read(STDIN_FILENO, buf, sizeof(buf));
                if (count == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                } else if (count == 0) {
                    printf("EOF on stdin, exiting...\n");
                    exit(EXIT_SUCCESS);
                }
                buf[count] = '\0';
                printf("Read from stdin: %s", buf);
            }
        }
    }

    close(epoll_fd);
    return 0;
}