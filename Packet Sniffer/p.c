#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //memset
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<sys/socket.h>
#include<netinet/if_ether.h>
#include<netinet/ether.h>
#include<arpa/inet.h>
 
 
int sock_raw;
struct sockaddr_in source,dest;
 
int main()
{
    //Variables declaration
    int saddr_size, data_size;
    struct sockaddr saddr;
    struct in_addr in;
    char buffer[42];
    unsigned short iphdrlen;
     
    printf("Starting...\n");
   

    //Create a raw socket that shall sniff
    sock_raw = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
    
    if(sock_raw < 0)
    {
        printf("Socket Error\n");
        return 1;
    }

    while(1)
    {
        saddr_size = sizeof saddr;
        //Receive a packet
        data_size = recvfrom(sock_raw , buffer , 1000, 0 , &saddr , &saddr_size);
                    
    struct iphdr *iph = (struct iphdr *) buffer;
    iphdrlen =iph->ihl*4;
    
    struct ethhdr *eth = (struct ethhdr *)buffer;
    source.sin_addr.s_addr = iph->saddr;
     

    dest.sin_addr.s_addr = iph->daddr;
    
     
    
    printf( "Ethernet Header\n");
    printf( "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
    printf( "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    printf("   |-Total Length : %d \n",  sizeof(struct ethhdr));
    

//Print the captured IP header values
    printf("IP Header\n");
    printf("   |-IP Version        : %d\n",(unsigned int)iph->version);
    printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    printf("   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    printf("   |-Identification    : %d\n",ntohs(iph->id));
    printf("   |-TTL      : %d\n",(unsigned int)iph->ttl);
    printf("   |-Protocol : %d\n",(unsigned int)iph->protocol);
    printf("   |-Checksum : %d\n",ntohs(iph->check));
    printf("   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));
    printf("   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
           
    }
    // Close the socket
    close(sock_raw);
    printf("Finished");
    return 0;
}
 

