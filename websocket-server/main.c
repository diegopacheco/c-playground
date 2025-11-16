#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define WS_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

char* base64_encode(const unsigned char *input, int length) {
    BIO *bmem, *b64;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    char *buff = malloc(bptr->length + 1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = 0;

    BIO_free_all(b64);

    return buff;
}

char* extract_key(const char *request) {
    char *key_start = strstr(request, "Sec-WebSocket-Key: ");
    if (!key_start) return NULL;

    key_start += 19;
    char *key_end = strstr(key_start, "\r\n");
    if (!key_end) return NULL;

    int key_len = key_end - key_start;
    char *key = malloc(key_len + 1);
    strncpy(key, key_start, key_len);
    key[key_len] = '\0';

    return key;
}

char* create_accept_key(const char *client_key) {
    char combined[256];
    sprintf(combined, "%s%s", client_key, WS_GUID);

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)combined, strlen(combined), hash);

    return base64_encode(hash, SHA_DIGEST_LENGTH);
}

void send_handshake_response(int client_socket, const char *accept_key) {
    char response[512];
    sprintf(response,
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: %s\r\n\r\n",
        accept_key);

    send(client_socket, response, strlen(response), 0);
}

void decode_frame(unsigned char *data, size_t length) {
    if (length < 2) return;

    int fin = (data[0] & 0x80) >> 7;
    int opcode = data[0] & 0x0F;
    int masked = (data[1] & 0x80) >> 7;
    unsigned long long payload_len = data[1] & 0x7F;

    int mask_offset = 2;

    if (payload_len == 126) {
        payload_len = (data[2] << 8) | data[3];
        mask_offset = 4;
    } else if (payload_len == 127) {
        payload_len = 0;
        for (int i = 0; i < 8; i++) {
            payload_len = (payload_len << 8) | data[2 + i];
        }
        mask_offset = 10;
    }

    unsigned char *mask = &data[mask_offset];
    unsigned char *payload = &data[mask_offset + 4];

    if (masked) {
        for (unsigned long long i = 0; i < payload_len; i++) {
            payload[i] = payload[i] ^ mask[i % 4];
        }
    }

    printf("Received message (opcode %d): %.*s\n", opcode, (int)payload_len, payload);
}

void send_text_frame(int client_socket, const char *message) {
    size_t msg_len = strlen(message);
    unsigned char frame[10 + msg_len];
    int frame_size = 0;

    frame[0] = 0x81;

    if (msg_len <= 125) {
        frame[1] = msg_len;
        frame_size = 2;
    } else if (msg_len <= 65535) {
        frame[1] = 126;
        frame[2] = (msg_len >> 8) & 0xFF;
        frame[3] = msg_len & 0xFF;
        frame_size = 4;
    }

    memcpy(&frame[frame_size], message, msg_len);
    frame_size += msg_len;

    send(client_socket, frame, frame_size, 0);
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

    if (bytes_read <= 0) {
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';

    if (strstr(buffer, "Upgrade: websocket")) {
        printf("WebSocket handshake request received\n");

        char *client_key = extract_key(buffer);
        if (!client_key) {
            printf("Invalid WebSocket key\n");
            close(client_socket);
            return;
        }

        char *accept_key = create_accept_key(client_key);
        send_handshake_response(client_socket, accept_key);

        printf("WebSocket handshake completed\n");
        printf("Client key: %s\n", client_key);
        printf("Accept key: %s\n", accept_key);

        free(client_key);
        free(accept_key);

        send_text_frame(client_socket, "Welcome to WebSocket server!");

        while (1) {
            bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_read <= 0) break;

            decode_frame((unsigned char*)buffer, bytes_read);
            send_text_frame(client_socket, "Echo from server");
        }
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        return 1;
    }

    printf("WebSocket server listening on port %d\n", PORT);
    printf("Connect using: ws://localhost:%d\n", PORT);
    printf("Test with: wscat -c ws://localhost:%d\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("\nClient connected from %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        handle_client(client_socket);
        printf("Client disconnected\n");
    }

    close(server_socket);
    return 0;
}
