#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <liburing.h>

#define PORT 8080
#define BACKLOG 128
#define BUFFER_SIZE 1024
#define RESPONSE "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!"

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct io_uring ring;
    struct io_uring_cqe *cqe;
    struct io_uring_sqe *sqe;
    char buffer[BUFFER_SIZE];

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Initialize io_uring
    if (io_uring_queue_init(32, &ring, 0) < 0) {
        perror("io_uring_queue_init failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Accept connection
        sqe = io_uring_get_sqe(&ring);
        io_uring_prep_accept(sqe, server_fd, (struct sockaddr *)&address, &addrlen, 0);
        io_uring_submit(&ring);
        io_uring_wait_cqe(&ring, &cqe);
        client_fd = cqe->res;
        io_uring_cqe_seen(&ring, cqe);

        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        // Read request
        sqe = io_uring_get_sqe(&ring);
        io_uring_prep_recv(sqe, client_fd, buffer, BUFFER_SIZE, 0);
        io_uring_submit(&ring);
        io_uring_wait_cqe(&ring, &cqe);
        io_uring_cqe_seen(&ring, cqe);

        // Send response
        sqe = io_uring_get_sqe(&ring);
        io_uring_prep_send(sqe, client_fd, RESPONSE, strlen(RESPONSE), 0);
        io_uring_submit(&ring);
        io_uring_wait_cqe(&ring, &cqe);
        io_uring_cqe_seen(&ring, cqe);

        // Close client connection
        close(client_fd);
    }

    // Clean up
    io_uring_queue_exit(&ring);
    close(server_fd);
    return 0;
}