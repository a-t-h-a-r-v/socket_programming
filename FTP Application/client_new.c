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

// Function to send a file to the server
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

// Function to receive a file from the server
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

// Function to interact with the server
void interact_with_server(int sd) {
    char buffer[BUFLEN];
    int n;
    FILE *fp;

    while (1) {
        printf("Enter command (SEND_FILE or RECEIVE_FILE): ");
        bzero(buffer, BUFLEN);
        fgets(buffer, BUFLEN, stdin);

        if (strncmp(buffer, "SEND_FILE", 9) == 0) {
            // Send a file to the server
            printf("Enter the filename to send: ");
            fgets(buffer, FILENAME_LEN, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';  // Remove the newline

            send(sd, "SEND_FILE", 9, 0);  // Tell the server we are sending a file
            send(sd, buffer, FILENAME_LEN, 0);  // Send the filename

            fp = fopen(buffer, "rb");
            if (fp == NULL) {
                perror("File opening failed");
                exit(1);
            }
            send_file(fp, sd);
            printf("File sent successfully\n");
            fclose(fp);
        } else if (strncmp(buffer, "RECEIVE_FILE", 12) == 0) {
            // Request a file from the server
            printf("Enter the filename to receive: ");
            fgets(buffer, FILENAME_LEN, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';  // Remove the newline

            send(sd, "RECEIVE_FILE", 12, 0);  // Tell the server we are receiving a file
            send(sd, buffer, FILENAME_LEN, 0);  // Send the filename

            // Open file to save the received content
            fp = fopen("received_file_from_server.txt", "wb");
            if (fp == NULL) {
                perror("File opening failed");
                exit(1);
            }

            printf("Receiving file from server and saving to 'received_file_from_server.txt'\n");
            receive_file(fp, sd);
            printf("File received and saved successfully\n");
            fclose(fp);
        }
    }
}

int main(int argc, char **argv) {
    int sd;
    struct sockaddr_in server;

    // Create socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Prepare server address
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_TCP_PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server\n");

    interact_with_server(sd);

    close(sd);
    return 0;
}

