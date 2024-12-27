#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFLEN 256

int main(int argc, char **argv) {
    int sd, port, n;
    char buf[BUFLEN];
    struct sockaddr_in server;

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

    // Configure server address structure
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);  // Convert port to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Connection to server failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    printf("Enter message to be echoed: ");
    if (fgets(buf, BUFLEN, stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Send message to server
    if (write(sd, buf, strlen(buf)) == -1) {
        perror("Write to server failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Receive echoed message from server
    n = read(sd, buf, sizeof(buf) - 1); // Leave space for null terminator
    if (n == -1) {
        perror("Read from server failed");
        close(sd);
        exit(EXIT_FAILURE);
    }
    buf[n] = '\0'; // Null-terminate the received message

    printf("Echoed Message:\n**************\n%s\n", buf);

    close(sd); // Close the socket connection
    return 0;
}

