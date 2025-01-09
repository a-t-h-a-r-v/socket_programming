#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock = 0;
    char buffer[BUF_SIZE];
    struct sockaddr_in servaddr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        send(sock, buffer, strlen(buffer), 0);
        int n = read(sock, buffer, BUF_SIZE);
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(sock);
    return 0;
}
