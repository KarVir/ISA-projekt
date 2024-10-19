/**
 * @file packet_handler.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "packet_handler.h"

// TODO předelat funkci
void process_ipv4_packet(const unsigned char *packet, packet_info_t *new_packet) {
    // Posun na začátek IPv4 hlavičky (za Ethernet hlavičku)
    const unsigned char *ip_header = packet + sizeof(struct ether_header);
    struct protoent *protocolname;

    // Získání IHL (délka IPv4 hlavičky v násobcích 4 bajtů)
    uint8_t ihl = (*ip_header & 0x0F) * 4;

    // Extrakce zdrojové a cílové IP adresy (pozice v hlavičce IP)
    struct in_addr src_ip, dst_ip;
    memcpy(&src_ip, ip_header + 12, sizeof(src_ip));  // Zdrojová IP je na bytech 12-15
    memcpy(&dst_ip, ip_header + 16, sizeof(dst_ip));  // Cílová IP je na bytech 16-19
    
    // Konverze IP adres na čitelný text
    inet_ntop(AF_INET, &src_ip, new_packet->src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &dst_ip, new_packet->dst_ip, INET_ADDRSTRLEN);

    // Extrakce protokolu (1 byte na pozici 9 v IPv4 hlavičce)
    uint8_t protocol = *(ip_header + 9);
    protocolname = getprotobynumber(protocol);

    
    if (protocolname != NULL) {
        if (protocolname->p_proto != IPPROTO_ICMP)
        {
            strncpy(new_packet->protocol, protocolname->p_name, sizeof(new_packet->protocol));
            new_packet->src_port = ntohs(*((uint16_t *)(ip_header + ihl)));
            new_packet->dst_port = ntohs(*((uint16_t *)(ip_header + ihl + 2)));
        }
        
    } else {
        printf("Protokol nebyl nalezen.\n");
    }
    // Celková délka paketu (2 byty na pozici 2-3 v IPv4 hlavičce)
    new_packet->byte = ntohs(*((uint16_t *)(ip_header + 2)));
}



void process_ipv6_packet(const unsigned char *packet, packet_info_t *new_packet) {
    struct ip6_hdr *ip6_header = (struct ip6_hdr *)(packet + sizeof(struct ether_header));
    struct protoent *protocolname;

    inet_ntop(AF_INET6, &ip6_header->ip6_src, new_packet->src_ip, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &ip6_header->ip6_dst, new_packet->dst_ip, INET6_ADDRSTRLEN);

    protocolname = getprotobynumber(ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt);

    if (protocolname != NULL) {
        if (protocolname->p_proto != IPPROTO_ICMPV6)
        {
            strncpy(new_packet->protocol, protocolname->p_name, sizeof(new_packet->protocol));
            new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr))));
            new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr) + 2)));
        }   
    } else {
        printf("Protokol nebyl nalezen.\n");
    }

    new_packet->byte = ntohs(ip6_header->ip6_ctlun.ip6_un1.ip6_un1_plen);
}

void packet_handler(unsigned char *user, const struct pcap_pkthdr *header __attribute__((unused)), const unsigned char *packet) {
    thread_args_t* thread_args = (thread_args_t*)user;

    packet_info_t new_packet;
    const struct ether_header *ethernet = (struct ether_header *)packet;
    
    switch (ntohs(ethernet->ether_type)) {
        case ETHERTYPE_IP:
            process_ipv4_packet(packet, &new_packet);
            break;
        case ETHERTYPE_IPV6:
            process_ipv6_packet(packet, &new_packet);
            break;
        default:
            break;
    }

    pthread_mutex_lock(&thread_args->vector_mutex);
    add_packet_to_vector( (void*)thread_args, new_packet);
    pthread_mutex_unlock(&thread_args->vector_mutex);
}

void* packet_capture_thread(void* arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Open capture
    thread_args->handle = pcap_open_live(thread_args->args.interface, BUFSIZ, 1, 1000, errbuf);
    if (thread_args->handle== NULL) {
        fprintf(stderr, "Chyba při otevření zařízení: %s\n", errbuf);
        return NULL;
    }

    // Capture
    while (!thread_args->break_while) {
        pcap_loop(thread_args->handle, -1, packet_handler, (unsigned char*)thread_args);
    }
    
    // Close capture
    pcap_close(thread_args->handle);
    return NULL;
}
