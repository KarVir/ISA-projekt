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

#define MAX_connection 1000
typedef struct {
    int time;
    char interface[255];
    char sorting_criteria[2];
} arg_t;
// Struktura pro ukládání statistik pro každé spojení
typedef struct {
    char src_ip[INET6_ADDRSTRLEN];
    char dst_ip[INET6_ADDRSTRLEN];
    uint16_t src_port;
    uint16_t dst_port;
    char protocol[5];
    uint64_t rx_bytes;  // Přijaté bajty (RX)
    uint64_t tx_bytes;  // Odeslané bajty (TX)
    uint64_t rx_packets; // Počet přijatých paketů
    uint64_t tx_packets; // Počet odeslaných paketů
} connection_stat_t;

/**
 * @brief 
 * 
 * @param __attribute__ 
 * @param header 
 * @param packet 
 */
void packet_handler(unsigned char *user __attribute__((unused)), const struct pcap_pkthdr *header , const unsigned char *packet);

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int compare_rx_bytes(const void *a, const void *b);


/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int compare_rx_packets(const void *a, const void *b);