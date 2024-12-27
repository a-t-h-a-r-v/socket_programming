#include <netinet/in.h>  // structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>  // for socket APIs
#include <sys/types.h>
#include <unistd.h>       // for close()
#include <time.h>       // for close()

int main(int argc, char const* argv[]) {
    // Create server socket similar to what was done in client program
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);
    
    // String store data to send to client
    char serMsg[255] = "Message from the server to the client 'Hello Client' ";
    
    // Define server address
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9001);
    servAddr.sin_addr.s_addr = INADDR_ANY;
    
    int acc_arr[] = {100, 200, 500, 700, 800};
    int acti, accno, amt, otp, temp_otp;
    
    // Bind socket to the specified IP and port
    bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
    
    int msg;
    
    // Listen for connections
    listen(servSockD, 1);
    
    while (1) {
        printf("ano\tamt\n");
        for (int i = 0; i < 5; i++) {
            printf("%d\t%d\n", i, acc_arr[i]);
        }
        
        // Integer to hold client socket
        int clientSocket = accept(servSockD, NULL, NULL);
        
        // Send messages to client socket
        send(clientSocket, serMsg, sizeof(serMsg), 0);
        
        char strData[255];
        otp = 0;
        while(otp < 1000){
            srand(time(NULL));
            otp = rand()%10000;
        }
        send(clientSocket, &otp, sizeof(otp), 0);
        recv(clientSocket, strData, sizeof(strData), 0);
        printf("Message: %s\n", strData);
        
        recv(clientSocket, &acti, sizeof(acti), 0);
        recv(clientSocket, &accno, sizeof(accno), 0);
        recv(clientSocket, &amt, sizeof(amt), 0);
        recv(clientSocket, &temp_otp, sizeof(temp_otp), 0);

        if (acti == 0) {
            if(temp_otp != otp){
                msg = 500;
                send(clientSocket, &msg, sizeof(msg), 0);
            }
            else{
                if (amt > acc_arr[accno]) {
                    msg = 9999;
                    send(clientSocket, &msg, sizeof(msg), 0);
                } else {
                    acc_arr[accno] -= amt;
                    msg = 1;
                    send(clientSocket, &msg, sizeof(msg), 0);
                }
            }
        }
        
        if (acti == 1) {
            acc_arr[accno] += amt;
            msg = 1;
            send(clientSocket, &msg, sizeof(msg), 0);
        }
        
        close(clientSocket);
    }
    
    return 0;
}

