/**
 * @file main.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @language english
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
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
pcap_t *handle;

// Funkce pro správné ukončení programu
void signal_handler(int sig) {
    if (sig == SIGINT) {
        // Přeruší smyčku pcap
        pcap_breakloop(handle); 
    }
}

// Struktura pro ukládání statistik pro každé spojení
typedef struct {
    char src_ip[INET_ADDRSTRLEN];
    int src_port;
    char dst_ip[INET_ADDRSTRLEN];
    int dst_port;
    const char *protocol; // Pro uložení názvu protokolu (TCP/UDP)
    uint64_t rx_bytes; // Přijaté bajty
    uint64_t tx_bytes; // Odeslané bajty
} connection_stat_t;

int zadano = 0;

char filter_exp[255] = "ip";
struct bpf_program filter;
connection_stat_t connection;
struct ethernet {
    unsigned char ether_dhost[ETHER_ADDR_LEN]; 
    unsigned char ether_shost[ETHER_ADDR_LEN];
    unsigned short ether_type;
};

const char *get_protokol_name(int proto){
    switch (proto)
    {
    case IPPROTO_TCP: 
            return "tcp";
        break;
    case IPPROTO_UDP: 
            return "udp";
        break;
    case IPPROTO_ICMP: 
        return "icmp";
    break;
    default:
        return "";
        break;
    }
}
void packet_handler(unsigned char *user __attribute__((unused)), const struct pcap_pkthdr *header __attribute__((unused)), const unsigned char *packet)
{
   // Print source and destination MAC address
    struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));

        // Convert source and destination IP addresses to string format
        inet_ntop(AF_INET, &ip_header->saddr, connection.src_ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &ip_header->daddr, connection.dst_ip, INET_ADDRSTRLEN);
        
        connection.protocol  = get_protokol_name(ip_header->protocol);
        connection.src_port = *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4));
        connection.dst_port= *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4 + 2));
        if (zadano != 1)
        {
            printf("Src IP:Port\t\tDst IP:Port\t\tProto\tRX Bytes\tTX Bytes\n");
            printf("--------------------------------------------------------------------------\n");
            zadano = 1;
        }else{
            printf("%s: %d\t%s: %d\t%s\t%d\t%d\n",connection.src_ip,ntohs(connection.src_port),connection.dst_ip,ntohs(connection.dst_port), connection.protocol, header->len,  header->len);
        }

}



int main(int argc, char* argv[])
{
    signal(SIGINT, signal_handler);
/**
 * @brief arguments program
 */
    int opt;
    int option_index = 0;
    char interface[255] = "";
    int interface_flag = 0;
    char err_buf[PCAP_ERRBUF_SIZE];

    static struct option long_options[] = {
        {"interface", optional_argument, NULL, 'i'},
        {"sorting", optional_argument, NULL, 's'},
        {"time", optional_argument, NULL, 't'},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "i:s:t:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i':
                if (argv[optind] == NULL) {
                    interface_flag = 1;
                    break;
                }
                strncpy(interface, argv[optind], 254);

                interface_flag = 1;
                break;
            case 's':
              //  printf("s zadáno");
                break;
            case 't':
              //  to cas aktualizace
                break;
            default:
                printf("Usage: ./isa-top -i int [-s b|p]");
                exit(EXIT_FAILURE);
        }
    }

    //printf("here");
    //If no interface is provided, print a list of available interfaces and exit
    if (strcmp(interface, "") == 0) {
        if ((interface_flag == 1 && argc == 2) || (interface_flag == 0 && argc == 1)) {
            pcap_if_t *interfaces;
            // pcap_findalldevs returns a linked list of available interfaces
                if (pcap_findalldevs(&interfaces, err_buf) == 0) {
                    printf("\n");
                    // loop through all interfaces and print their names
                    for (pcap_if_t *interface = interfaces; interface != NULL; interface = interface->next) {
                        printf("%s\n", interface->name);
                    }
                    printf("\n");
                    // free the memory allocated by pcap_findalldevs
                    pcap_freealldevs(interfaces);
                } else {
                    // if pcap_findalldevs fails, print an error message
                }
            return 0;
        } else {
            exit(1);
        }
    }
    //printf("here2");

    /**
     * @brief 
     * 
     */
    handle = pcap_open_live(interface, BUFSIZ, 1, 1000, err_buf);
    if (handle == NULL) {
        exit(2);
    }
        
   // printf("here3");

    /**
    * @brief Used to translate a text string containing a BPF (Berkeley Packet Filter)
    */
    if (pcap_compile(handle, &filter, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        exit(2);
    }

   // printf("here4");
    if (pcap_setfilter(handle, &filter) == -1) {
        exit(2);
    }


   // printf("here5");
        /**
     * @brief Used to capture packets on the specified capture interface within a loop.
     */
    //num_packets
        pcap_dispatch(handle, 11 , packet_handler, NULL);
    /**
     * @brief Used to free the memory allocated for the compiled filter stored in the bpf_program structure
     */
    pcap_freecode(&filter);

    pcap_close(handle);
    //*****
   // printf("end");
    return 0;
}
