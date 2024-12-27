#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFLEN 256

int main(int argc, char **argv) {
    int sd, new_sd, client_len, n, port;
    struct sockaddr_in server, client;
    char buf[BUFLEN];
    int yes = 1;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);

    // Create a TCP socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket option to reuse address
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("Setting socket options failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);  // Convert to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the socket to the address
    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Bind failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sd, 5) == -1) {
        perror("Listen failed");
        close(sd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", port);

    while (1) {
        client_len = sizeof(client);
        
        // Accept a new client connection
        if ((new_sd = accept(sd, (struct sockaddr *)&client, (socklen_t *)&client_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        // Read message from client
        n = read(new_sd, buf, sizeof(buf) - 1); // Leave space for null terminator
        if (n > 0) {
            buf[n] = '\0'; // Null-terminate the received message
            printf("Message received from client: %s\n", buf);
            
            // Echo the message back to client
            write(new_sd, buf, n);
        } else {
            perror("Read failed or connection closed by client");
        }

        close(new_sd); // Close the client connection
    }

    close(sd); // Close the server socket
    return 0;
}

