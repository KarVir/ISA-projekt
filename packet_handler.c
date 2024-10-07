/**
 * @file isa-top.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "packet_handler.h"

void process_ipv4_packet(const unsigned char *packet, packet_info_t *new_packet) {
    struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));

    inet_ntop(AF_INET, &ip_header->saddr, new_packet->src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ip_header->daddr, new_packet->dst_ip, INET_ADDRSTRLEN);

    if (ip_header->protocol == IPPROTO_TCP) {
        new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4)));
        new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4 + 2)));
        strncpy(new_packet->protocol, "TCP", sizeof(new_packet->protocol));
    } else if (ip_header->protocol == IPPROTO_UDP) {
        new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4)));
        new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4 + 2)));
        strncpy(new_packet->protocol, "UDP", sizeof(new_packet->protocol));
    } else if (ip_header->protocol == IPPROTO_ICMP) {
        strncpy(new_packet->protocol, "ICMP", sizeof(new_packet->protocol));
        new_packet->src_port = 0;
        new_packet->dst_port = 0;
    }
    new_packet->byte = ntohs(ip_header->tot_len);
}


void process_ipv6_packet(const unsigned char *packet, packet_info_t *new_packet) {
    struct ip6_hdr *ip6_header = (struct ip6_hdr *)(packet + sizeof(struct ether_header));

    inet_ntop(AF_INET6, &ip6_header->ip6_src, new_packet->src_ip, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &ip6_header->ip6_dst, new_packet->dst_ip, INET6_ADDRSTRLEN);

    if (ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt == IPPROTO_TCP) {
        new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr))));
        new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr) + 2)));
        strncpy(new_packet->protocol, "TCP", sizeof(new_packet->protocol));
    } else if (ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt == IPPROTO_UDP) {
        new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr))));
        new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr) + 2)));
        strncpy(new_packet->protocol, "UDP", sizeof(new_packet->protocol));
    } else if (ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt == IPPROTO_ICMPV6) {
        strncpy(new_packet->protocol, "ICMPv6", sizeof(new_packet->protocol));
        new_packet->src_port = 0;
        new_packet->dst_port = 0;
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
            process_ipv4_packet(packet, &new_packet);
            return;
    }

    // Zámek mutexu pro přístup k vektoru, přidání paketu do vektoru
    pthread_mutex_lock(&thread_args->vector_mutex);
    add_packet_to_vector( (void*)thread_args, new_packet);
    pthread_mutex_unlock(&thread_args->vector_mutex);
}

// Vlákno pro zachytávání paketů
void* packet_capture_thread(void* arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Otevření zařízení
    thread_args->handle = pcap_open_live(thread_args->args.interface, BUFSIZ, 1, 1000, errbuf);
    if (thread_args->handle== NULL) {
        fprintf(stderr, "Chyba při otevření zařízení: %s\n", errbuf);
        return NULL;
    }

    while (!thread_args->break_while) {
        pcap_loop(thread_args->handle, -1, packet_handler, (unsigned char*)thread_args);
    }
    
    pcap_close(thread_args->handle);
    return NULL;
}
