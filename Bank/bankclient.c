#include <netinet/in.h> // structure for storing address information
#include <stdio.h>
#include <sys/socket.h> // for socket APIs
#include <sys/types.h>
#include <unistd.h> // for close()

int main(int argc, char const* argv[]) {
    int sockD = socket(AF_INET, SOCK_STREAM, 0);
    //int sockD2 = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9001); // use some unused port number
    servAddr.sin_addr.s_addr = INADDR_ANY;
    
    int val, id1, id2;
    int temp, otp, temp_otp;
    
    printf("Enter Your Account ID: ");
    scanf("%d", &id1);
    
    printf("Enter Account ID to send money: ");
    scanf("%d", &id2);
    
    printf("Enter Amount: ");
    scanf("%d", &val);

    
    int connectStatus = connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
    if (connectStatus == -1) {
        printf("Error...\n");
    } else {
        char strData[255];
        recv(sockD, strData, sizeof(strData), 0);
        printf("Message: %s\n", strData);
        recv(sockD, &otp, sizeof(otp), 0);
        printf("\nAPKA OTP HAIN : %d\n", otp);

        printf("\nENTER OTP : \n");
        scanf("%d", &temp_otp);
        
        char serMsg[255] = "Message from the client to the server 'Hello server' ";
        send(sockD, serMsg, sizeof(serMsg), 0);
        
        temp = 0;
        int r_temp;
        send(sockD, &temp, sizeof(temp), 0);
        send(sockD, &id1, sizeof(id1), 0);
        send(sockD, &val, sizeof(val), 0);
        send(sockD, &temp_otp, sizeof(temp_otp), 0);
        
        recv(sockD, &r_temp, sizeof(r_temp), 0);
        
        if(r_temp == 500){
            printf("WRONG OTP");
            return -1;
        }
        if (r_temp == 9999) {
            printf("Insufficient Funds\n");
            return -1;
        } else {
            printf("Successfully Debited\n");
        }
    }
    
    close(sockD);
    
    /*while (1) {
        connectStatus = connect(sockD2, (struct sockaddr*)&servAddr, sizeof(servAddr));
        if (connectStatus == -1) {
            printf("Error...\n");
        } else {
            char strData[255];
            recv(sockD2, strData, sizeof(strData), 0);
            printf("Message: %s\n", strData);
            
            char serMsg1[255] = "Message from the client to the server 'Hello server' ";
            send(sockD2, serMsg1, sizeof(serMsg1), 0);
            
            int r1_temp;
            temp = 1;
            send(sockD2, &temp, sizeof(temp), 0);
            send(sockD2, &id2, sizeof(id2), 0);
            send(sockD2, &val, sizeof(val), 0);
            
            recv(sockD2, &r1_temp, sizeof(r1_temp), 0);
            
            if (r1_temp == -1) {
                printf("Error Funds\n");
                return -1;
            } else {
                printf("Successfully Credited\n");
                return 0;
            }
        }
    }*/
    
    return 0;
}

