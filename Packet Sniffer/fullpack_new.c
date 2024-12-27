#include<stdio.h>   // For standard things
#include<stdlib.h>  // malloc
#include<string.h>  // memset
#include<netinet/ip_icmp.h>  // For icmp header
#include<netinet/udp.h>      // For udp header
#include<netinet/tcp.h>      // For tcp header
#include<netinet/ip.h>       // For ip header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/if_ether.h> // For ETH_P_ALL
#include<netinet/ether.h>    // For ethernet header
#include<unistd.h>           // For close

// Constants
#define ETH_P_ALL 0x0003   // Capture all protocols

// Function prototypes
void ProcessPacket(unsigned char*, int);
void print_ip_header(unsigned char*, int);
void print_tcp_packet(unsigned char*, int);
void print_udp_packet(unsigned char*, int);
void print_icmp_packet(unsigned char*, int);
void PrintData(unsigned char*, int);

// Global variables
int sock_raw;
FILE *logfile;
int tcp = 0, udp = 0, icmp = 0, others = 0, igmp = 0, total = 0, i, j;
struct sockaddr_in source, dest;

int main()
{
    int saddr_size, data_size;
    struct sockaddr saddr;
    struct in_addr in;

    unsigned char *buffer = (unsigned char *)malloc(65536); // Its Big!

    logfile = fopen("log.txt", "w");
    if (logfile == NULL) {
        printf("Unable to create file.");
        return 1;
    }
    printf("Starting...\n");

    // Create a raw socket using AF_PACKET, SOCK_RAW, and htons(ETH_P_ALL) to capture all packets
    sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_raw < 0) {
        perror("Socket Error");
        return 1;
    }

    while (1)
    {
        saddr_size = sizeof saddr;
        // Receive a packet
        data_size = recvfrom(sock_raw, buffer, 65536, 0, &saddr, (socklen_t*)&saddr_size);
        if (data_size < 0) {
            printf("Recvfrom error, failed to get packets\n");
            return 1;
        }
        // Process the packet
        ProcessPacket(buffer, data_size);
    }
    fclose(logfile);  // Close the log file
    close(sock_raw);  // Close the socket
    printf("Finished");
    return 0;
}

void ProcessPacket(unsigned char* buffer, int size)
{
    // Get the IP Header part of this packet (skip the Ethernet header)
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr)); // Move to IP header after Ethernet header
    ++total;

    switch (iph->protocol) // Check the Protocol and act accordingly
    {
        case 1:  // ICMP Protocol
            ++icmp;
            print_icmp_packet(buffer, size);
            break;

        case 2:  // IGMP Protocol
            ++igmp;
            break;

        case 6:  // TCP Protocol
            ++tcp;
            print_tcp_packet(buffer, size);
            break;

        case 17: // UDP Protocol
            ++udp;
            print_udp_packet(buffer, size);
            break;

        default: // Other Protocols
            ++others;
            break;
    }
    printf("TCP: %d   UDP: %d   ICMP: %d   IGMP: %d   Others: %d   Total: %d\r", tcp, udp, icmp, igmp, others, total);
}

void print_ip_header(unsigned char* Buffer, int Size)
{
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr*)(Buffer + sizeof(struct ethhdr)); // Skip Ethernet header

    iphdrlen = iph->ihl * 4;

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    fprintf(logfile, "\n");
    fprintf(logfile, "IP Header\n");
    fprintf(logfile, "   |-IP Version        : %d\n", (unsigned int)iph->version);
    fprintf(logfile, "   |-IP Header Length  : %d DWORDS or %d Bytes\n", (unsigned int)iph->ihl, ((unsigned int)(iph->ihl)) * 4);
    fprintf(logfile, "   |-Type Of Service   : %d\n", (unsigned int)iph->tos);
    fprintf(logfile, "   |-IP Total Length   : %d  Bytes(Size of Packet)\n", ntohs(iph->tot_len));
    fprintf(logfile, "   |-Identification    : %d\n", ntohs(iph->id));
    fprintf(logfile, "   |-TTL      : %d\n", (unsigned int)iph->ttl);
    fprintf(logfile, "   |-Protocol : %d\n", (unsigned int)iph->protocol);
    fprintf(logfile, "   |-Checksum : %d\n", ntohs(iph->check));
    fprintf(logfile, "   |-Source IP        : %s\n", inet_ntoa(source.sin_addr));
    fprintf(logfile, "   |-Destination IP   : %s\n", inet_ntoa(dest.sin_addr));
    fflush(logfile); // Flush the buffer to ensure it's written to disk
}

// Minimal implementations for logging in these functions
void print_tcp_packet(unsigned char* Buffer, int Size) {
    fprintf(logfile, "TCP Packet\n");
    print_ip_header(Buffer, Size);
    fflush(logfile);
}

void print_udp_packet(unsigned char* Buffer, int Size) {
    fprintf(logfile, "UDP Packet\n");
    print_ip_header(Buffer, Size);
    fflush(logfile);
}

void print_icmp_packet(unsigned char* Buffer, int Size) {
    fprintf(logfile, "ICMP Packet\n");
    print_ip_header(Buffer, Size);
    fflush(logfile);
}

// Minimal PrintData function for data dump
void PrintData(unsigned char* data, int Size) {
    for (i = 0; i < Size; i++) {
        if (i % 16 == 0) fprintf(logfile, "\n");
        fprintf(logfile, " %02X", data[i]);
    }
    fflush(logfile);
}

