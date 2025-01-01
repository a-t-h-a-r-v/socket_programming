#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void perform_calculation(char *input, char *result) {
    double num1, num2;
    char operator;

    // Parse the input (expected format: num1 operator num2)
    sscanf(input, "%lf %c %lf", &num1, &operator, &num2);

    switch (operator) {
        case '+':
            sprintf(result, "%lf", num1 + num2);
            break;
        case '-':
            sprintf(result, "%lf", num1 - num2);
            break;
        case '*':
            sprintf(result, "%lf", num1 * num2);
            break;
        case '/':
            if (num2 != 0) {
                sprintf(result, "%lf", num1 / num2);
            } else {
                strcpy(result, "Error: Division by zero");
            }
            break;
        default:
            strcpy(result, "Error: Invalid operator");
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE], result[BUFFER_SIZE];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connections...\n");

    // Accept client connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    // Read and process client request
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        memset(result, 0, sizeof(result));

        // Receive calculation request from client
        int read_size = recv(client_fd, buffer, sizeof(buffer), 0);
        if (read_size == 0) {
            printf("Client disconnected\n");
            break;
        }
        buffer[read_size] = '\0';

        // Perform calculation
        perform_calculation(buffer, result);

        // Send result back to client
        send(client_fd, result, strlen(result), 0);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
