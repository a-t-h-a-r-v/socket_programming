#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFLEN 256
#define MAX 80

void communicate_with_server(int sockfd) {
    char buff[MAX];
    for (;;) {
        memset(buff, 0, sizeof(buff));
        printf("Enter the string: ");
        
        // Use fgets for safer input handling
        if (fgets(buff, sizeof(buff), stdin) == NULL) {
            perror("Input error");
            break;
        }

        // Check for "exit" command
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        // Send message to server
        if (write(sockfd, buff, strlen(buff)) < 0) {
            perror("Write to server failed");
            break;
        }

        // Clear buffer and receive response from server
        memset(buff, 0, sizeof(buff));
        int bytes_read = read(sockfd, buff, sizeof(buff) - 1);
        if (bytes_read < 0) {
            perror("Read from server failed");
            break;
        }
        buff[bytes_read] = '\0'; // Null-terminate received message
        printf("From Server: %s", buff);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sd, port;
    struct sockaddr_in server;

    port = atoi(argv[1]);

    // Create a TCP socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port); // Convert port to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Connection to server failed");
        close(sd);
        exit(EXIT_FAILURE);
    }
  
    communicate_with_server(sd);  // Start communication with server
    
    close(sd); // Close socket after communication ends
    return 0;
}

