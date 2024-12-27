#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFLEN 1024

// Function to receive the file from the client and write it to the disk
void receive_file(int new_sockfd) {
    char buffer[BUFLEN];
    FILE *fp;
    int n;

    fp = fopen("received_file.txt", "wb");
    if (fp == NULL) {
        perror("Error opening file to write");
        exit(1);
    }

    while ((n = recv(new_sockfd, buffer, BUFLEN, 0)) > 0) {
        fwrite(buffer, sizeof(char), n, fp);
        if (n < BUFLEN) {
            break; // File transfer completed
        }
    }
    
    if (n < 0) {
        perror("Error receiving data");
    }
    
    printf("File received successfully\n");
    fclose(fp);
}

int main() {
    int sockfd, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    
    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }
    
    // Listen for incoming connections
    listen(sockfd, 5);
    printf("Server listening on port %d\n", PORT);
    
    client_len = sizeof(client_addr);
    
    // Accept client connection
    new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
    if (new_sockfd < 0) {
        perror("Error accepting connection");
        exit(1);
    }

    printf("Client connected\n");

    // Receive file from the client
    receive_file(new_sockfd);
    
    close(new_sockfd);
    close(sockfd);

    return 0;
}

