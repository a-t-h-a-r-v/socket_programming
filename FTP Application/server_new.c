#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_TCP_PORT 5750   // Port for the server
#define BUFLEN 256             // Buffer size for messages
#define FILENAME_LEN 100       // Length for the filename

// Function to send a file to the client
void send_file(FILE *fp, int sockfd) {
    char data[BUFLEN] = {0};
    size_t n;
    while ((n = fread(data, sizeof(char), BUFLEN, fp)) > 0) {
        if (send(sockfd, data, n, 0) == -1) {
            perror("Error sending file");
            exit(1);
        }
        bzero(data, BUFLEN);
    }
}

// Function to receive a file from the client and save it to a file on the server
void receive_file(FILE *fp, int sockfd) {
    char data[BUFLEN] = {0};
    int n;
    while ((n = recv(sockfd, data, BUFLEN, 0)) > 0) {
        if (fwrite(data, sizeof(char), n, fp) < n) {
            perror("Error writing to file");
            exit(1);
        }
        bzero(data, BUFLEN);
    }
}

// Function to handle client interaction
void handle_client(int new_sd) {
    char buffer[BUFLEN];
    int n;
    FILE *fp;
    
    while (1) {
        bzero(buffer, BUFLEN);
        n = read(new_sd, buffer, sizeof(buffer));
        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Received command: %s", buffer);

        if (strncmp(buffer, "SEND_FILE", 9) == 0) {
            // Receive the filename from the client
            bzero(buffer, BUFLEN);
            read(new_sd, buffer, sizeof(buffer));
            printf("Receiving file, saving to 'received_file.txt'\n");

            // Open the file to store the received content
            fp = fopen("received_file.txt", "wb");
            if (fp == NULL) {
                perror("Error opening file to write received data");
                exit(1);
            }
            receive_file(fp, new_sd);
            printf("File received and saved successfully as 'received_file.txt'\n");
            fclose(fp);
        } else if (strncmp(buffer, "RECEIVE_FILE", 12) == 0) {
            // Send a fixed file to the client
            bzero(buffer, BUFLEN);
            read(new_sd, buffer, sizeof(buffer));
            printf("Sending file: %s\n", buffer);
            
            fp = fopen("server_file.txt", "rb");
            if (fp == NULL) {
                perror("Error opening file to send data to client");
                exit(1);
            }
            send_file(fp, new_sd);
            printf("File sent successfully\n");
            fclose(fp);
        }
    }
}

int main(int argc, char **argv) {
    int sd, new_sd, client_len;
    struct sockaddr_in server, client;
    int yes = 1;

    // Create socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set socket options (SO_REUSEADDR)
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt failed");
        exit(1);
    }

    // Prepare server address
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_TCP_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    // Listen for client connections
    listen(sd, 5);
    printf("Server is listening on port %d\n", SERVER_TCP_PORT);

    while (1) {
        client_len = sizeof(client);
        if ((new_sd = accept(sd, (struct sockaddr *)&client, &client_len)) == -1) {
            perror("Accept failed");
            exit(1);
        }
        printf("Client connected\n");

        // Handle client
        handle_client(new_sd);

        close(new_sd);
    }

    close(sd);
    return 0;
}

