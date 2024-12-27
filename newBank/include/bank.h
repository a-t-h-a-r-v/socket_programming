#ifndef BANK_H
#define BANK_H

typedef struct account{
    int accNo;
    float balance;
    char password[30];
    struct account *next;
}ACCOUNT;

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

ACCOUNT* createAccount(int accNo, float balance);
void insertAccount(ACCOUNT **head, int accNo, float balance);
int serverConnection(int port);
int clientConnection(const char *server_ip, int port);
float findBalance(ACCOUNT *head, int accNo);
bool findAccount(ACCOUNT *head, int accNo);
bool debitMoney(ACCOUNT *head, int accNo, float amount);
void creditMoney(ACCOUNT *head, int accNo, float amount);
int transferAmount(ACCOUNT *head, int destAccNo, int srcAccNo, float amount);
bool checkPassword(ACCOUNT *head, int accNo, char password[30]);
void changePassword(ACCOUNT *head, int accNo, char password[30]);

#endif
