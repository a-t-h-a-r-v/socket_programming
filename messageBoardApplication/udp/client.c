#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len = sizeof(servaddr);

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, len);
        int n = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
