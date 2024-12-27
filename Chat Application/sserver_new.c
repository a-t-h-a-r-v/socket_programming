#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define SERVER_TCP_PORT 5750  /* Default TCP port */
#define BUFLEN 256            /* Buffer length */
#define MAX 80                /* Max message size */

void handle_client(int sockfd) {
    char buff[MAX];
    int n;

    for (;;) {
        memset(buff, 0, MAX);
        
        // Read message from client
        int read_bytes = read(sockfd, buff, sizeof(buff) - 1);
        if (read_bytes < 0) {
            perror("Read from client failed");
            break;
        }
        buff[read_bytes] = '\0';  // Null-terminate the received message
        printf("From client: %s\nTo client: ", buff);

        // Get server input
        memset(buff, 0, MAX);
        if (fgets(buff, MAX, stdin) == NULL) {
            perror("Input failed");
            break;
        }

        // Send response to client
        if (write(sockfd, buff, strlen(buff)) < 0) {
            perror("Write to client failed");
            break;
        }

        // Exit if "exit" command is sent
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main(int argc, char **argv) {
    int sd, new_sd, port;
    struct sockaddr_in server, client;
    socklen_t client_len;
    int yes = 1;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);

    // Create a TCP socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Allow socket reuse
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("Setting socket options failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);              // Convert port to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the socket
    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Binding socket failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(sd, 5) == -1) {
        perror("Listen failed");
        close(sd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", port);

    // Accept and handle client connections
    while (1) {
        client_len = sizeof(client);
        if ((new_sd = accept(sd, (struct sockaddr *)&client, &client_len)) == -1) {
            perror("Accept failed");
            continue;  // Continue to accept next connections
        }

        printf("Connected to client\n");
        handle_client(new_sd);  // Communicate with client
        close(new_sd);          // Close client connection
        printf("Client disconnected\n");
    }

    close(sd);  // Close server socket
    return 0;
}

