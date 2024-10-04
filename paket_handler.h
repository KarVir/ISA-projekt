/**
 * @file main.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include <net/ethernet.h>
#include <stdio.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <pcap/pcap.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <ifaddrs.h>

#define MAX_PACKETS 10

/**
 * @brief program arguments
 */
typedef struct {
    int time;
    char interface[255];
    char sorting_criteria[2];
} arg_t;
// Struktura pro ukládání statistik pro každé spojen

typedef struct {
    char src_ip[INET6_ADDRSTRLEN];
    char dst_ip[INET6_ADDRSTRLEN];
    uint16_t src_port;
    uint16_t dst_port;
    char protocol[10];
    uint16_t rx_bytes;  // Přijaté bajty
    uint16_t tx_bytes;  // Odeslané bajty
     uint64_t rx_packets;  // Přijaté pakety
    uint64_t tx_packets;  // Odeslané pakety
    uint64_t count;
} packet_info_t;


void packet_handler(unsigned char *user __attribute__((unused)), const struct pcap_pkthdr *header , const unsigned char *packet);

/**
 * @brief 
 * 
 * @param saved_packets 
 * @param packet_count 
 * @param new_packet 
 * @return int 
 */
bool is_packet_duplicate(packet_info_t *saved_packets, int packet_count, packet_info_t *new_packet);

/**
 * @brief provnání podle velikosti bitů
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int compare_bytes(const void *a, const void *b);


/**
 * @brief porovnání podle velikosti paketů
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int compare_pakets(const void *a, const void *b);