#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFLEN 1024

// Function to send the file to the server
void send_file(FILE *fp, int sockfd) {
    char buffer[BUFLEN];
    size_t n;

    while ((n = fread(buffer, sizeof(char), BUFLEN, fp)) > 0) {
        if (send(sockfd, buffer, n, 0) == -1) {
            perror("Error sending file");
            exit(1);
        }
    }
    
    printf("File sent successfully\n");
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "file_to_send.txt";  // File to send

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Change to server IP if necessary

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Open the file to send
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Send the file to the server
    send_file(fp, sockfd);

    fclose(fp);
    close(sockfd);

    return 0;
}

