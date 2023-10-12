#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define PORT 8080

int s_socket;

int main(int argc, char *argv[]) {
    int port = PORT;
    int sock;

    struct sockaddr_in socket_addr = {// server address
                                      .sin_family = AF_INET,
                                      .sin_addr.s_addr = htonl(INADDR_ANY),
                                      .sin_port = htons(port)};

    // Creating listening socket
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("[-] Couldn't create a socket.");
        exit(1);
    }

    // Binding the socket
    if ((bind(sock, (struct sockaddr *)&socket_addr, (socklen_t)sizeof(socket_addr))) < 0) {
        printf("[-] Couldn't bind the socket.");
        exit(1);
    }

    printf("[+] Created and binded socket.\n");

    if ((listen(sock, 16)) < 0) {
        printf("[-] Couldn't open socket for listening connections");
        exit(1);
    }

    struct sockaddr_in cl_addrr;
    socklen_t cl_addrr_length;

    printf("... Waiting for connections at %s:%d...\n", inet_ntoa(socket_addr.sin_addr), ntohs(socket_addr.sin_port));
    
    while (1) {
        int r_socket = -1;
        if ((r_socket = accept(sock, (struct sockaddr *)&cl_addrr, &cl_addrr_length)) < 0) {
            printf("[-] Couldn't accept connection from client.");
            exit(1);
        }

        printf("[+] Connected to %s\n", inet_ntoa(cl_addrr.sin_addr));

        char *buffer = calloc(BUFFER_SIZE, sizeof(char));

        if (recv(r_socket, buffer, BUFFER_SIZE, 0) < 0) {
            printf("[-] Couldn't receive filename from client.\n");
            break;
        }

        char *file_to_send = calloc(strlen(buffer) + 1, sizeof(char));
        strncpy(file_to_send, buffer, strlen(buffer));
        FILE *fd;

        if ((fd = fopen(file_to_send, "rb")) == NULL) {
            printf("[-] Couldn't open file to read.\n");
            break;
        } else {
            size_t b_read;
            ssize_t b_sent;
            while (!feof(fd)) {
                b_read = fread(buffer, sizeof(char), BUFFER_SIZE, fd);

                if (b_read > 0 && (b_sent = send(r_socket, buffer, b_read, 0)) < 0) {
                    printf("[-] Couldn't send file to client.\n");
                    break;
                }
            }
            printf("[+] File %s sent to %s.\n", file_to_send, inet_ntoa(cl_addrr.sin_addr));
        }

        fclose(fd);
        close(r_socket);
        free(buffer);
    }
}