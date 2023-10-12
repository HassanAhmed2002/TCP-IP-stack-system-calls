#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define PORT 8080

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <remote_filename> <l_filename>\n", argv[0]);
        exit(1);
    }
    int port = PORT;
    char *remote_filename = argv[1];
    char *l_filename = argv[2];

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);

    printf("... Trying to connect to the server at %s:%d ...\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("[-] Unable to create a socket.");
        exit(1);
    }

    if ((connect(sock, (struct sockaddr *)&server_addr, (socklen_t)sizeof(server_addr))) < 0) {
        printf("[-] Couldn't connect to server.");
        exit(1);
    }

    printf("[+] Connected to server.\n");

    printf("[+] Sent the filename: %s to server.\n", remote_filename);
    send(sock, remote_filename, strlen(remote_filename), 0);

    FILE *f;

    if ((f = fopen(l_filename, "wb")) == NULL) {
        printf("[-] Couldn't open file %s for writing.\n", l_filename);
        exit(1);
    }

    ssize_t b_received;
    size_t b_written_total = 0;
    char *buffer = calloc(BUFFER_SIZE, sizeof(char));
    while ((b_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        b_written_total += fwrite(buffer, sizeof(char), b_received, f);
        printf("[+] Written %ld bytes to %s.\r", b_written_total, l_filename);
    }

    if (b_written_total == 0) {
        printf("[+] Faliure to receive file.\n");
        exit(1);
    } else {
        printf("\n[+] Completely received the file\n");
    }

    fclose(f);
    free(buffer);

    close(sock);
}