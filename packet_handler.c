/**
 * @file packet_handler.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "packet_handler.h"

//========================================================================================================================
// TODO předelat funkci
void process_ipv4_packet(const unsigned char *packet, packet_info_t *new_packet) {
     // IPv4 hlavička
    const struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));

    // Získání zdrojové a cílové IP adresy
    struct in_addr src_ip, dst_ip;
    src_ip.s_addr = ip_header->saddr;  // Zdrojová IP
    dst_ip.s_addr = ip_header->daddr;  // Cílová IP

    // Konverze IP adres na čitelný text
    inet_ntop(AF_INET, &src_ip, new_packet->src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &dst_ip, new_packet->dst_ip, INET_ADDRSTRLEN);
    
    // Extrakce protokolu
    uint8_t protocol = ip_header->protocol;
    struct protoent *protocolname = getprotobynumber(protocol);
    
    if (protocolname != NULL) {
        strncpy(new_packet->protocol, protocolname->p_name, sizeof(new_packet->protocol));
    } else {
        printf("Protokol nebyl nalezen.\n");
    }
    
    // Celková délka paketu
    new_packet->byte = ntohs(ip_header->tot_len); // Celková délka paketu
    new_packet->src_port = 0; // Inicializace portů, pokud neplatí
    new_packet->dst_port = 0;

    // Pokud protokol není ICMP, extrakce portů
    if (protocol != IPPROTO_ICMP) {
        uint8_t ihl = ip_header->ihl * 4; // Převod z 32-bitového slova na bajty
        new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ihl))); // Zdrojový port
        new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ihl + 2))); // Cílový port
    }
}
//========================================================================================================================


void process_ipv6_packet(const unsigned char *packet, packet_info_t *new_packet) {
    struct ip6_hdr *ip6_header = (struct ip6_hdr *)(packet + sizeof(struct ether_header));
    struct protoent *protocolname;

snprintf(new_packet->src_ip, INET6_ADDRSTRLEN, "[%s]", 
             inet_ntop(AF_INET6, &ip6_header->ip6_src, new_packet->src_ip + 1, INET6_ADDRSTRLEN - 2));
    
    snprintf(new_packet->dst_ip, INET6_ADDRSTRLEN, "[%s]", inet_ntop(AF_INET6, &ip6_header->ip6_dst, new_packet->dst_ip + 1, INET6_ADDRSTRLEN - 2));


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
    memset(&new_packet, 0, sizeof(packet_info_t));
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
    // find packet or add packet to vector
    add_packet_to_vector((void*)thread_args, new_packet);
    pthread_mutex_unlock(&thread_args->vector_mutex);
}

void* packet_capture_thread(void* arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Open capture
    if(thread_args->args.pcap_file_provided){

        char errbuf[PCAP_ERRBUF_SIZE];
        thread_args->handle = pcap_open_offline(thread_args->args.pcap_file_path, errbuf);
        if (thread_args->handle == NULL) {
            fprintf(stderr, "Could not open pcap file %s: %s\n", thread_args->args.pcap_file_path, errbuf);
            free_packet_vector(thread_args->packets);
            free(thread_args);
                  return NULL;
        }
    }else if (thread_args->args.interface_provided)
    {

        thread_args->handle = pcap_open_live(thread_args->args.interface, BUFSIZ, 1, 1000, errbuf);
        if (thread_args->handle== NULL) {
            fprintf(stderr, "Chyba při otevření zařízení: %s\n", errbuf);
        return NULL;
        }
    }
    
    // Capture
    while (!thread_args->break_while) {
        pcap_loop(thread_args->handle, -1, packet_handler, (unsigned char*)thread_args);
      
    }
    
    // Close capture
    pcap_close(thread_args->handle);
    return NULL;
}
