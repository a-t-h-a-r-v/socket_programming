#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE], result[BUFFER_SIZE];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Get input from the user
    while (1) {
        printf("Enter calculation (e.g., 5 + 3) or 'exit' to quit: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Exit if user types 'exit'
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }

        // Send calculation request to server
        send(sock, buffer, strlen(buffer), 0);

        // Receive result from server
        memset(result, 0, sizeof(result));
        recv(sock, result, sizeof(result), 0);

        printf("Result: %s\n", result);
    }

    close(sock);

    return 0;
}

