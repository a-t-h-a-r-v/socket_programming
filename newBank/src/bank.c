#include "bank.h"

int serverConnection(int port){
    int server_fd, client_socket, choice, accNoCount = 1, tempAccNo, boolPass = 0, loggedInAccountNumber;
    float tempBalance;
    char tempPassword[30];
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    ACCOUNT *accounts = NULL;
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0){
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while(1){
        if((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0){
            perror("Accept failed");
            continue;
        }
        read(client_socket, &choice, sizeof(choice));
        if(choice == 1){
            read(client_socket, &tempAccNo, sizeof(tempAccNo));
            read(client_socket, tempPassword, sizeof(tempPassword));
            if(checkPassword(accounts, tempAccNo, tempPassword)){
                boolPass = 1;
                loggedInAccountNumber = tempAccNo;
                write(client_socket, &boolPass, sizeof(boolPass));
                read(client_socket, &choice, sizeof(choice));
                if(choice == 1){
                    tempBalance = findBalance(accounts, loggedInAccountNumber);
                    write(client_socket, &tempBalance, sizeof(tempBalance));
                }
                else if(choice == 2){
                    read(client_socket, &tempAccNo, sizeof(tempAccNo));
                    read(client_socket, &tempBalance, sizeof(tempBalance));
                    int temp1 = transferAmount(accounts, tempAccNo, loggedInAccountNumber, tempBalance);
                    write(client_socket, &temp1, sizeof(temp1));
                }
                else if(choice == 3){
                    read(client_socket, tempPassword, sizeof(tempPassword));
                    if(checkPassword(accounts, loggedInAccountNumber, tempPassword)){
                        boolPass = 1;
                        write(client_socket, &boolPass, sizeof(boolPass));
                        read(client_socket, tempPassword, sizeof(tempPassword));
                        if(checkPassword(accounts, loggedInAccountNumber, tempPassword)){
                            boolPass = 1;
                            write(client_socket, &boolPass, sizeof(boolPass));
                            changePassword(accounts, loggedInAccountNumber, tempPassword);
                        }
                        else{
                            boolPass = 0;
                            write(client_socket, &boolPass, sizeof(boolPass));
                        }
                    }
                    else{
                        boolPass = 0;
                        write(client_socket, &boolPass, sizeof(boolPass));
                    }
                }
            }
            else{
                boolPass = 0;
                write(client_socket, &boolPass, sizeof(boolPass));
            }
        }
        else if(choice == 2){
            read(client_socket, &tempBalance, sizeof(tempBalance));
            read(client_socket, tempPassword, sizeof(tempPassword));
            insertAccount(&accounts, accNoCount, tempBalance);
            write(client_socket, &accNoCount, sizeof(accNoCount));
            changePassword(accounts, accNoCount, tempPassword);
            accNoCount++;
        }
        close(client_socket);
    }
    return 0;
}

int clientConnection(const char *server_ip, int port){
    int client_fd, choice, tempAccNo, boolPass = 0;
    float tempBalance;
    char tempPassword[30];
    struct sockaddr_in address;

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if(inet_pton(AF_INET, server_ip, &address.sin_addr) <= 0){
        perror("Invalid address/ Address not supported");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    if(connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("Connection failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("1.Login\t2.Create Account\n");
    printf("Enter your choice : ");
    scanf("%d", &choice);
    write(client_fd, &choice, sizeof(choice));
    if(choice == 1){
        printf("Enter the account number");
        scanf("%d", &tempAccNo);
        printf("Enter the password : ");
        scanf("%s", tempPassword);
        write(client_fd, &tempAccNo, sizeof(tempAccNo));
        write(client_fd, tempPassword, sizeof(tempPassword));
        read(client_fd, &boolPass, sizeof(boolPass));
        if(boolPass == 1){
            printf("Successfully Logged in.\n");
            printf("1.Check Balance\t2.Transfer Amount\t3.Change Password\t");
            scanf("%d", &choice);
            write(client_fd, &choice, sizeof(choice));
            if(choice == 1){
                read(client_fd, &tempBalance, sizeof(tempBalance));
                if(tempBalance == -1){
                    printf("Account number not found");
                }
                else{
                    printf("Balance : %f\n", tempBalance);
                }
            }
            else if(choice == 2){
                printf("Enter the account number you would like to send money to : ");
                scanf("%d", &tempAccNo);
                write(client_fd, &tempAccNo, sizeof(tempAccNo));
                printf("Enter the amount you would like to transfer : ");
                scanf("%f", &tempBalance);
                write(client_fd, &tempBalance, sizeof(tempBalance));
                read(client_fd, &tempAccNo, sizeof(tempAccNo));
                if(tempAccNo == 0){
                    printf("Successfully transfered amount\n");
                }
                else if(tempAccNo == 1){
                    printf("Insufficient balance in the source account");
                }
                else if(tempAccNo == 2){
                    printf("Account number not found\n");
                }
            }
            else if(choice == 3){
                printf("Enter the old password : ");
                scanf("%s", tempPassword);
                write(client_fd, tempPassword, sizeof(tempPassword));
                read(client_fd, &boolPass, sizeof(boolPass));
                if(boolPass == 1){
                    printf("Enter the new password : ");
                    scanf("%s", tempPassword);
                    write(client_fd, tempPassword, sizeof(tempPassword));
                    read(client_fd, &boolPass, sizeof(boolPass));
                    if(boolPass == 1){
                        printf("Password Changed Successfully\n");
                    }
                    else{
                        printf("Wrong password\n");
                    }
                }
                else{
                    printf("Wrong password\n");
                }
            }
        }
        else{
            printf("Wrong credentials.\n");
        }
    }
    else if(choice == 2){
        printf("Enter the amount you would like to deposit : ");
        scanf("%f", &tempBalance);
        printf("Enter the password : ");
        scanf("%s", tempPassword);
        write(client_fd, &tempBalance, sizeof(tempBalance));
        write(client_fd, tempPassword, sizeof(tempPassword));
        read(client_fd, &tempAccNo, sizeof(tempAccNo));
        printf("Created an account with account no %d and balance %02f\n", tempAccNo, tempBalance);
    }
    return 0;
}

ACCOUNT* createAccount(int accNo, float balance){
    ACCOUNT* newNode = (ACCOUNT*)malloc(sizeof(ACCOUNT));
    newNode->accNo = accNo;
    newNode->balance = balance;
    newNode->next = newNode;
    return newNode;
}

void insertAccount(ACCOUNT **head, int accNo, float balance){
    ACCOUNT* newNode = createAccount(accNo, balance);
    if(*head == NULL){
        *head = newNode;
        newNode->next = *head;
    }
    else{
        ACCOUNT* temp = *head;
        while(temp->next != *head){
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->next = *head;
    }
}

float findBalance(ACCOUNT *head, int accNo){
    if(head != NULL){
        ACCOUNT* temp = head;
        do{
            if(accNo == temp->accNo){
                return temp->balance;
            }
            temp = temp->next;
        }while(temp!=head);
    }
    return -1;
}

bool checkPassword(ACCOUNT *head, int accNo, char password[30]){
    if(head != NULL){
        ACCOUNT* temp = head;
        do{
            if(accNo == temp->accNo){
                if(strcmp(temp->password, password) == 0){
                    return true;
                }
            }
            temp = temp->next;
        }while(temp!=head);
    }
    return false;
}

void changePassword(ACCOUNT *head, int accNo, char password[30]){
    if(head != NULL){
        ACCOUNT* temp = head;
        do{
            if(accNo == temp->accNo){
                strcpy(temp->password, password);
            }
            temp = temp->next;
        }while(temp!=head);
    }
}

bool findAccount(ACCOUNT *head, int accNo){
    if(head != NULL){
        ACCOUNT* temp = head;
        do{
            if(accNo == temp->accNo){
                return true;
            }
            temp = temp->next;
        }while(temp!=head);
    }
    return false;
}

bool debitMoney(ACCOUNT *head, int accNo, float amount){
    if(head != NULL){
        ACCOUNT* temp = head;
        do{
            if(accNo == temp->accNo){
                if((temp->balance - amount) < 0){
                    return false;
                }
                temp->balance -= amount;
                return true;
            }
            temp = temp->next;
        }while(temp!=head);
    }
    return false;
}

void creditMoney(ACCOUNT *head, int accNo, float amount){
    if(head != NULL){
        ACCOUNT* temp = head;
        do{
            if(accNo == temp->accNo){
                temp->balance += amount;
            }
            temp = temp->next;
        }while(temp!=head);
    }
}
int transferAmount(ACCOUNT *head, int destAccNo, int srcAccNo, float amount){
    if(findAccount(head, destAccNo) && findAccount(head, srcAccNo)){
        if(debitMoney(head, srcAccNo, amount)){
            creditMoney(head, destAccNo, amount);
            return 0;
        }
        else{
            return 1;
        }
    }
    else{
        return 2;
    }
    return 0;
}
